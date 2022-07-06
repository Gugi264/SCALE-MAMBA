/*
Copyright (c) 2017, The University of Bristol, Senate House, Tyndall Avenue, Bristol, BS8 1TH, United Kingdom.
Copyright (c) 2021, COSIC-KU Leuven, Kasteelpark Arenberg 10, bus 2452, B-3001 Leuven-Heverlee, Belgium.

All rights reserved
*/

#include "Input_Output_Simple.h"
#include "Exceptions/Exceptions.h"
#include <fcntl.h>
#include <sqlite3.h>


int tripleCounter = 0;
int sqlCounter = 0;
vector<string> a_shares;
vector<string> b_shares;
vector<string> c_shares;
sqlite3 *db;
sqlite3_stmt *pStmt;
char *sql = "INSERT INTO triples (shareA, shareB, shareC) VALUES(?,?,?)";
bool firstRun = true;
const int cacheSize = 10000;

long Input_Output_Simple::open_channel(unsigned int channel)
{
  cout << "Opening channel " << channel << endl;
  return 0;
}

void Input_Output_Simple::close_channel(unsigned int channel)
{
  cout << "Closing channel " << channel << endl;
}

gfp Input_Output_Simple::private_input_gfp(unsigned int channel)
{
  cout << "Input channel " << channel << " : ";
  word x;
  cin >> x;
  gfp y;
  y.assign(x);
  return y;
}

void Input_Output_Simple::private_output_gfp(const gfp &output, unsigned int channel)
{
  cout << "Output channel " << channel << " : ";
  output.output(cout, true);
  cout << endl;
}

gfp Input_Output_Simple::public_input_gfp(unsigned int channel)
{
  cout << "Enter value on channel " << channel << " : ";
  word x;
  cin >> x;
  gfp y;
  y.assign(x);

  // Important to have this call in each version of public_input_gfp
  Update_Checker(y, channel);

  return y;
}

void Input_Output_Simple::public_output_gfp(const gfp &output, unsigned int channel)
{
  cout << "Output channel " << channel << " : ";
  output.output(cout, true);
  cout << endl;
}

long Input_Output_Simple::public_input_int(unsigned int channel)
{
  cout << "Enter value on channel " << channel << " : ";
  long x;
  cin >> x;

  // Important to have this call in each version of public_input_gfp
  Update_Checker(x, channel);

  return x;
}

void Input_Output_Simple::public_output_int(const long output, unsigned int channel)
{
  cout << "Output channel " << channel << " : " << output;
  cout << " = 0x" << hex << output << dec << endl;
}

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
  int i;
  for(i = 0; i<argc; i++) {
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
  printf("\n");
  return 0;
}


void Input_Output_Simple::output_share(const Share &S, unsigned int channel)
{
//  (*outf) << "Output channel " << channel << " : ";
//  S.output(*outf, human);


  if (db == NULL) {
      char *zErrMsg = 0;
      int rc;
      string fileName = "mydb" + to_string(S.get_player()) + ".db";
      rc = sqlite3_open(fileName.c_str(), &db);

      if (rc)
        {
          fprintf(stderr, "Cant open database %s\n", sqlite3_errmsg(db));
          return ;
        }
      else
        {
          fprintf(stderr, "Opened succesfully\n");
        }
      stringstream sqlCreate;
      sqlCreate << "CREATE TABLE if not exists triples (";
      sqlCreate << "id INTEGER PRIMARY KEY AUTOINCREMENT,";
      sqlCreate << "shareA TEXT,";
      sqlCreate << "shareB TEXT,";
      sqlCreate << "shareC TEXT";
      sqlCreate << ");";

      rc = sqlite3_exec(db, sqlCreate.str().c_str(), callback, 0, &zErrMsg);
      if( rc != SQLITE_OK ){
          fprintf(stderr, "SQL error create table: %s\n", zErrMsg);
          sqlite3_free(zErrMsg);
        } else {
          fprintf(stdout, "Table created successfully\n");
        }
    }

  if (firstRun) {
      int rc = sqlite3_prepare_v2(db, sql, -1, &pStmt, 0);
      if (rc != SQLITE_OK) {
          cout << "Cannot prepare statement: " << sqlite3_errmsg(db) << endl;
          return ;
        }
      firstRun = false;
    }


  if (sqlCounter == 0) {
      sqlite3_exec(db, "BEGIN TRANSACTION;", NULL, NULL, NULL);
      a_shares.reserve(cacheSize);
      b_shares.reserve(cacheSize);
      c_shares.reserve(cacheSize);
    }

  bigint te;
  gfp share = S.get_share(0);
  to_bigint(te, share, true);
  stringstream tmpstringstream;
  tmpstringstream << te;
  string tmpString = tmpstringstream.str();
  switch (tripleCounter) {
      case 0:
        a_shares.push_back(tmpString);
        tripleCounter++;
        return;
      case 1:
        b_shares.push_back(tmpString);
        tripleCounter++;
        return ;
      case 2:
        c_shares.push_back(tmpString);
       //no return here as we want to perpare the sql statement if we have all 3 shares
    }
  sqlCounter++;
  int rc;
  rc = sqlite3_bind_text(pStmt, 1, a_shares.back().c_str(), a_shares.back().size(), SQLITE_STATIC);
  if (rc != SQLITE_OK) {
      cout << "Bind failed: " << sqlite3_errmsg(db) << endl;
    }
  sqlite3_bind_text(pStmt, 2, b_shares.back().c_str(), b_shares.back().size(), SQLITE_STATIC);
  sqlite3_bind_text(pStmt, 3, c_shares.back().c_str(), c_shares.back().size(), SQLITE_STATIC);
  rc = sqlite3_step(pStmt);
  if (rc != SQLITE_DONE) {
      cout << "Execution failed: " << sqlite3_errmsg(db) << endl;
    }
  sqlite3_reset(pStmt);
  sqlite3_clear_bindings(pStmt);
  if (sqlCounter == cacheSize) { // 10000 is around 1.2MB
      sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);
      //sqlite3_commit_hook(db, NULL, NULL);
      //rc = sqlite3_finalize(pStmt);
      //cout << "writing to db....." << endl;
//      if (rc != SQLITE_OK) {
//          cout << "Finalize failed: " << sqlite3_errmsg(db) << endl;
//        }
      sqlCounter = 0;
      a_shares.clear();
      b_shares.clear();
      c_shares.clear();
    }
  tripleCounter = 0;

}


Share Input_Output_Simple::input_share(unsigned int channel)
{
  cout << "Enter value on channel " << channel << " : ";
  Share S;
  S.input(*inpf, human);
  return S;
}

void Input_Output_Simple::trigger(Schedule &schedule)
{
  printf("Restart requested: Enter a number to proceed\n");
  int i;
  cin >> i;

  // Load new schedule file program streams, using the original
  // program name
  //
  // Here you could define programatically what the new
  // programs you want to run are, by directly editing the
  // public variables in the schedule object.
  unsigned int nthreads= schedule.Load_Programs();
  if (schedule.max_n_threads() < nthreads)
    {
      throw Processor_Error("Restart requires more threads, cannot do this");
    }
}

void Input_Output_Simple::debug_output(const stringstream &ss)
{
  printf("%s", ss.str().c_str());
  fflush(stdout);
}

void Input_Output_Simple::crash(unsigned int PC, unsigned int thread_num)
{
  printf("Crashing in thread %d at PC value %d\n", thread_num, PC);
  throw crash_requested();
}
