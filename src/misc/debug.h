/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#ifndef _DEBUG_H_
#define _DEBUG_H_

#if (_DEBUG > 0 && _WIN32 < 1 )
#include <iostream>
#include <iomanip>
#include <execinfo.h>
#include <stdio.h>

namespace P4th {
  void show_stackframe();
}

#define _DBOUT(str) std::cout << str << std::flush;
#define _DB(todo)  todo; std::cout << std::flush; std::cerr << std::flush;
//#define _DB(todo) { todo; {std::cout << std::flush; std::cerr << std::flush;} }
#define _DBWAITKEY(str) { std::cerr << __DATE__ << ": In " << __FILE__ << " at " << __LINE__ << std::endl << std::flush; std::cerr << str; std::cin.get(); }
#define _DBTHROW(str) { std::cerr << __DATE__ << ": In " << __FILE__ << " at " << __LINE__ << std::endl; P4th::show_stackframe(); std::cerr << "EXCEPTION: " << str << std::endl << "Press <CR>" << std::flush; std::cin.get(); throw (str); }

#define _DBDO(todo) { std::cerr << __DATE__ << ": In " << __FILE__ << " at " << __LINE__ << std::endl; P4th::show_stackframe(); {todo;} std::cerr << "Press <CR>" << std::flush; std::cin.get(); }

#else

#define _DBOUT(str)  ; 
#define _DB(todo) ; 
#define _DBWAITKEY(str) ; 
#define _DBTHROW(str) throw str; 
#define _DBDO(todo) {todo;} 

#endif

#ifdef _SAFE

#define _SF(todo) todo

#else

#define _SF(todo) ; 

#endif

#endif
