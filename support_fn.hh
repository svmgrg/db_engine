#ifndef _support_fn_included_
#define _support_fn_included_

#include "ra_op.hh"
#include "table_fn.hh"
#include "data_str.hh" 
#include <vector>
#include <sstream>
#include <iostream>

#define SSTR(x) static_cast<ostringstream &>((ostringstream()<<dec<<x)).str()

using namespace std;

int ra (string query);
int parser (string query);
int split (string query);
bool is_num (string str);

#endif
