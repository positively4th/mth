/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#ifndef _TIMETOOLS_H_
#define _TIMETOOLS_H_

#include <string>
#include <time.h>

using namespace std;

namespace P4th
{

//! Tools for time related operations
/*!
*/
class TimeTools
{
public:
  /// Current time as time_t
  static time_t Current2Time();
  /// Current as string
  static string Current2DateTime()
  { return Time2DateTime( Current2Time() ); }
  /// Convert the  DateTime @param str to a time_t
  static time_t DateTime2Time( const string &str ); 
  /// Convert the date @param str to a time_t
  static time_t Date2Time( const string &str ); 
  /// Convert the time_t @param secs to a DateTime string with separators @param sep1 and @param sep2
  static string Time2DateTime( time_t secs , const string &sep1 = "-" , const string &sep2 = ":" ); 
  /// Convert the time_t @param secs to a date string
  static string Time2Date( time_t secs ); 
  /// Convert the time_t @param secs to a DateTime string
  static string Time2DateTime(); 
  /// Convert the time_t @param secs to a DateTime string with separators @param sep1 and @param sep2
  static string Time2DateTime( const string &sep1 , const string &sep2 ); 
  /// Extract time part of the DateTime @param str time_t time
  static time_t Time2Time( const string &str ); 

};

} 

#endif
