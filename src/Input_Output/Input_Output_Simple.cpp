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
vector<string> a_a_shares;
vector<string> b_a_shares;
vector<string> c_a_shares;
vector<string> a_b_shares;
vector<string> b_b_shares;
vector<string> c_b_shares;
vector<string> a_c_shares;
vector<string> b_c_shares;
vector<string> c_c_shares;
vector<string> a_r_shares;
vector<string> b_r_shares;
vector<string> c_r_shares;
vector<string> r_shares;
sqlite3 *db;
sqlite3_stmt *pStmt;
char *sql = "INSERT INTO triples (shareA, shareB, shareC, shareA_A, shareB_A, shareC_A, shareA_B, shareB_B, shareC_B, shareA_C, shareB_C, shareC_C, shareA_R, shareB_R, shareC_R, share_r) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)";
bool firstRun = true;
const int cacheSize = 10000; //6.6MB

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
      sqlCreate << "shareC TEXT,";
      sqlCreate << "shareA_A TEXT,";
      sqlCreate << "shareB_A TEXT,";
      sqlCreate << "shareC_A TEXT,";
      sqlCreate << "shareA_B TEXT,";
      sqlCreate << "shareB_B TEXT,";
      sqlCreate << "shareC_B TEXT,";
      sqlCreate << "shareA_C TEXT,";
      sqlCreate << "shareB_C TEXT,";
      sqlCreate << "shareC_C TEXT,";
      sqlCreate << "shareA_R TEXT,";
      sqlCreate << "shareB_R TEXT,";
      sqlCreate << "shareC_R TEXT,";
      sqlCreate << "share_r TEXT";

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

      a_a_shares.reserve(cacheSize);
      b_a_shares.reserve(cacheSize);
      c_a_shares.reserve(cacheSize);

      a_b_shares.reserve(cacheSize);
      b_b_shares.reserve(cacheSize);
      c_b_shares.reserve(cacheSize);

      a_c_shares.reserve(cacheSize);
      b_c_shares.reserve(cacheSize);
      c_c_shares.reserve(cacheSize);

      a_r_shares.reserve(cacheSize);
      b_r_shares.reserve(cacheSize);
      c_r_shares.reserve(cacheSize);

      r_shares.reserve(cacheSize);
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
        return;
      case 2:
        c_shares.push_back(tmpString);
        tripleCounter++;
        return;
      case 3:
        a_a_shares.push_back(tmpString);
        tripleCounter++;
        return;
      case 4:
        b_a_shares.push_back(tmpString);
        tripleCounter++;
        return;
      case 5:
        c_a_shares.push_back(tmpString);
        tripleCounter++;
        return;
      case 6:
        a_b_shares.push_back(tmpString);
        tripleCounter++;
        return;
      case 7:
        b_b_shares.push_back(tmpString);
        tripleCounter++;
        return;
      case 8:
        c_b_shares.push_back(tmpString);
        tripleCounter++;
        return;
      case 9:
        a_c_shares.push_back(tmpString);
        tripleCounter++;
        return;
      case 10:
        b_c_shares.push_back(tmpString);
        tripleCounter++;
        return;
      case 11:
        c_c_shares.push_back(tmpString);
        tripleCounter++;
        return;
      case 12:
        a_r_shares.push_back(tmpString);
        tripleCounter++;
        return;
      case 13:
        b_r_shares.push_back(tmpString);
        tripleCounter++;
        return;
      case 14:
        c_r_shares.push_back(tmpString);
        tripleCounter++;
        return;
      case 15:
        r_shares.push_back(tmpString);
        tripleCounter++;
        return ;
      case 16:
        tripleCounter++;
        return ;
      case 17:
        tripleCounter++;
        //no return here as we want to prepare the sql statement if we have all 3 shares
    }
  sqlCounter++;
  int rc;
  rc = sqlite3_bind_text(pStmt, 1, a_shares.back().c_str(), a_shares.back().size(), SQLITE_STATIC);
  if (rc != SQLITE_OK) {
      cout << "Bind failed 1: " << sqlite3_errmsg(db) << endl;
    }
  rc = sqlite3_bind_text(pStmt, 2, b_shares.back().c_str(), b_shares.back().size(), SQLITE_STATIC);
  if (rc != SQLITE_OK) {
      cout << "Bind failed 2: " << sqlite3_errmsg(db) << endl;
    }
  rc = sqlite3_bind_text(pStmt, 3, c_shares.back().c_str(), c_shares.back().size(), SQLITE_STATIC);
  if (rc != SQLITE_OK) {
      cout << "Bind failed 3: " << sqlite3_errmsg(db) << endl;
    }

  rc = sqlite3_bind_text(pStmt, 4, a_a_shares.back().c_str(), a_a_shares.back().size(), SQLITE_STATIC);
  if (rc != SQLITE_OK) {
      cout << "Bind failed 4: " << sqlite3_errmsg(db) << endl;
    }
  rc = sqlite3_bind_text(pStmt, 5, b_a_shares.back().c_str(), b_a_shares.back().size(), SQLITE_STATIC);
  if (rc != SQLITE_OK) {
      cout << "Bind failed 5: " << sqlite3_errmsg(db) << endl;
    }
  rc = sqlite3_bind_text(pStmt, 6, c_a_shares.back().c_str(), c_a_shares.back().size(), SQLITE_STATIC);
  if (rc != SQLITE_OK) {
      cout << "Bind failed 6: " << sqlite3_errmsg(db) << endl;
    }

  rc = sqlite3_bind_text(pStmt, 7, a_b_shares.back().c_str(), a_b_shares.back().size(), SQLITE_STATIC);
  if (rc != SQLITE_OK) {
      cout << "Bind failed 7: " << sqlite3_errmsg(db) << endl;
    }
  rc = sqlite3_bind_text(pStmt, 8, b_b_shares.back().c_str(), b_b_shares.back().size(), SQLITE_STATIC);
  if (rc != SQLITE_OK) {
      cout << "Bind failed 8: " << sqlite3_errmsg(db) << endl;
    }
  rc = sqlite3_bind_text(pStmt, 9, c_b_shares.back().c_str(), c_b_shares.back().size(), SQLITE_STATIC);
  if (rc != SQLITE_OK) {
      cout << "Bind failed 9: " << sqlite3_errmsg(db) << endl;
    }

  rc = sqlite3_bind_text(pStmt, 10, a_c_shares.back().c_str(), a_c_shares.back().size(), SQLITE_STATIC);
  if (rc != SQLITE_OK) {
      cout << "Bind failed 10: " << sqlite3_errmsg(db) << endl;
    }
  rc = sqlite3_bind_text(pStmt, 11, b_c_shares.back().c_str(), b_c_shares.back().size(), SQLITE_STATIC);
  if (rc != SQLITE_OK) {
      cout << "Bind failed 11: " << sqlite3_errmsg(db) << endl;
    }
  rc = sqlite3_bind_text(pStmt, 12, c_c_shares.back().c_str(), c_c_shares.back().size(), SQLITE_STATIC);
  if (rc != SQLITE_OK) {
      cout << "Bind failed 12: " << sqlite3_errmsg(db) << endl;
    }

  rc = sqlite3_bind_text(pStmt, 13, a_r_shares.back().c_str(), a_r_shares.back().size(), SQLITE_STATIC);
  if (rc != SQLITE_OK) {
      cout << "Bind failed 13: " << sqlite3_errmsg(db) << endl;
    }
  rc = sqlite3_bind_text(pStmt, 14, b_r_shares.back().c_str(), b_r_shares.back().size(), SQLITE_STATIC);
  if (rc != SQLITE_OK) {
      cout << "Bind failed 14: " << sqlite3_errmsg(db) << endl;
    }
  rc = sqlite3_bind_text(pStmt, 15, c_r_shares.back().c_str(), c_r_shares.back().size(), SQLITE_STATIC);
  if (rc != SQLITE_OK) {
      cout << "Bind failed 15: " << sqlite3_errmsg(db) << endl;
    }

  rc = sqlite3_bind_text(pStmt, 16, r_shares.back().c_str(), r_shares.back().size(), SQLITE_STATIC);
  if (rc != SQLITE_OK) {
      cout << "Bind failed 16: " << sqlite3_errmsg(db) << endl;
    }


  rc = sqlite3_step(pStmt);
  if (rc != SQLITE_DONE) {
      cout << "Execution failed: " << sqlite3_errmsg(db) << endl;
    }
  sqlite3_reset(pStmt);
  sqlite3_clear_bindings(pStmt);
  if (sqlCounter == cacheSize) {
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

      a_a_shares.clear();
      b_a_shares.clear();
      c_a_shares.clear();

      a_b_shares.clear();
      b_b_shares.clear();
      c_b_shares.clear();

      a_c_shares.clear();
      b_c_shares.clear();
      c_c_shares.clear();

      a_r_shares.clear();
      b_r_shares.clear();
      c_r_shares.clear();

      r_shares.clear();
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
