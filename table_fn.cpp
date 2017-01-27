#include "table_fn.hh"

#define uint unsigned int

// displays the list of tables stored in the database
void show_tables (vector <table> list_table) {
  if (list_table.size() == 0)
   cout<<"\nNO TABLES ARE CURRENTLY ADDED TO THE DATABASE.\n";
  else {
    cout<<"\nTABLES IN THE DATABASE:\n";
    for (uint i = 0; i < list_table.size(); i++)
      cout<<i+1<<". "<<list_table[i].name<<"\n";
  }
}

// prints the table T in a nice and easy to see format
void print_table (table T) {
  int width = 15;
  cout<<"\n\nTABLE: "<<T.name<<"\n+";
  for (uint i = 0; i < T.columns.size(); i++) 
    cout<<"-----------------+";
  cout<<"\n| ";
  for (uint i = 0; i < T.columns.size(); i++) 
    cout<<setw(width)<<T.columns[i].name<<" | ";
  cout<<"\n+";
  for (uint i = 0; i < T.columns.size(); i++) 
    cout<<"-----------------+";
  cout<<"\n+";
   for (uint i = 0; i < T.columns.size(); i++) 
    cout<<"-----------------+";
  cout<<"\n| ";
  for (uint j = 0; j < T.columns[0].values.size(); j++) {
    for (uint i = 0; i < T.columns.size(); i++) 
      cout<<setw(width)<<T.columns[i].values[j]<<" | ";
    cout<<"\n+";
    for (uint i = 0; i < T.columns.size(); i++) 
    cout<<"-----------------+";
    cout<<"\n| ";
  }
}

// create a table in the database
int create() {

  // SYNTAX:
  //  CREATE <table name>
  //    <data type> <attribute1 name>
  //    <data type> <attribute2 name>
  //    ...  
  
  // checking for repetitive table name
  for (uint j = 0;
       j < list_table.size();
       j++)
    if (broken_query[1] ==
	list_table[j].name) {
      cout<<"\n  > ERROR: TABLE NAME \""<<broken_query[1]
	  <<"\" HAS ALREADY BEEN CREATED!\n"
	  <<"    -----\n"; 
      return -1;
    }
  
  // push table in list_table
  table temp;
  temp.name = broken_query[1];
  list_table.push_back(temp);
  for (uint i = 2; i < broken_query.size(); i = i+2) {
    // push attributes in columns of the table
    attribute temp;
    temp.type = broken_query[i];

    // checking for repetitive attribute name
    for (uint j = 0;
	 j < list_table[list_table.size()-1].columns.size();
	 j++)
      if (broken_query[i+1] ==
	  list_table[list_table.size()-1].columns[j].name) {
	list_table.pop_back();
	cout<<"\n  > ERROR: ATTRIBUTE NAME \""<<broken_query[i+1]
	    <<"\" HAS ALREADY BEEN CREATED! ABORTING TABLE CREATION.\n"
	    <<"    -----\n"; 
	return -1;
      }
    
    temp.name = broken_query[i+1];
    list_table[list_table.size()-1].columns.push_back(temp);
  }
  cout<<"\n  > TABLE \""<<broken_query[1]<<"\" HAS BEEN CREATED!\n";
  return 0;
}

// insert records into an existing table
int insert() {
  
  // SYNTAX:
  //  INSERT <table name>
  //    <value11> <value12> <value13> ...
  //    <value11> <value12> <value13> ...
  //    ...

  // check whether any table exists
  if (list_table.size() == 0) {
    cout<<"\n  > TABLE \""<<broken_query[1]<<"\" DOES NOT EXIST!\n";
    return -1;
  }

  // find index of table
  int table_index = -1;
  for (uint x = 0; x < list_table.size(); x++) {
    // cout<<"|"<<list_table[x].name<<"|\n";
    if (list_table[x].name == broken_query[1]) {
      // cout<<"T "<<list_table[x].name<<" "<<broken_query[1]<<"\n";
      table_index = x;
      break;
    }
  }

  if (table_index == -1) {
    cout<<"\n  > TABLE \""<<broken_query[1]<<"\" DOES NOT EXIST!\n";
    return -1;
  } 
 
  uint no_attributes = list_table[table_index].columns.size();
  uint no_tuple_add = 0;
  
  // table temp;
  // temp.name = broken_query[1];
  // list_table.push_back(temp);

  for (uint i = 2; i < broken_query.size(); i = i+no_attributes) {

    // to check for duplicate tuples
    bool flag;
    bool flag_add = true;
    // for each tuple
    if (no_attributes > 0) 
      for (uint x = 0; x < list_table[table_index].columns[0].values.size(); x++) {
	// for each record of that tuple
	flag = true;
	for (uint y = 0; y < no_attributes; y++)
	  if (list_table[table_index].columns[y].values[x] != broken_query[i+y]) {
	    flag = false;
	    break;
	  }
	// if this is a duplicate tuple
	if (flag == true) {
	  flag_add = false;
	  break;
	}
      }    
    
    // if no duplicate is found
    if (flag_add == true) {

      // check if insert data is conforming to the type of the correspoding attribute
      flag = true;
      for (uint k = 0; k < no_attributes; k++) {
	// push values in columns of the table
	if (list_table[table_index].columns[k].type == "INT" && is_num(broken_query[i+k]) == false) {
	  flag = false;
	  break;
	}
      }

      // if insert data is conforming to the type of the correspoding attribute add it 
      if (flag == true) {
	no_tuple_add++;
	for (uint k = 0; k < no_attributes; k++) 
	  // push values in columns of the table
	  list_table[table_index].columns[k].values.push_back(broken_query[i+k]);
      }	  
    }
  }
  
  cout<<"\n  > "<<no_tuple_add
      <<" TUPLE(S) ADDED TO TABLE \""
      <<broken_query[1]<<"\"!\n";
  return 0;
}
