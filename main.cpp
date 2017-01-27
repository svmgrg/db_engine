#include "support_fn.hh"
#include "db_rw.hh"
#include <iostream>

#define uint unsigned int

using namespace std;

int main () {
  cout<<endl;
  cout<<"***************************************************************\n"
      <<"*                           DB_ENGINE                         *\n"
      <<"*                           ---------                         *\n"
      <<"*                          Shivam Garg                        *\n"
      <<"*                           14074017                          *\n"
      <<"*	         (shivam.gargcd.cse14@itbhu.ac.in)            *\n"
      <<"***************************************************************\n";
  cout<<"                   PROGRAM SUCCESSFULLY STARTED.\n\n";
  // create prior DB from db_backup
  read_db_dat();

  // main program loop
  cout<<"> YOUR QUERIES HERE: ";
  while (1) {
    cout<<"\n>>> ";
    string query;
    cin.clear();
    fflush(stdin);
    query.clear();
    getline (cin, query, ';');
    
    // query was successful write it to db_backup
    int type = parser(query);
    if (type == 1 || type == 2)
      write_db_dat(query);
  }
}

 

