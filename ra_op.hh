#ifndef _ra_op_included_
#define _ra_op_included_

#include "data_str.hh"
#include "expr_eval.hh"
#include <vector>

using namespace std;

table select (string query, table T);
table project (string query, table T);
table rename (string query, table T);
int union_table (int left, int right);
int set_diff (int left, int right);
int cartesian (int left, int right);
bool union_compatibility (table A, table B);

#endif
