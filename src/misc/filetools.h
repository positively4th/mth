/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#ifndef _FILETOOLS_H_
#define _FILETOOLS_H_



#include <linkedlist.h>
#include <fstream>

using namespace std;

namespace P4th
{
  
  class FileTools  
  {
  public:
	static bool Delete(const string &filePath);
	static bool Exists(const string &filePath);
	//Delete file given by path
	static string SafeFilename( const string &path, const string &base , const string &ext);


};

}
#endif
