#include <stdio.h>

//  RETURN VALUES  //
// 1 => SUCCESS
// 0 => Error opening database
extern int open_db(FILE **db_ptr,const char *DBNAME);

//  RETURN VALUES  //
// 1 => SUCCESS
// 0 => Object already in database
//-2 => Cannot open the file that contains the object
//-3 => No open database file
extern int import_obj(FILE **db_ptr ,const char *FNAME ,const char *OBJNAME);

//  RETURN VALUES  //
// char** array => array that contains the obj_names
// NULL => No open database file
extern char** find_similar_obj(FILE **db_ptr ,const char *OBJNAME);

//  RETURN VALUES  //
// 1 => SUCCESS
// 0 => OBJNAME not in database
//-2 => Cannot open file FNAME
//-3 => No open database file
extern int export_obj(FILE **db_ptr ,const char *OBJNAME ,const char *FNAME);

//  RETURN VALUES  //
// 1 => SUCCESS
//-2 => OBJNAME not in database
//-3 => No open database file
extern int delete_obj(FILE **db_ptr ,const char *DBNAME ,const char *OBJNAME);

//  RETURN VALUES  //
// 1 => SUCCESS
// 0 => No open database file
extern int close_db(FILE **fptr);

//  Deletes The Array Returned by the find_similar_obj() //
extern void find_array_clean(char **array);