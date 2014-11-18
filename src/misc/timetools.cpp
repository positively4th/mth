/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#include "timetools.h"

using namespace std;

namespace P4th
{

  time_t TimeTools::Current2Time()
  {
    return time( 0l );
  }

  time_t TimeTools::DateTime2Time( const string &str )
  {
    tm tmp;
    strptime( str.c_str() , "%Y-%m-%d %H:%M:%S" , &tmp );
    return mktime( &tmp );
    
  }

  time_t TimeTools::Date2Time( const string &str )
  {
    string str2 = str + " 00:00:00";
    return DateTime2Time( str2 );
  }

  string TimeTools::Time2DateTime( time_t secs , const string &sep1 , const string &sep2 )
  {
    char tmp[256];
    tm *tmptm = localtime( &secs );
    strftime( tmp , 256 , ("%Y" + sep1 + "%m" + sep1 + "%d" + " " + "%H" + sep2 + "%M" + sep2 + "%S").c_str() , tmptm );
    return (string)tmp;
  }

  string TimeTools::Time2Date( time_t secs )
  {
    char tmp[256];
    tm *tmptm = localtime( &secs );
    strftime( tmp , 256 , "%Y-%m-%d" , tmptm );
    return (string)tmp;
  }

  string TimeTools::Time2DateTime()
  {
    time_t secs = time( 0L );
    return Time2DateTime( secs );
  }

  string TimeTools::Time2DateTime( const string &sep1 , const string &sep2 )
  {
    time_t secs = time( 0L );
    return Time2DateTime( secs , sep1 , sep2 );
  }
    

  time_t TimeTools::Time2Time( const string &str )
  {
    tm tmp;
    strptime( str.c_str() , "%H:%M:%S" , &tmp );
    return mktime( &tmp );
    
  }


}
