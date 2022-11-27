/*
Copyright (c) 2017, The University of Bristol, Senate House, Tyndall Avenue, Bristol, BS8 1TH, United Kingdom.
Copyright (c) 2021, COSIC-KU Leuven, Kasteelpark Arenberg 10, bus 2452, B-3001 Leuven-Heverlee, Belgium.

All rights reserved
        */

#include "Input_Output_Simple.h"
#include "Exceptions/Exceptions.h"
#include <fcntl.h>
#include <sqlite3.h>

int tripleCounter= 0;
int sqlCounter= 0;
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
sqlite3 *db= NULL;
sqlite3_stmt *pStmtTriples;
sqlite3_stmt *pStmtRand;
char *sqlTriples= "INSERT INTO triples (shareA, shareB, shareC, shareA_A, shareB_A, shareC_A, shareA_B, shareB_B, shareC_B, shareA_C, shareB_C, shareC_C) VALUES (?,?,?,?,?,?,?,?,?,?,?,?)";
char *sqlRand= "INSERT INTO randVal (shareA_R, shareB_R, shareC_R, share_r) VALUES (?,?,?,?)";
bool firstRun= true;
const int cacheSize= 10000; // 10.000 = 6.6MB
bool triples_choice= true;

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

static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
  int i;
  for (i= 0; i < argc; i++)
    {
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
  printf("\n");
  return 0;
}

static int callbackCount(void *count, int argc, char **argv, char **azColName) {
  int *c = static_cast<int *>(count);
  *c = atoi(argv[0]);
  return 0;
}

void reserve_vectors()
{
  if (triples_choice)
    {
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
    }
  else
    {
      a_r_shares.reserve(cacheSize);
      b_r_shares.reserve(cacheSize);
      c_r_shares.reserve(cacheSize);

      r_shares.reserve(cacheSize);
    }
}

int push_into_correct_vector(string value, int tripleCount)
{
  if (triples_choice)
    {
      switch (tripleCount)
        {
          case 0:
            a_shares.push_back(value);
            return 1;
          case 1:
            b_shares.push_back(value);
            return 1;
          case 2:
            c_shares.push_back(value);
            return 1;
          case 3:
            a_a_shares.push_back(value);
            return 1;
          case 4:
            b_a_shares.push_back(value);
            return 1;
          case 5:
            c_a_shares.push_back(value);
            return 1;
          case 6:
            a_b_shares.push_back(value);
            return 1;
          case 7:
            b_b_shares.push_back(value);
            return 1;
          case 8:
            c_b_shares.push_back(value);
            return 1;
          case 9:
            a_c_shares.push_back(value);
            return 1;
          case 10:
            b_c_shares.push_back(value);
            return 1;
          case 11:
            c_c_shares.push_back(value);
            return 0;
        }
    }
  else
    {
      switch (tripleCount)
        {
          case 0:
            a_r_shares.push_back(value);
            return 1;
          case 1:
            b_r_shares.push_back(value);
            return 1;
          case 2:
            c_r_shares.push_back(value);
            return 1;
          case 3:
            r_shares.push_back(value);
            return 1;
          case 4:
            return 1;
          case 5:
            return 0;
        }
    }
}

