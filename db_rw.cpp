#include "db_rw.hh"

// for reading table creation commands and tuple insertion
// from previous sessions (stored in the file "db_data.dat")
// into the memory for current sessions
int read_db_dat () {
  struct stat buf;
  if (stat("db_data.dat", &buf) == -1) {
    cout<<"> No prior DB found.\n";
    return -1;
  }
  else
    cout<<"> Prior DB found. Uploading data..."
	<<"\n-------------------------------------";
  fstream db_data;
  db_data.open("db_data.dat");
  string query_1, query_2;
  query_1 = "O_o";
  while (getline (db_data, query_2, ';')) {
    if (query_1 != "O_o")
      parser(query_1);
    query_1 = query_2;
  }
  db_data.close();
  cout<<"-------------------------------------\n"
      <<"> Prior DB is now in memory.\n\n";
       
  return 0;
}

// for writing table creation commands and tuple insertion
// from current sessions (appended to the file "db_data.dat")
// for future sessions
int write_db_dat (string query) {
  fstream db_data;
  db_data.open("db_data.dat", ios::app);
  query += ";\n";
  db_data<<query;
  db_data.close();
  return 0;
}

