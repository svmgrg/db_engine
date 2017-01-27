// 1 -> 1, 2, 5
// 3 -> 3, 4
//
// for a table for grouping by Dept:
//
// S.#  Faculty      Dept
// ---  -------      ----
// 1.   Einstein     Physics
// 2.   Curie        Physics
// 3.   Reddy        Comp. Sci.
// 4.   Minsky       Comp. Sci.
// 5.   Bose         Physics
//
// Now we can form a map between {1:[1, 2, 5]; 3:[3, 4]}

#ifndef _aggregate_included_
#define _aggregate_included_

#include "data_str.hh"
#include "table_fn.hh"
#include <cstdlib>
#include <iostream>
#include <map>
#include <vector>
#include <sstream>

#define SSTR(x) static_cast<ostringstream &>((ostringstream()<<dec<<x)).str()
#define uint unsigned int

int check_duplicate (vector <uint> attri_index, uint tuple, table T);
table aggregate_fn (string list_attri, string list_expr, table T);
int find_attri (string temp, table T);

#endif
