/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#ifndef _TMATHTOOLS_H_
#define _TMATHTOOLS_H_

#include <string>
#include <limits>
#include <math.h>
#include <assert.h>
#include <tfunction0.h>
#include <tnmmatrix.h>

#undef max

#ifdef _GSL
#include <gsl/gsl_math.h>
#endif

using namespace std;

namespace P4th
{


template <class TYPE>
class tMathTools  
{
 public:
  /*
  static TYPE GetNaN()
  {
    assert( std::numeric_limits<TYPE>::has_quiet_NaN );
    return std::numeric_limits<TYPE>::quiet_NaN();
  }
  */

#ifdef _GSL
    static void InitGSLErrorHandler();
    static void GSLErrorHandler(const char * reason, const char * file, int line,int gsl_errno); 
    static gsl_error_handler_t *gslerrorhandler0;   
#endif


  static TYPE Max();

  static TYPE NaN()
  {
    assert( std::numeric_limits<TYPE>::has_quiet_NaN );
    return std::numeric_limits<TYPE>::quiet_NaN();
  }

  static int IsNaN( const TYPE &x )
  {
    assert( std::numeric_limits<TYPE>::has_quiet_NaN );
    return x == std::numeric_limits<TYPE>::quiet_NaN();
  }
  static int IsInf( const TYPE &x )
  {
    assert( std::numeric_limits<TYPE>::has_infinity );
    return x == std::numeric_limits<TYPE>::infinity();
  }
  static int IsNumber( const TYPE &x )
  {
    return ! ( IsInf( x ) || IsNaN( x ) );
  }


  static int HasNaN( const tnmmatrix<TYPE> &x );
  static int HasInf( const tnmmatrix<TYPE> &x );
  static int OnlyNumbers( const tnmmatrix<TYPE> &x );

  static TYPE Inf()
  {
    assert( std::numeric_limits<TYPE>::has_infinity );
    return std::numeric_limits<TYPE>::infinity();
  }

  static TYPE Abs( const TYPE &x )
    {
      if ( x < (TYPE)0 )
	return -(TYPE)x;
      return (TYPE)x;
    }

  static TYPE Max( const TYPE x , const TYPE y )
  {
    if ( x >= y )
      return x;
    return y;
  }

  static TYPE Radians( const TYPE aDegrees ) 
  {
    return M_PI * aDegrees / 180;
  }

  static TYPE Degrees( const TYPE aRadians ) 
  {
    return 180 *  aRadians / M_PI;
  }

  static TYPE Min( const TYPE x , const TYPE y )
  {
    if ( x <= y )
      return x;
    return y;
  }
  
  static int MinDecimals( TYPE x , int maxdecimals )
  {
    int ctr = 0;
    x = tMathTools::Abs( x );
    while ( ctr < maxdecimals )
      {
	if ( (TYPE)(long int)x == x ) 
	  return ctr;
	ctr++;
	x *= 10.0;
      }
    return maxdecimals;
  }

  static TYPE Derivativex1( const TYPE &x0 , TYPE &d , const TYPE &mul )
    {
      volatile TYPE add;
      d = sqrt( tFunction0<TYPE>::Epsilon() ) * x0;
      if ( tMathTools<TYPE>::Abs( (TYPE)add ) <= tFunction0<TYPE>::Epsilon() )
	d = sqrt( tFunction0<TYPE>::Epsilon() );
      d *= mul;
      volatile TYPE tmp = x0 + d;
      d = tmp - x0;
      
      return x0 + d;
  }


  static std::ostream &PrintColumnVector( std::ostream &dest , 
					  const tnmmatrix<TYPE>  &matrix ,  
					  int width ,  
					  string collabels = "" ,  
					  string lm = ""  , 
					  const string &separator = "," )
    {
      tnmmatrix<TYPE> tmp( matrix.GetCols() , matrix.GetRows() );
      matrix.Trp( &tmp );
      return PrintMatrix( dest , tmp , width , "" , collabels , lm , separator ); 
    }
  
  static std::ostream &PrintMatrix( std::ostream &dest , const tnmmatrix<TYPE>  &matrix ,  
				    int width , string rowlabels = "" , 
				    string collabels = "" ,  
				    string lm = ""  , const string &separator = "," );
  
  static string PrettyNumber( TYPE nr , int width = 100 );
  
 };
 
}
#endif
