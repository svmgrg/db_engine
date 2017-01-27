#ifndef _ddl_op_included_
#define _ddl_op_included_

#include "data_str.hh"
#include "support_fn.hh"
#include <vector>
#include <iomanip>
#include <iostream>

void show_tables (vector <table> list_table);
void print_table (table T);
int create ();
int insert ();

#endif
