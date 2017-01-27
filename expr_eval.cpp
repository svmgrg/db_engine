#include "expr_eval.hh"

#define uint unsigned int

vector < table > dollar;
int var_test = false;

// converts infix logic and arithmetic expressions into reverse polish notation
// used for select queries
// @[<queries>](table)
vector < string > shunting_yard_adv (string cond) {
  vector < string > output;
  vector < char > stack;
  string temp;
  for (uint i = 0; i < cond.size(); i++) {
    if (cond[i] == '&' || cond[i] == '|' || cond[i] == '!'
	|| cond[i] == '*' || cond[i] == '/' || cond[i] == '+'
	|| cond[i] == '-' || cond[i] == '>' || cond[i] == '<'
	|| cond[i] == '=' || cond[i] == '~') {
      if (temp.length() > 0) {
	output.push_back(temp);
	temp.clear();
      }
      while (stack.size() > 0 &&
	     prec_adv(cond[i], stack[stack.size()-1]) < 0) {
	output.push_back(string(1, stack[stack.size()-1]));
	stack.pop_back();
      }
      stack.push_back(cond[i]);
    }
    else if (cond[i] == '(') {
      if (temp.length() > 0) {
	output.push_back(temp);
	temp.clear();
      }
      stack.push_back(cond[i]);
    }
    else if (cond[i] == ')') {
      if (temp.length() > 0) {
	output.push_back(temp);
	temp.clear();
      }
      while (stack.size() > 0 && stack[stack.size()-1] != '(') {
	output.push_back(string(1, stack[stack.size()-1]));
	stack.pop_back();
      }
      stack.pop_back();
    }
    else
      if (cond[i] != ' ')
	temp += cond[i];
  }
  if (temp.length() > 0) {
    output.push_back(temp);
    temp.clear();
  }
  while (stack.size() > 0) {
    output.push_back(string(1, stack[stack.size()-1]));
    stack.pop_back();
  }
  return output;
}

// for determining the precedence of operators
int prec_adv (char a, char b) {
  a = conv_adv(a);
  b = conv_adv(b);
  return a-b;
}

// for determining the precedence of operators
// higher return values correspond to higher
// precedence. eg: * > +; ! > & > |
int conv_adv (char a) {
  if (a == '/')
    return 12;
  else if (a == '*')
    return 11;
  else if (a == '+')
    return 10;
  else if (a == '-')
    return 9;
  else if (a == '~')
    return 8;
  else if (a == '=')
    return 7;
  else if (a == '>')
    return 6;
  else if (a == '<')
    return 5;
  else if (a == '!')
    return 3;
  else if (a == '&')
    return 2;
  else if (a == '|')
    return 1;
  else
    return 0;
}

