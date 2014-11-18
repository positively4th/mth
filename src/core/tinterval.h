/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#pragma once

#include <fstream>

#include "tnmmatrix.h"
#include <string>

//class istream;
//class ::std::ostream;


namespace P4th
{

  using namespace std;

  template<class TYPE> 
    class tInterval {
  public:
    const static char _SEPARATOR;
  protected:
    string symbol;
    TYPE x0,x1;
    int points;
  public:
    tInterval( const string &str = "" );
    tInterval( const string &_symbol , TYPE _x0 , TYPE _x1 , int _points );
    
    string GetSymbol() const
    { return symbol; }
    TYPE GetLower() const
    { return x0; }
    TYPE GetUpper() const
    { return x1; }
    TYPE GetDelta() const
    { return x1 - x0; }
    int GetPoints() const
    { return points; }

    bool operator==( const tInterval &i ) const;

    virtual bool Parse( string str );

    virtual operator tnmmatrix<TYPE>();

    virtual std::ostream &Dump( std::ostream &dest , P4th::string lm = "" ) const; 

  };


}