void bind_values()
{
  if (triples_choice)
    {
      int rc;
      rc= sqlite3_bind_text(pStmtTriples, 1, a_shares.back().c_str(), a_shares.back().size(), SQLITE_STATIC);
      if (rc != SQLITE_OK)
        {
          cout << "Bind failed 1: " << sqlite3_errmsg(db) << endl;
        }
      rc= sqlite3_bind_text(pStmtTriples, 2, b_shares.back().c_str(), b_shares.back().size(), SQLITE_STATIC);
      if (rc != SQLITE_OK)
        {
          cout << "Bind failed 2: " << sqlite3_errmsg(db) << endl;
        }
      rc= sqlite3_bind_text(pStmtTriples, 3, c_shares.back().c_str(), c_shares.back().size(), SQLITE_STATIC);
      if (rc != SQLITE_OK)
        {
          cout << "Bind failed 3: " << sqlite3_errmsg(db) << endl;
        }

      rc= sqlite3_bind_text(pStmtTriples, 4, a_a_shares.back().c_str(), a_a_shares.back().size(), SQLITE_STATIC);
      if (rc != SQLITE_OK)
        {
          cout << "Bind failed 4: " << sqlite3_errmsg(db) << endl;
        }
      rc= sqlite3_bind_text(pStmtTriples, 5, b_a_shares.back().c_str(), b_a_shares.back().size(), SQLITE_STATIC);
      if (rc != SQLITE_OK)
        {
          cout << "Bind failed 5: " << sqlite3_errmsg(db) << endl;
        }
      rc= sqlite3_bind_text(pStmtTriples, 6, c_a_shares.back().c_str(), c_a_shares.back().size(), SQLITE_STATIC);
      if (rc != SQLITE_OK)
        {
          cout << "Bind failed 6: " << sqlite3_errmsg(db) << endl;
        }

      rc= sqlite3_bind_text(pStmtTriples, 7, a_b_shares.back().c_str(), a_b_shares.back().size(), SQLITE_STATIC);
      if (rc != SQLITE_OK)
        {
          cout << "Bind failed 7: " << sqlite3_errmsg(db) << endl;
        }
      rc= sqlite3_bind_text(pStmtTriples, 8, b_b_shares.back().c_str(), b_b_shares.back().size(), SQLITE_STATIC);
      if (rc != SQLITE_OK)
        {
          cout << "Bind failed 8: " << sqlite3_errmsg(db) << endl;
        }
      rc= sqlite3_bind_text(pStmtTriples, 9, c_b_shares.back().c_str(), c_b_shares.back().size(), SQLITE_STATIC);
      if (rc != SQLITE_OK)
        {
          cout << "Bind failed 9: " << sqlite3_errmsg(db) << endl;
        }

      rc= sqlite3_bind_text(pStmtTriples, 10, a_c_shares.back().c_str(), a_c_shares.back().size(), SQLITE_STATIC);
      if (rc != SQLITE_OK)
        {
          cout << "Bind failed 10: " << sqlite3_errmsg(db) << endl;
        }
      rc= sqlite3_bind_text(pStmtTriples, 11, b_c_shares.back().c_str(), b_c_shares.back().size(), SQLITE_STATIC);
      if (rc != SQLITE_OK)
        {
          cout << "Bind failed 11: " << sqlite3_errmsg(db) << endl;
        }
      rc= sqlite3_bind_text(pStmtTriples, 12, c_c_shares.back().c_str(), c_c_shares.back().size(), SQLITE_STATIC);
      if (rc != SQLITE_OK)
        {
          cout << "Bind failed 12: " << sqlite3_errmsg(db) << endl;
        }

      rc= sqlite3_step(pStmtTriples);
      if (rc != SQLITE_DONE)
        {
          cout << "Execution failed: " << sqlite3_errmsg(db) << endl;
        }
      sqlite3_reset(pStmtTriples);
      sqlite3_clear_bindings(pStmtTriples);
    }
  else
    {
      int rc;
      rc= sqlite3_bind_text(pStmtRand, 1, a_r_shares.back().c_str(), a_r_shares.back().size(), SQLITE_STATIC);
      if (rc != SQLITE_OK)
        {
          cout << "Bind failed 13: " << sqlite3_errmsg(db) << endl;
        }
      rc= sqlite3_bind_text(pStmtRand, 2, b_r_shares.back().c_str(), b_r_shares.back().size(), SQLITE_STATIC);
      if (rc != SQLITE_OK)
        {
          cout << "Bind failed 14: " << sqlite3_errmsg(db) << endl;
        }
      rc= sqlite3_bind_text(pStmtRand, 3, c_r_shares.back().c_str(), c_r_shares.back().size(), SQLITE_STATIC);
      if (rc != SQLITE_OK)
        {
          cout << "Bind failed 15: " << sqlite3_errmsg(db) << endl;
        }

      rc= sqlite3_bind_text(pStmtRand, 4, r_shares.back().c_str(), r_shares.back().size(), SQLITE_STATIC);
      if (rc != SQLITE_OK)
        {
          cout << "Bind failed 16: " << sqlite3_errmsg(db) << endl;
        }
      rc= sqlite3_step(pStmtRand);
      if (rc != SQLITE_DONE)
        {
          cout << "Execution failed: " << sqlite3_errmsg(db) << endl;
        }
      sqlite3_reset(pStmtRand);
      sqlite3_clear_bindings(pStmtRand);
    }
}

void clear_vectors() {
  if (triples_choice) {
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
    } else
    {
      a_r_shares.clear();
      b_r_shares.clear();
      c_r_shares.clear();

      r_shares.clear();
    }


}

