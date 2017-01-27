// PICTORIAL REPRESENTATION OF A TABLE
// -----------------------------------
//                   Columns
//                      ^
//           ___________|____________
//          /                        \           // for multiline comment warning
// Table -> Col1  Col2  Col3 ...  Coln  
//           |     |     |         |    
//           v     v     v         v    
//          Val1  Val1  Val1 ...  Val1  \        // for multiline comment warning
//          Val2  Val2  Val2 ...  Val2  |
//          Val3  Val3  Val3 ...  Val3  |
//           .     .     .         .    -->Attribute
//           .     .     .         .    |
//           .     .     .         .    |
//          Valm  Valm  Valm ...  Valm  /
//
// NOTE: Indexing is from 1->n in diagram, and 0->(n-1) in the program.
// ----

#ifndef _data_str_included_
#define _data_str_included_

#include <vector>
#include <iostream>

using namespace std;

struct attribute {
  string name, type;
  vector < string > values;
};
struct table {
  string name;
  vector < attribute > columns;
};

extern vector < table > dollar;
extern vector < table > list_table;
extern vector < string > broken_query;

#endif
