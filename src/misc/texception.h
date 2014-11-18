/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#ifndef _P4TH_TEXCEPTION_H_
#define _P4TH_TEXCEPTION_H_

#include <string>
#include <time.h>
#include <timetools.h>
#include <exception>

using namespace std;

namespace P4th{

  template<class TYPE>
    class tException : public exception {
  protected:
    string message;
    string where;
    time_t when;
  public:
    tException( string _message, string _where = "", time_t _when = 0) { 
      message = _message;
      where = _where;
      if ( _when != 0 ) 
	this->when = _when; 
      else
	::time( &this->when );
    }
    virtual ~tException() throw () {}
    virtual const char* what() const throw()
    { return message.c_str(); }
    
  };
  
}

#define aComma ,
#define aException(type, msg) tException<type>(msg , __PRETTY_FUNCTION__ )

#endif
