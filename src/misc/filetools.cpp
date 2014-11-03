
#include "filetools.h"
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <debug.h>
#include <stlstringtools.h>
#include <timetools.h>
#include <errno.h>

using namespace std;


namespace P4th
{

  bool FileTools::Delete(const string &filePath) {
    return unlink(filePath.c_str()) == 0;
  }
  
  bool FileTools::Exists(const string& fullpath) {
    ifstream f(fullpath.c_str());
    if (f.good()) {
      f.close();
      return true;
    } else {
      f.close();
      return false;
    }   
  }

  string FileTools::SafeFilename( const string &path , 
				  const string &base , 
				  const string &ext)
  {
    string id = "";
    string full = "";
    while ( FileTools::Exists( full = (path + "/" + base + id + "." + ext ) ) ) {
      id = "_" + STLStringTools::AsString( (unsigned int)TimeTools::Current2Time() );      
    }
    return full;
    
  }

}
