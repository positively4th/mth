/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#ifndef _P4TH_TSLATECERROR_H_
#define _P4TH_TSLATECERROR_H_

#include <stdio.h>
#include <time.h>
#include <timetools.h>
#include <string>
#include <texception.h>
#include <linkedptrlist.h>

#include "tslatecstack.h"
#include "tslatectools.h"


//class istream;
//class ::std::ostream;


using namespace std;

namespace P4th
{
  
  template<class TYPE> 
    class tSlatecError : public tException<tSlatecTools<TYPE> >
    {
    protected:
      string functionid;
      int code;
      time_t errortime;
    public:
      tSlatecError<TYPE>( string msg , string fid , int cd , time_t t = 0  ) : tException<tSlatecTools<TYPE> >(msg)
	{ functionid = fid; code = cd; if ( t != 0 ) this->errortime = t; ::time( &this->errortime ); }
      virtual ~tSlatecError() throw () {}      
      std::ostream &Print( std::ostream &dest , string lm = "" ) const
	{
	  dest << functionid << " generated the error " 
	       << code 
	       << " at " << TimeTools::Time2DateTime( errortime ) 
	       << this->message << std::endl;
	  return dest;
      }
    
  };
  



};



#define _SLATECERROR(code) tSlatecError<TYPE>("" , __PRETTY_FUNCTION__ , code )

#endif
