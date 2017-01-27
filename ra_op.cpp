#include "ra_op.hh"

#define uint unsigned int

// for selecting specific tuples that
// correspond to the query
// @[query](table)
table select (string cond, table T) {
  if (T.columns.size() == 0)
    return T;

  vector < string > tuple_eval;
  tuple_eval = shunting_yard_adv(cond);

  // for each tuple in table T
  for (uint tuple = 0; tuple < T.columns[0].values.size(); tuple++) {

    // evaluate the conditions
    int cond_eval = eval(tuple_eval, T, tuple);

    // remove the tuple if condition evaluated to false
    if (cond_eval == 0) {
      for (uint x = 0; x < T.columns.size(); x++) {
    	T.columns[x].values.erase(T.columns[x].values.begin()+tuple);
      }
      tuple--;
    }
    
  }
  return T;
}

// for projecting specified columns of the table
// #[list_col](table)
table project (string query, table T) {
  uint i = 0;
  table new_table;
  new_table.name = T.name;
  while (i < query.length()) {
    string col_name;
    for (; i < query.length() && query[i] != ','; i++)
      col_name += query[i];
    // cout<<"COL:  |"<<col_name<<"|\n";
    i++;
    for (uint x = 0; x < T.columns.size(); x++) {
      if (T.columns[x].name == col_name) {
	new_table.columns.push_back(T.columns[x]);
	break;
      }
    }
  }
  return new_table;
}

// rename the table
// `[new_table_name, list_col](table)
table rename (string query, table T) {
  uint i;
  T.name.clear();
  for (i = 0; i < query.length() && query[i] != ','; i++)
    T.name += query[i];
  i++;
  for (uint x = 0; x < T.columns.size(); x++) {
    T.columns[x].name.clear();
    for (; i < query.length() && query[i] != ','; i++)
      T.columns[x].name += query[i];
    i++;
  }
  return T;
}

// for taking union of two tables
// (table1 ^ table2)
int union_table (int left, int right) {
  table T1 = dollar[left];
  table T2 = dollar[right];
  
  if (union_compatibility (T1, T2) == false)
    return -1;

  // add all the tuples of T1 to T
  table T = T1;
  
  // add those tuples of T2 which are non-existant in T
  // for each tuple in T2
  if (T2.columns.size() > 0)
    for (uint x = 0; x < T2.columns[0].values.size(); x++) {

      // to check for duplicate tuples
      bool flag;

      // for each tuple in T1
      bool flag_add = true;
      if (T1.columns.size() > 0) // # of tuples is greater than zero
	for (uint q = 0; q < T1.columns[0].values.size(); q++) {
	  // for each attribute of every tuple
	  flag = true;
	  for (uint y = 0; y < T1.columns.size(); y++)
	    if (T1.columns[y].values[q] != T2.columns[y].values[x]) {
	      flag = false;
	      break;
	    }
	  // if this is a duplicate tuple
	  if (flag == true) {
	    flag_add = false;
	    break;
	  }
	}
    
      // if no duplicate is found add the tuple to T
      if (flag_add == true) 
	// for each attribute of current tuple of T2
	for (uint y = 0; y < T1.columns.size(); y++)
	  T.columns[y].values.push_back(T2.columns[y].values[x]);
    }

  dollar[left] = T;
  return 0;
}

// for taking set-difference of two tables
// (table1 : table2)
int set_diff (int left, int right) {
  table T1 = dollar[left];
  table T2 = dollar[right];
  
  if (union_compatibility (T1, T2) == false)
    return -1;

  // add all the tuples of T1 to T
  table T = T1;
  
  // remove those tuples of T1 which also exist in T2
  // for each tuple in T
  if (T.columns.size() > 0)
    for (uint x = 0; x < T.columns[0].values.size(); x++) {

      // to check for duplicate tuples
      bool flag;

      // for each tuple in T2
      bool flag_add = true;
      if (T2.columns.size() > 0) // # of tuples is greater than zero
	for (uint q = 0; q < T2.columns[0].values.size(); q++) {
	  // for each attribute of every tuple
	  flag = true;
	  for (uint y = 0; y < T.columns.size(); y++)
	    if (T2.columns[y].values[q] != T.columns[y].values[x]) {
	      flag = false;
	      break;
	    }
	  // if this is a duplicate tuple
	  if (flag == true) {
	    flag_add = false;
	    break;
	  }
	}
    
      // if current tuple is a duplicate, remove it from T
      if (flag_add == false) {
	// for each attribute of current tuple of T
	for (uint y = 0; y < T.columns.size(); y++)
	  T.columns[y].values.erase(T.columns[y].values.begin()+x);
	x--;
      }
      
    }

  dollar[left] = T;
  return 0;
}

// for taking cartesian product of two tables
// (table1 % table2)
int cartesian (int left, int right) {
  table T1 = dollar[left];
  table T2 = dollar[right];
  table T;
  for (uint x = 0; x < T1.columns.size(); x++) {
    attribute A;
    A.name = T1.name+"."+T1.columns[x].name;
    A.type = T1.columns[x].type;
    T.columns.push_back(A);
  }
  
  for (uint x = 0; x < T2.columns.size(); x++) {
    attribute A;
    A.name = T2.name+"."+T2.columns[x].name;
    A.type = T2.columns[x].type;
    T.columns.push_back(A);
  }
  
  for (uint x = 0; x < T1.columns[0].values.size(); x++) {
      for (uint y = 0; y < T2.columns[0].values.size(); y++) {

	uint z, t;
	for (z = 0; z < T1.columns.size(); z++) 
	  T.columns[z].values.push_back(T1.columns[z].values[x]);
	for (t = 0; t < T2.columns.size(); t++)
	   T.columns[z+t].values.push_back(T2.columns[t].values[y]);

      }
  }
  dollar[left] = T;
  return 0;
}

// check two tables for union compatibility
// ie. check if their corresponding column names
// and data type are equal
bool union_compatibility (table A, table B) {
  if (A.columns.size() != B.columns.size())
    return false;
  for (uint i = 0; i < A.columns.size(); i++) {
    if (A.columns[i].name != B.columns[i].name
	|| A.columns[i].type != B.columns[i].type
	) {
      return false;
    }
  }
  return true;
}