void Input_Output_Simple::output_share(const Share &S, unsigned int channel)
{
  //  (*outf) << "Output channel " << channel << " : ";
  //  S.output(*outf, human);


  if (db == NULL)
    {
      char *zErrMsg= 0;
      int rc;
      string fileName= "mydb" + to_string(S.get_player()+1) + ".db";
      (*outf) << " before open" << endl;
      rc= sqlite3_open(fileName.c_str(), &db);
      (*outf) << " after open" << endl;

      if (rc)
        {
          fprintf(stderr, "Cant open database %s\n", sqlite3_errmsg(db));
          return;
        }
      else
        {
          fprintf(stderr, "Opened succesfully\n");
        }

      stringstream sqlCreate;
      sqlCreate << "PRAGMA journal_mode=WAL;";
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
      sqlCreate << "shareC_C TEXT";
      sqlCreate << ");";
      (*outf) << " before first exec" << endl;
      rc= sqlite3_exec(db, sqlCreate.str().c_str(), callback, 0, &zErrMsg);
      if (rc != SQLITE_OK)
        {
          cout << "Error in creating table triples: " << zErrMsg << endl;
          sqlite3_free(zErrMsg);
        }
      else
        {
          fprintf(stdout, "Table triples created successfully\n");
        }

        (*outf) << " after first exec" << endl;
      sqlCreate.clear();
      sqlCreate << "CREATE TABLE if not exists randVal (";
      sqlCreate << "id INTEGER PRIMARY KEY AUTOINCREMENT,";
      sqlCreate << "shareA_R TEXT,";
      sqlCreate << "shareB_R TEXT,";
      sqlCreate << "shareC_R TEXT,";
      sqlCreate << "share_r TEXT";
      sqlCreate << ");";

      rc= sqlite3_exec(db, sqlCreate.str().c_str(), callback, 0, &zErrMsg);
      if (rc != SQLITE_OK)
        {
          cout << "Error in creating table randVal: " << zErrMsg << endl;
          sqlite3_free(zErrMsg);
        }
      else
        {
          fprintf(stdout, "Table randVal created successfully\n");
        }
    }

  if (firstRun)
    {
      int rc= sqlite3_prepare_v2(db, sqlTriples, -1, &pStmtTriples, 0);
      if (rc != SQLITE_OK)
        {
          cout << "Cannot prepare statement for triples: " << sqlite3_errmsg(db) << endl;
          return;
        }
      rc= sqlite3_prepare_v2(db, sqlRand, -1, &pStmtRand, 0);
      if (rc != SQLITE_OK)
        {
          cout << "Cannot prepare statement for randVal: " << sqlite3_errmsg(db) << endl;
          return;
        }
      firstRun= false;
    }

  // true for triples, false for randVal


  if (sqlCounter == 0)
    {
      int triple_rows= nr_rows_in_table("triples");
      int rand_rows= nr_rows_in_table("randVal");
      triples_choice= (triple_rows <= rand_rows);
      sqlite3_exec(db, "BEGIN TRANSACTION;", NULL, NULL, NULL);
      reserve_vectors();
    }

  bigint te;
  gfp share= S.get_share(0);
  to_bigint(te, share, true);
  stringstream tmpstringstream;
  tmpstringstream << te;
  string tmpString= tmpstringstream.str();

  int retVal= push_into_correct_vector(tmpString, tripleCounter);
  if (retVal == 1)
    {
      tripleCounter++;
      return;
    }

  sqlCounter++;
  bind_values();
  if (sqlCounter == cacheSize)
    {
      sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);
      // sqlite3_commit_hook(db, NULL, NULL);
      // rc = sqlite3_finalize(pStmtTriples);
      // cout << "writing to db....." << endl;
      //       if (rc != SQLITE_OK) {
      //           cout << "Finalize failed: " << sqlite3_errmsg(db) << endl;
      //         }
      sqlCounter= 0;
      clear_vectors();
    }
  tripleCounter = 0;
}

int Input_Output_Simple::nr_rows_in_table(string table)
{
  char *zErrMsg= 0;
  int rc;
  int count= 0;
  stringstream ss;
  ss << "select count(*) from " << table;
  auto x= ss.str();
  const char *cstr2= x.c_str();

  rc= sqlite3_exec(db, cstr2, callbackCount, &count, &zErrMsg);
  if (rc != SQLITE_OK)
    {
      cout << "error: " << zErrMsg << endl;
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    }
  //cout << "count from " << table << " is: " << count << endl;
  return count;
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
