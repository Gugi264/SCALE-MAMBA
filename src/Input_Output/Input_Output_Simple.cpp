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
stringstream share_a;
stringstream share_b;
stringstream share_c;
sqlite3 *db;

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
 // (*outf) << "Output channel " << channel << " : ";
  //S.output(*outf, human);


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

  bigint te;
  gfp share = S.get_share(0);
  to_bigint(te, share, true);
  switch (tripleCounter) {
      case 0:
        share_a << te;
        tripleCounter++;
        return ;
      case 1:
        share_b << te;
        tripleCounter++;
        return ;
      case 2:
        share_c << te;
    }

  stringstream sql;
  char *zErrMsg = 0;
  int rc;
  sql << "INSERT INTO triples (shareA, shareB, shareC)";
  sql << "VALUES ('" << share_a.str() << "', '" << share_b.str() << "', '" << share_c.str() << "' );";
  rc = sqlite3_exec(db, sql.str().c_str(), callback, 0, &zErrMsg);
  if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error while insert: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    }

  tripleCounter = 0;
  stringstream().swap(share_a);
  stringstream().swap(share_b);
  stringstream().swap(share_c);

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