// converts the nested into reverse polish notation
// eg: converts
// --------------------------------------
// #[list_col](@[query](table1 % table2))
// --------------------------------------
//      |
//      v
// ------------
// table1
// table2
// %
// @[query]
// #[list_col]
// ------------
// this reverse polish is then evaluated by the function
// eval_nest (see below)
vector < string > shunting_yard_nest (string cond) {
  vector < string > output;
  vector < string > stack;
  string temp, temp2;
  for (uint i = 0; i < cond.size(); i++) {
    
    if (cond[i] == '#' || cond[i] == '@' || cond[i] == '`'
	|| cond[i] == ':' || cond[i] == '^' || cond[i] == '%'
	|| cond[i] == '{') {

      if (cond[i] == '#' || cond[i] == '@' || cond[i] == '`') {
	while (cond[i] != ']') {
	  if (cond[i] != ' ')
	    temp2 += cond[i];
	  i++;
	}
	temp2 += cond[i];
      }
      else if (cond[i] == '{') {
	while (!(cond[i] == '}' && cond[i+1] == '(')) {
	  // {G1, G2, G3, ...}*{g(1), g(2), ...}(Table)
	  // the point till                    ^ 
	  if (cond[i] != ' ')
	    temp2 += cond[i];
	  i++;
	}
	temp2 += cond[i];
      }
      else
	temp2 += cond[i];
     
      if (temp.length() > 0) {
	output.push_back(temp);
	temp.clear();
      }

      while (stack.size() > 0 &&
	     prec_nest(temp2[0], stack[stack.size()-1][0]) < 0) {
	output.push_back(stack[stack.size()-1]);
	stack.pop_back();
      }
      stack.push_back(temp2);
      temp2.clear();
    }
    else if (cond[i] == '(') {
      if (temp.length() > 0) {
	output.push_back(temp);
	temp.clear();
      }
      temp2 += cond[i];
      stack.push_back(temp2);
      temp2.clear();
    }

    // There are two kinds of parenthesis in this shunting yard string.
    // One is used for a fn: fn() and other is used to group things:
    // ( (A and G) and (W or P) ).
    // In the first case when a ')' is encountered fn. should also be
    // popped from the shunting along with the arguments of the function.
    // However if the matching '(' does not follow a fn. then preceding
    // input on the shunting should not be popped.
    // Note: fn () is possible.
    // 	    So is ( ( ).
    // 	    But ) ( ) should not be possible.
    
    else if (cond[i] == ')') {
      if (temp.length() > 0) {
	output.push_back(temp);
	temp.clear();
      }
      while (stack.size() > 0 && stack[stack.size()-1] != "(") {
	output.push_back(stack[stack.size()-1]);
	stack.pop_back();
      }
      stack.pop_back();
      if (stack.size() > 0 && stack[stack.size()-1][0] != '(') {      
	output.push_back(stack[stack.size()-1]);
	stack.pop_back();
      }
    }
    else
      if (cond[i] != ' ')
	temp += cond[i];
  }
  if (temp.length() > 0) {
    output.push_back(temp);
    temp.clear();
  }
  while (stack.size() > 0) {
    output.push_back(stack[stack.size()-1]);
    stack.pop_back();
  }

  if (var_test == true) {
    cout<<"________________________________________________________\n"
	<<"SHUNTING YARD NEST REVERSE POLISH:\n"
	<<"--------------------------------------------------------\n";
    vector <string> A = output;
    for (uint i = 0; i < A.size(); i++) {
      if (A[i] == "\n") {
	A.erase(A.begin()+i);
	i--;
      }
    }
    for (uint i = 0; i < A.size(); i++) {
      cout<<"|"<<A[i]<<"| "<<"\n";
    }
    cout<<"________________________________________________________\n";
  }

  for (uint i = 0; i < output.size(); i++) {
    if (output[i] == "\n") {
      output.erase(output.begin()+i);
      i--;
    }
  }
  return output;
}

int prec_nest (char a, char b) {
  a = conv_nest(a);
  b = conv_nest(b);
  return a-b;
}

int conv_nest (char a) {
  if (a == '`')
    return 13;
  else if (a == '*')
    return 12;
  else if (a == '%')
    return 11;
  else if (a == '@')
    return 10;
  else if (a == '#')
    return 9;
  else if (a == '^')
    return 8;
  else if (a == ':')
    return 7;
  else
    return 0;
}

