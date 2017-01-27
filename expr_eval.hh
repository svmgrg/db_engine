#ifndef _expr_eval_included_
#define _expr_eval_included_

#include "data_str.hh"
#include "ra_op.hh"
#include "aggregate.hh"
#include <vector>
#include <cstdlib>
#include <iostream>
#include <sstream>

#define SSTR(x) static_cast<ostringstream &>((ostringstream()<<dec<<x)).str()

using namespace std;

vector < string > shunting_yard_adv (string query);
int prec_adv (char a, char b);
int conv_adv (char a);

vector < string > shunting_yard_nest (string query);
int prec_nest (char a, char b);
int conv_nest (char a);

int eval (vector < string > tuple_eval, table T, int tuple);
int eval_nest (vector < string > table_eval);

#endif
