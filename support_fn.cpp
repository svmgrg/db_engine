#include "support_fn.hh"

#define uint unsigned int

vector < table > list_table;
vector < string > broken_query;

// call appropriate functions to perform relational
// algebra operations
int ra(string query) {
  vector < string > table_eval;
  
  // char tmp = query[0];
  // if (tmp == '\n' || t)
  //   tmp = query[1];
  // if (!(tmp == '@' || tmp == '#' || tmp == '`'
  // 	|| tmp == '^' || tmp == ':' || tmp == '%'
  // 	|| tmp == '{'))
  //   return -1;
  
  table_eval = shunting_yard_nest (query);
  // for (int i = 0; i < table_eval.size(); i++) 
  //   cout<<table_eval[i]<<endl; 
  eval_nest(table_eval);
  string temp = table_eval[0].substr(1, table_eval[0].length()-1);
  int index = ::atof(temp.c_str());
  print_table(dollar[index]);
  dollar.clear();
  return 0;
}

// parse the query, decide whether it's simple exit, create, insert
// or a relational algebra expression
// then take action accordingly
int parser (string query) {
  split(query);
  if (broken_query[0] == "EXIT") {
    cout<<"\n> PROGRAM EXITING NOW...\n";
    // print_table(list_table[0]);
    exit(0);
  }
  else if (broken_query[0] == "CREATE") {
    if (create() == 0)
      return 1; // indicate query was of CREATE and worked correctly
  }
  else if (broken_query[0] == "INSERT") {
    if (insert() == 0)
      return 2; // indicate query was of INSERT and worked correctly
  }
  else if (broken_query[0] == "SHOW_TABLES")
    show_tables(list_table);
  else 
    ra(query);
  return 0;
}

// split a string into a vector of strings
// split occurs at every space
// eg. "A B C" converts to "A"->"B"->"C"
int split (string query) {
  broken_query.clear();
  istringstream iss(query);
  do {
    string sub;
    iss >> sub;
    broken_query.push_back(sub);
    } while (iss);
  broken_query.pop_back();
  return 0;
}

// check if a string is entirely composed of numbers
// eg. "2.141" -> True
//     "35346" -> True
//     "sgs"   -> False
//     "fgd45" -> False
bool is_num (string str) {
  if (str.length() == 0)
    return false;
  uint i = 0;
  if (str[i] == '-')
    i++;
  for (; i < str.length() && str[i] != '.'; i++)
    if (!(str[i] >= '0' && str[i] <= '9'))
      return false;
  i++;
  for (; i < str.length(); i++)
    if (!(str[i] >= '0' && str[i] <= '9'))
      return false;
  return true;
}