// evaluates the reverse polish generated by shunting_yard_adv
// and return 1 (for True) if the query evaluates to true,
// otherwise 0 (for False)
int eval (vector < string > tuple_eval, table T, int tuple) {
  // evaluate the conditions Part-2
  for (uint x = 0; x < tuple_eval.size(); x++) {
    if (tuple_eval[x] == "!") {
      tuple_eval[x] = "T";
      if (tuple_eval[x-1] == "T")
	tuple_eval[x] = "F";
      tuple_eval.erase(tuple_eval.begin()+x-1);
      x--;
    }
    else if (tuple_eval[x] == "&") {
      tuple_eval[x] = "F";
      if (tuple_eval[x-1] == "T" && tuple_eval[x-2] == "T")
	tuple_eval[x] = "T";
      tuple_eval.erase(tuple_eval.begin()+x-1);
      tuple_eval.erase(tuple_eval.begin()+x-2);
      x = x-2;
    }
    else if (tuple_eval[x] == ":") {
      tuple_eval[x] = "F";
      if (tuple_eval[x-1] == "T" || tuple_eval[x-2] == "T")
	tuple_eval[x] = "T";
      tuple_eval.erase(tuple_eval.begin()+x-1);
      tuple_eval.erase(tuple_eval.begin()+x-2);
      x = x-2;
    }
    else if (tuple_eval[x] == ">" || tuple_eval[x] == "<" ||
	     tuple_eval[x] == "=" || tuple_eval[x] == "~") {
	
      string lhs, rhs;

      // get string values of lhs
      if (tuple_eval[x-2][0] == '\'') {
        lhs = tuple_eval[x-2].substr(1, tuple_eval[x-2].length()-2);
	// cout<<"LHS :"<<lhs<<"\n";
      }
      else {
	for (uint y = 0; y < T.columns.size(); y++)
	  if (T.columns[y].name == tuple_eval[x-2])
	    lhs = T.columns[y].values[tuple];
	// cout<<"LHS :"<<lhs<<"\n";
      }

      // get string values of rhs
      if (tuple_eval[x-1][0] == '\'') {
        rhs = tuple_eval[x-1].substr(1, tuple_eval[x-1].length()-2);
	// cout<<"RHS :"<<rhs<<"\n";
      }
      else {
	for (uint y = 0; y < T.columns.size(); y++)
	  if (T.columns[y].name == tuple_eval[x-1])
	    rhs = T.columns[y].values[tuple];
	// cout<<"RHS :"<<rhs<<"\n";
      }

      string val = "F";
      if (tuple_eval[x] == ">" && lhs > rhs)
	val = "T";
      else if (tuple_eval[x] == "<" && lhs < rhs)
	val = "T";
      else if (tuple_eval[x] == "=" && lhs == rhs)
	val = "T";
      else if (tuple_eval[x] == "~" && lhs != rhs)
	val = "T";
      tuple_eval[x] = val;
      tuple_eval.erase(tuple_eval.begin()+x-1);
      tuple_eval.erase(tuple_eval.begin()+x-2);
      x = x-2;
    }
    else if (tuple_eval[x] == "/" || tuple_eval[x] == "*" ||
	     tuple_eval[x] == "+" || tuple_eval[x] == "-") {
	
      string lhs, rhs;

      // get string values of lhs
      if (tuple_eval[x-1][0] == '\'') 
        lhs = tuple_eval[x-1].substr(1, tuple_eval[x-1].length()-2);
      else 
	for (uint y = 0; y < T.columns.size(); y++)
	  if (T.columns[y].name == tuple_eval[x-1])
	    lhs = T.columns[y].values[tuple];

      // get string values of rhs
      if (tuple_eval[x-2][0] == '\'') 
        rhs = tuple_eval[x-2].substr(1, tuple_eval[x-2].length()-2);
      else 
	for (uint y = 0; y < T.columns.size(); y++)
	  if (T.columns[y].name == tuple_eval[x-2])
	    lhs = T.columns[y].values[tuple];

      float val;
      float lhs_int, rhs_int;
      lhs_int = ::atof(lhs.c_str());
      rhs_int = ::atof(rhs.c_str());
      if (tuple_eval[x] == "/")
	val = lhs_int/rhs_int;
      else if (tuple_eval[x] == "*")
	val = lhs_int*rhs_int;
      else if (tuple_eval[x] == "+")
	val = lhs_int+rhs_int;
      else if (tuple_eval[x] == "-")
	val = lhs_int-rhs_int;
      
      tuple_eval[x] = '\''+SSTR(val)+'\'';
      tuple_eval.erase(tuple_eval.begin()+x-1);
      tuple_eval.erase(tuple_eval.begin()+x-2);
      x = x-2;
    }
   
    // for (int z = 0; z < tuple_eval.size(); z++)
    //   cout<<tuple_eval[z]<<" ";
    // cout<<endl;
  }
  if (tuple_eval[0] == "T")
    return 1;
  else if (tuple_eval[0] == "F")
    return 0;
  else
    return -1;
}

