/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#include "tmathtools.h"
#include <debug.h>

#include <string>
#include <iostream>
#include <iomanip>

#include <stlstringtools.h>



namespace P4th
{

#ifdef _GSL
  template<class TYPE> 
  gsl_error_handler_t *tMathTools<TYPE>::gslerrorhandler0 = NULL; 

  template<class TYPE> 
  void tMathTools<TYPE>::InitGSLErrorHandler()
  {
    if ( gslerrorhandler0 != NULL )
      return;

    assert( gslerrorhandler0 == NULL );
    gslerrorhandler0 = gsl_set_error_handler( &GSLErrorHandler );
  }

  template<class TYPE> 
  void tMathTools<TYPE>::GSLErrorHandler(const char * reason, const char * file, int line,int gsl_errno)
  {
    throw tException<tMathTools<TYPE> >( (string)"GSL error (" + STLStringTools::AsString( gsl_errno ) + ")"  
					 + ": " + (string)reason 
					 + "( at " + STLStringTools::AsString( line ) + " in " + (string)file + ")."  );
  }

#endif

  template<class TYPE>
  TYPE tMathTools<TYPE>::Max()
  {
    return std::numeric_limits<TYPE>::max();
  }

  template<class TYPE>
  int tMathTools<TYPE>::HasNaN( const tnmmatrix<TYPE> &x )
  {
    for ( int i = 1 ; i <= x.GetRows() ; i++ )
      for ( int j = 1 ; j <= x.GetCols() ; j++ )
	if ( tMathTools<TYPE>::IsNaN( x.Get(i,j) ) )
	  return 1;
    return 0;
  }

  template<class TYPE>
  int tMathTools<TYPE>::HasInf( const tnmmatrix<TYPE> &x )
  {
    for ( int i = 1 ; i <= x.GetRows() ; i++ )
      for ( int j = 1 ; j <= x.GetCols() ; j++ )
	if ( tMathTools<TYPE>::IsInf( x.Get(i,j) ) )
	  return 1;
    return 0;
  }

  template<class TYPE>
  int tMathTools<TYPE>::OnlyNumbers( const tnmmatrix<TYPE> &x )
  {
    if ( tMathTools<TYPE>::HasNaN( x ) || tMathTools<TYPE>::HasInf( x )  )
      return 0;
    return 1;
  }


  
  template <class TYPE>
  std::ostream &tMathTools<TYPE>::PrintMatrix( std::ostream &dest , const tnmmatrix<TYPE>  &matrix ,  
					       int width , string rowlabels , string collabels ,  
					       string lm , const string &sep )
  {
    if ( collabels != (string)"" ) 
      {
	dest << lm;
	if ( rowlabels != (string)"" )
	  dest << std::setw( width ) << " "; 	  
	for ( int j = 1 ; j <= matrix.GetCols() ; j++ )
	  dest << std::setw( width ) << 
	    STLStringTools::GetNth( collabels , j - 1 , sep ).substr( 0 , width ).c_str(); 
	dest << std::endl;
      }

    for ( int i = 1 ; i <= matrix.GetRows() ; i++ )
      {
	dest << lm;
	if ( rowlabels != (string)"" )
	  dest << std::setw( width ) << 
	    STLStringTools::GetNth( rowlabels , i - 1 , sep ).substr( 0 , width ).c_str(); 
	for ( int j = 1 ; j <= matrix.GetCols() ; j++ )
	  {
	    dest << std::setw( width ) << 
	      tMathTools<TYPE>::PrettyNumber( matrix.Get( i , j ) , width - 1 ).c_str();
	  }
	dest << std::endl;
      }
    return dest;
  }


  template <class TYPE>
  string tMathTools<TYPE>::PrettyNumber( TYPE nr  , int width )
  {
    TYPE p;
    
    if ( Abs( nr ) <  pow( 10 , -12 ) )
      return "0.000";

    p = log10( tMathTools<double>::Abs( nr ) );


    if ( p > 12 )
      return STLStringTools::AsString( nr / (TYPE)1000000000000, 3 , '.' ).substr( 0 , width - 1 ) + "T";
    if ( p > 9 )
      return STLStringTools::AsString( nr / (TYPE)1000000000 , 3 , '.' ).substr( 0 , width - 1 ) + "G";
    if ( p > 6 )
      return STLStringTools::AsString( nr / (TYPE)1000000 , 3 , '.').substr( 0 , width - 1 ) + "M";
    if ( p > 3 )
      return STLStringTools::AsString( nr / (TYPE)1000 , 3 , '.' ).substr( 0 , width - 1 ) + "k";

    if ( p < -9 )
      return STLStringTools::AsString( (TYPE)1000000000000 * nr , 3 , '.').substr( 0 , width - 1 ) + "p";
    if ( p < -6 )
      return STLStringTools::AsString( (TYPE)1000000000 * nr , 3 , '.' ).substr( 0 , width - 1 ) + "n";
    if ( p < -6 )
      return STLStringTools::AsString( (TYPE)1000000 * nr , 3 , '.').substr( 0 , width - 1 ) + "u";
    if ( p < -3 )
      return STLStringTools::AsString( (TYPE)1000 * nr , 3 , '.' ).substr( 0 , width - 1 ) + "m";


    return STLStringTools::AsString( nr , 3 , '.' ).substr( 0 , width );

    
  }

}

template class P4th::tMathTools<double>;
template class P4th::tMathTools<float>;

