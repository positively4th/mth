/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#include "stlstringtools.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <math.h>
//#include <ctype.h>
#include <cctype>
#include <debug.h>

#include <stdarg.h>



namespace P4th
{

  ostream &operator<<( ostream &dest, const string &str )
  {
    dest << str.c_str();
    return dest;
  }
  
  string &STLStringTools::Append( string &dest , const string &item ,
				  const string &sep , bool infront )
  {
  if ( dest == "" )
    dest = item; 
  else
    if ( infront )
      dest = item + sep + dest;
  else
    dest += sep + item;
  return dest;
}

  string STLStringTools::AsString( double number ) 
  {
    std::ostringstream tmp;
    if ( ! ( tmp << number ) ) 
      throw (string)"Error: Could not convert number to a string.";
    return tmp.str();	
  }
  string STLStringTools::AsString( float number ) 
  {
    std::ostringstream tmp;
    if ( ! ( tmp << number ) ) 
      throw (string)"Error: Could not convert number to a string.";
    return tmp.str();	
  }
  string STLStringTools::AsString( int number ) 
  {
    std::ostringstream tmp;
    if ( ! ( tmp << number ) ) 
      throw (string)"Error: Could not convert number to a string.";
    return tmp.str();	
  }
  string STLStringTools::AsString( unsigned int number ) 
  {
    std::ostringstream tmp;
    if ( ! ( tmp << number ) ) 
      throw (string)"Error: Could not convert number to a string.";
    return tmp.str();	
  }

  string STLStringTools::AsString( double vv , int prec , char decchar )
  {
    if ( isnan( vv ) )
      return "NaN";
    
    if ( vv < 0 )
      return (string)"-" +  STLStringTools::AsString( -vv , prec , decchar );
    
    vv += 0.5 * pow( 10 , - prec );
    int maxpot = (int)log10( vv  );
    
    string res = "";
    if ( maxpot < 0 )
      maxpot = 0;
    
    if ( maxpot < -prec )
      return "0";
    
    int digit;
    //	_DB( std::cout << "prec = " << prec << std::endl; );
    while ( maxpot >= -prec )
      {
	digit = (int)(vv / pow(10,maxpot));
	/*
	  _DB( std::cout << "vv = " << vv << std::endl; );
	  _DB( std::cout << "maxpot = " << maxpot << std::endl; );
	  _DB( std::cout << "digit = " << digit << std::endl; );
	*/
	res += STLStringTools::AsString( digit );
	
	if ( maxpot == 0 )
	  res += decchar;
	vv -= (double)(digit) * pow( 10 , maxpot );
	maxpot--;
	//	    _DB( std::cout << "res = " << res << std::endl; );
      }
    
    //	_DB( std::cout << vv << " -> " << (const char *)res << std::endl; );
    return res;
  }

  bool STLStringTools::Contains( const string &data , const string &items )
  {
    return string::npos != data.find_first_of( items );
  }

  int STLStringTools::Count( const string &data , char item )
  {
    int ant = 0;
    for ( size_t i = 0 ; i < data.size() ; i++ )
      if ( data[i] == item )
	ant++;
    return ant;
  }
  
  string STLStringTools::GetNth( const string &str , int nr , const string &separator )
  {
    int next,next0;
    int l = separator.length();

    next = next0 = -l;
    do {
      nr--;
      next0 = next + l;
      next = str.find( separator , next + l );
      if ( next == string::npos )
	{
	  next = str.length();
	  break;
	}
    } while ( nr >= 0 );
    
    if ( nr < 0 )
      return str.substr(  next0 , next - next0 );
    
    return (string)"";
  }

  string STLStringTools::LeftOfExc( const string &str , size_t index )
  {
    //    return str.Left( index );
    return str.substr( 0 , index );
  }


  bool STLStringTools::IsNumber( const string &str , char separator )
  {
    if ( str.length() < 1 )
      return false;
    int i = 0;
    bool decimals = false;
    if ( str[i] == '-' )
      if ( str.length() > 1 )
      i++;
      else
	return false;
    for ( ; i < str.length() ; i++ )
      {
	if ( str[i] == separator )
	  if ( decimals )
	    return false;
	  else
	    decimals = true;
	else
	  if ( (int)str[i] < (int)'0' || (int)str[i] > (int)'9' )
	    return false;
      }
    return true;
  }

  string STLStringTools::LeftOfExc( const string &str , const string &pat )
  {
    size_t index = str.find( pat );
    if ( index == string::npos )
      return "";
    return LeftOfExc( str , index );
  }
  
  
  
  string STLStringTools::RightOfInc( const string &str , size_t index )
  {
    if ( index >= str.size() )
      return "";
    return str.substr( index );
  }

  string STLStringTools::RightOfInc( const string &str , const string &pat )
  {
    //    return RightOf( str , IndexRight( str , pat ) + pat.GetLength() );
    size_t index = str.find( pat );
    if ( index == string::npos )
      return "";
    return RightOfInc( str , index  );
  }

  double STLStringTools::String2Double( const string &str )
  {
    std::istringstream stm;
    stm.str( str );
    double d;
    stm >>d;
    return d;
  }

  int STLStringTools::String2Int( const string &str )
  {
    std::istringstream stm;
    stm.str( str );
    int i;
    stm >> i;
    return i;
  }

  string STLStringTools::String2Lower( const string &str )
  {
    string res( str );
    std::transform( res.begin() , res.end() , res.begin() , (int(*)(int))tolower );
    return res;
  }
  
  string STLStringTools::String2Upper( const string &str )
  {
    string res( str );
    std::transform( res.begin() , res.end() , res.begin() , (int(*)(int))toupper );
    return res;
  }

  string STLStringTools::Strip( const string &str , const string &skips , int upto )
  {
    if ( upto < 0 )
      upto = str.length();
  string res;
  int j = 0;
  for ( size_t i = 0 ; i < str.length() ; i++ ) {
    if ( j >= upto )
      break;
    if ( string::npos ==  skips.find( str[i] ) ) {
      res += str[i];
      j++;
    }
  }
  return res;
  }

  vector<string> STLStringTools::Split( const string &str , const string &glue )
  {
    vector<string> res;
    if (str.size() < 1) {
      return res;
    }
    size_t start = 0;
    size_t stop;
    while ((stop = str.find(glue, start)) != string::npos) {
      res.push_back(str.substr(start,stop-start)); 
      start = stop + glue.size();
    }
    //    if (start != 0) {
      //Only add if there was at least one occurence of glue
      res.push_back(str.substr(start)); 
      //    }
    return res;
  }

  string STLStringTools::Join(const _strVec &strVec, const string &glue ) 
  {
    if (strVec.size() < 1) {
      return "";
    }
    cstrVecee it = strVec.begin();
    string res = (*it);
    it++;
    for ( ; it != strVec.end() ; it++) {
      res += glue + (*it);
    }
    return res;
  }

}


