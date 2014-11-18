/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/

#include "tinterval.h"

#include <debug.h>
#include <math.h>
#include <iomanip>
#include <string>
#include <stlstringtools.h>
#include <algorithm>

using namespace std;

namespace P4th
{

  template<class TYPE> 
  const char tInterval<TYPE>::_SEPARATOR = ':';
  

  template<class TYPE> 
  tInterval<TYPE>::tInterval( const string &str )  
  {
    this->Parse( str );
  }
  

  template<class TYPE> 
  tInterval<TYPE>::tInterval( const string &_symbol , TYPE _x0 , TYPE _x1 , int _points )
  {
    symbol = _symbol;
    x0 = _x0;
    x1 = _x1;
    points = _points;
  }

  template <class TYPE>
  bool tInterval<TYPE>::Parse( string str ) 
  {
  //    _DB( std::cout << "void tInterval<TYPE>::Parse( const string str )" << std::endl; );
    
    symbol = "";
    x0 = x1 = 0;
    points = 0;	
    if ( str == (string)"" )
      return true;
    
    str += tInterval<TYPE>::_SEPARATOR;
    
    //    _DB( std::cout << "str = " << str << std::endl; );

    int i = 0;
    
    string current = STLStringTools::GetNth( str , i , "=" );
    //    _DB( std::cout << "i = " << i << ", current = " << current << std::endl; );
    if ( ! STLStringTools::IsNumber( current ) )
      {
	symbol = current;
	str = STLStringTools::RightOfInc( str , 1 + STLStringTools::Index( str , "=" ) );
      } else
      symbol = "";

    current = STLStringTools::GetNth( str , i , tInterval<TYPE>::_SEPARATOR );
    //    _DB( std::cout << "i = " << i << ", current = " << current << std::endl; );
    if ( current == (string)"" || ! STLStringTools::IsNumber( current ) )
      {
	return false;
      } else {
      x0 = STLStringTools::String2Double( current );
      i++;
    }

    current = STLStringTools::GetNth( str , i , tInterval<TYPE>::_SEPARATOR );
    //    _DB( std::cout << "i = " << i << ", current = " << current << std::endl; );
    if ( current == (string)"" || ! STLStringTools::IsNumber( current ) )
      {
	x1 = x0;
	points = 1;
	return true;
      } else {
      x1 = STLStringTools::String2Double( current );
      i++;
    }

    current = STLStringTools::GetNth( str , i , tInterval<TYPE>::_SEPARATOR );
    //    _DB( std::cout << "i = " << i << ", current = " << current << std::endl; );
    if ( current == (string)"" || ! STLStringTools::IsNumber( current ) )
      {
	points = 2;
	return true;
      } else {
      points = STLStringTools::String2Int( current );
    }

    return true;
  } 
  
  template <class TYPE>
  bool tInterval<TYPE>::operator==( const tInterval &i ) const 
  {
    if ( x0 != i.x0 || x1 != i.x1 || points != i.points )
      return false;
    if ( symbol != i.symbol )
      return false;

    return true;
  }	 

  template <class TYPE>
  tInterval<TYPE>::operator tnmmatrix<TYPE>()
  {
    if ( points <= 0 )
      _DBTHROW( (string)"Interval not initialized." );

    if ( points == 1 )
      return tnmmatrix<TYPE>( 1 , 1 , x0 );

    tnmmatrix<TYPE> res( points , 1 );
    TYPE step = (x1 - x0) / (TYPE)(points - 1);
    TYPE x = x0;
    for ( int i = 1 ; i <= points ; i++ )
      {
	res(i,1) = x;
	x += step;
      }
    return res;
  }

  template<class TYPE> 
  std::ostream &tInterval<TYPE>::Dump( std::ostream &dest , P4th::string lm ) const 
  {
    dest << lm << "x0\t = " << x0 << std::endl;
    dest << lm << "x1\t = " << x1 << std::endl;
    dest << lm << "points\t = " << points << std::endl;
    return dest;
  }
  
}

template class P4th::tInterval<float>;
template class P4th::tInterval<double>;

#include <linkedlist.cpp>
#include <linkedlistpost.cpp>
template class P4th::LinkedList<P4th::tInterval<float> >;
template class P4th::LinkedList<P4th::tInterval<double> >;
template class P4th::LinkedListPost<P4th::tInterval<float> >;
template class P4th::LinkedListPost<P4th::tInterval<double> >;






