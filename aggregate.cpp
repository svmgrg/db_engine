#include "aggregate.hh"
#include <iostream>

using namespace std;

// implements aggregate functions of
// SUM, MAX, MIN, AVG
// CNT: Count of tuples in each group
// CND: Count of distinct tuples in each group
table aggregate_fn (string list_attri, string list_expr, table T) {
  map <int, vector <int> > group;
  string temp;
  vector <uint> attri_index;
  // convert the name of all the attributes to corresponding indices
  for (uint i = 0;  i < list_attri.size(); i++) {
    while (i < list_attri.size() && list_attri[i] != ',' && list_attri[i] != '}') {
      if (list_attri[i] != ' ' && list_attri[i] != '{') 
	temp += list_attri[i];
      i++;
    }

    // check if this attribute exists
    int x = find_attri (temp, T);
   
    // attribute doesn't exist in table T
    if (x == -1)
      return T;
    attri_index.push_back(x);
      
    temp.clear();
  }
  vector <uint> to_remove;
  // for each tuple q of the Table T
  for (uint q = 0; q < T.columns[0].values.size(); q++) {
    int index = check_duplicate (attri_index, q, T);
    if (index != -1) {
      // add q to map of index
      group[index].push_back(q);
      to_remove.push_back(q);
    }
    else {
      vector <int> temp;
      temp.push_back(q);
      group[q] = temp;
    }
  }
  table T_temp = T;
  // remove tuples stored in to_remove from T_temp
  for (uint x = 0; x < to_remove.size(); x++)
    for (uint y = 0; y < T.columns.size(); y++) 
      T_temp.columns[y].values.erase(T_temp.columns[y].values.begin()+to_remove[x]-x);
  
  uint basic_attri = T_temp.columns.size();

  // // for testing purpose
  // print_table(T_temp);
  // cout<<"+++++++++++++++++++++++++++++++GROUP\n";
  // for (map <int, vector <int> >::iterator it = group.begin();
  //      it != group.end(); it++) {
  //   vector <int> temp = it->second;
  //   for (uint i = 0; i < temp.size(); i++)
  //     cout<<temp[i]<<" ";
  //   cout<<endl;
  // }
  
  // Now evaluate expression right of < in {G1, G2, G3, ...}?{g(1), g(2), ...}(Table)
  //                                    ----------------- -----------------
  //                                        list_attri        list_expr
  // AVG, SUM, MIN, MAX, CNT, CND
  uint k = 0; // on which index of list_expr

  // check if list_expr is empty
  bool flag_empty = true;
  for (uint i = 1; i < list_expr.size()-1; i++)
    if (list_expr[i] != ' ' && list_expr[i] != '\n' && list_expr[i] != '\t') {
      flag_empty = false;
      break;
    }
  if (flag_empty == true)
    return T_temp;
  
  for (uint i = 0;  i < list_expr.size(); i++) {
    temp.clear();
    while (i < list_expr.size() && list_expr[i] != ',' && list_expr[i] != '}') {
      if (list_expr[i] != ' ' && list_expr[i] != '{') 
	temp += list_expr[i];
      i++;
    }
    // cout<<"|"<<temp<<"|\n";
    string ag_fn = temp.substr(0, 3);  // type of function
    int a_ind = find_attri (temp.substr(4, temp.size()-5), T_temp); // attri to apply fn
    // cout<<"AG_FN: "<<ag_fn<<" a_ind: "<<a_ind<<"|"<<temp.substr(4, temp.size()-5)<<"|\n";

    if (a_ind == -1)
      return T;
    // Add the column "temp", eg: CND(Dept) to table T_temp
    attribute attri_temp;
    attri_temp.name = temp;
    attri_temp.type = "INT";
    T_temp.columns.push_back(attri_temp);
    

    // start calculating ag_fn for each tuple (group) of T_temp
    map <int, vector <int> >::iterator it = group.begin();
    for (uint p = 0; p < T_temp.columns[0].values.size(); p++, it++) {
      
      vector <int> attri_list = it->second;      
      vector <float> ag_val_f;
      vector <string> ag_val_st;
      
      // add all the attributes of the current group of T_temp to ag_val
      for (uint g = 0; g < attri_list.size(); g++) {
	string temp = T.columns[a_ind].values[attri_list[g]];
	if (ag_fn == "AVG" || ag_fn == "SUM" || ag_fn == "MIN" || ag_fn == "MAX") 
	  ag_val_f.push_back (::atof(temp.c_str()));
	else if (ag_fn == "CNT" || ag_fn == "CND") 
	  ag_val_st.push_back(temp);
      }

      // // testing purpose
      // cout<<p<<": ";
      // for (uint f = 0; f < ag_val_f.size(); f++)
      // 	cout<<ag_val_f[f]<<" ";
      // for (uint f = 0; f < ag_val_st.size(); f++)
      // 	cout<<ag_val_st[f]<<" ";
      
      // apply appropriate ag_fn and add ans to final
      float final = 0;
      if (ag_fn == "AVG" || ag_fn == "SUM") {
	for (uint s = 0; s < ag_val_f.size(); s++)
	  final += ag_val_f[s];
	if (ag_fn == "AVG")
	  final /= ag_val_f.size();
      }
      else if (ag_fn == "MIN") {
	final = ag_val_f[0];
	for (uint s = 1; s < ag_val_f.size(); s++)
	  if (final > ag_val_f[s])
	    final = ag_val_f[s];
      }
      else if (ag_fn == "MAX") {
	final = ag_val_f[0];
	for (uint s = 1; s < ag_val_f.size(); s++)
	  if (final < ag_val_f[s])
	    final = ag_val_f[s];
      }
      // count distinct
      else if (ag_fn == "CND") {
	if (ag_val_st.size() == 0)
	  final = 0;
	else {
	  // remove duplicates
	  for (uint r = 0; r < ag_val_st.size(); r++) {
	    bool flag = false;
	    for (uint o = 0; o < r; o++) {
	      if (ag_val_st[r] == ag_val_st[o]) {
		flag = true;
		break;
	      }
	    }
	    if (flag == true) {
	      ag_val_st.erase(ag_val_st.begin()+r);
	      r--;
	    }
	    
	  }
	  final = ag_val_st.size();
	}
      }
      else if (ag_fn == "CNT")
	final = ag_val_st.size();
      
      // add the value calculated for this tuple to the table T_temp
      string temp = SSTR(final);
      T_temp.columns[basic_attri+k].values.push_back(temp);

    } // end of 'for each group'
    // print_table(T_temp);
    k++;
  } // end of 'for each list_expr'
  
  // set Table T equal to Table T_temp
  T = T_temp;
  return T;
} // end of aggregate_fn

// check if a tuple is identical to previous tuples in the table
int check_duplicate (vector <uint> attri_index, uint tuple, table T) {
  if (T.columns.size() == 0)
    return -1;
  if (tuple == 0)
    return -1;
  bool duplicate = false;
  uint x;
  // for each tuple of the table, less than tuple
  for (x = 0; x < T.columns[0].values.size() && x < tuple; x++) {
    bool flag = true;
    // for each attribute in the attri_index
    for (uint i = 0; i < attri_index.size(); i++) 
      if (T.columns[attri_index[i]].values[x]
	  != T.columns[attri_index[i]].values[tuple]) {
	flag = false;
	break;
      }
    if (flag == true) {
      duplicate = true;
      break;
    }
  }
  if (duplicate == true)
    return x; // return the index of the duplicate
  else
    return -1; // return -1 to indicate that no duplicate is found
}

int find_attri (string temp, table T) {
  bool flag = false;
  uint x;
  for (x = 0; x < T.columns.size(); x++) 
    if (T.columns[x].name == temp) {
      flag = true;
      break;
    }
  // attribute doesn't exist in table T
  if (flag == false)
    return -1;
  else
    return x;
}