// evaluates the reverse polish generated by shunting_yard_nest
// it calls the appropriate functions on tables generated
// by previous queries
int eval_nest (vector < string > table_eval) {
   
  for (uint i = 0; i < table_eval.size(); i++) {
    
    if (table_eval[i][0] == '#' ||  table_eval[i][0] == '@'
	|| table_eval[i][0] == '`') {

      string temp, temp2;
      int index;

      // extracting conditions of the operation
      for (uint x = 2;
	   x < table_eval[i].size() && table_eval[i][x] != ']';
	   x++)
	temp += table_eval[i][x];

      // extracting the table on which the operator would function
      temp2 =  table_eval[i-1].substr(1, table_eval[i-1].length()-1);
      index = ::atof(temp2.c_str());

      // calling the specified operator
      if (table_eval[i][0] == '#')
	dollar[index] = project(temp, dollar[index]);
      else if (table_eval[i][0] == '@')
	dollar[index] = select(temp, dollar[index]);
      else if (table_eval[i][0] == '`')
	dollar[index] = rename(temp, dollar[index]);

      // removing the operator from stack table_eval
      table_eval.erase(table_eval.begin()+i);
      i--;
    }
    else if (table_eval[i][0] == '{') {
      // Aggregate function

      string temp, temp2, temp3;
      int index;

      // extracting conditions of the operation
      uint x;
      for (x = 0;
	   x < table_eval[i].size() && table_eval[i][x] != '}';
	   x++)
	temp += table_eval[i][x];
      temp += table_eval[i][x];

      for (x = x+2;
	   x < table_eval[i].size() && table_eval[i][x] != '}';
	   x++)
	temp2 += table_eval[i][x];
      temp2 += table_eval[i][x];
      
      // extracting the table on which the operator would function
      temp3 = table_eval[i-1].substr(1, table_eval[i-1].length()-1);
      index = ::atof(temp2.c_str());

      // calling the specified operator
      dollar[index] = aggregate_fn(temp, temp2, dollar[index]);

      // removing the operator from stack table_eval
      table_eval.erase(table_eval.begin()+i);
      i--;      
    }
    else if ( table_eval[i][0] == ':'
	      || table_eval[i][0] == '^' || table_eval[i][0] == '%'
	      || table_eval[i][0] == '$') {

      // extracting the table on which the operator would function
      string temp;
      int left, right;
      
      // left table 
      temp = table_eval[i-2].substr(1, table_eval[i-2].length()-1);
      left = ::atof(temp.c_str());

      // right table 
      temp = table_eval[i-1].substr(1, table_eval[i-1].length()-1);
      right = ::atof(temp.c_str());

      // apply the appropriate operator
      if (table_eval[i][0] == ':') {
	if (set_diff(left, right) == -1)
	  cout<<"\n  > ERROR: TABLES ARE NOT UNION-COMPATIBLE. SO ONLY FIRST TABLE IS PRINTED:\n"
	      <<"    -----";    
	
      }
      else if (table_eval[i][0] == '^') {
	if (union_table(left, right) == -1)
	  cout<<"\n  > ERROR: TABLES ARE NOT UNION-COMPATIBLE. SO ONLY FIRST TABLE IS PRINTED:\n"
	      <<"    -----"; 
      }
      else if (table_eval[i][0] == '%')
	cartesian(left, right);

      // remove the right table and the operator from the stack table_eval
      // left <- left op right [working of the operator]
      table_eval.erase(table_eval.begin()+i);
      table_eval.erase(table_eval.begin()+i-1);
      i = i-2;      
    }
    else {
      for (uint x = 0; x < list_table.size(); x++)
	if (table_eval[i] == list_table[x].name){
	  table temp = list_table[x];
	  dollar.push_back(temp);
	  table_eval[i] = "$"+SSTR(dollar.size()-1);
	  break;
	}
    }
  }
  return 0;
}
