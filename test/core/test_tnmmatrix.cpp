/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE tnmmatrix
#include <boost/test/unit_test.hpp>

#include "tnmmatrix.h"
#include "tmathtools.h"
//#include "signalhandler.h"
#include <randomtools.h>

using namespace P4th;


BOOST_AUTO_TEST_CASE( testDet )
{

  // 1x1 Matrix
  tnmmatrix<double> A11( 1 , 1 );
  
  A11.Set( 1 , 1, 1 );
  BOOST_CHECK_CLOSE_FRACTION( 1 , A11.Det() , 0.00001  );

  A11.Set( 1.1 , 1, -1.5 );
  BOOST_CHECK_CLOSE_FRACTION( -1.5 ,  A11.Det() , 0.00001 );

  A11.Set( 1 , 1, 0.0 );
  BOOST_CHECK_SMALL( A11.Det() , 0.00001 );

  // 2x2 Matrix
  const int testCountA22 = 10;
  tnmmatrix<double> A22( 2 , 2 );

  for ( int i = 1 ; i <= testCountA22 ; i++ ) {
    for ( int r = 1 ; r <= 2 ; r++ ) 
      for ( int c = 1 ; c <= 2 ; c++ ) 
	A22.Set( r , c , RandomTools::DrawUniform( -10 , 10 ) );
    BOOST_CHECK_CLOSE_FRACTION( A22(1,1) * A22(2,2) - A22(1,2) * A22(2,1) , A22.Det() , 0.001  );
  };
}


BOOST_AUTO_TEST_CASE( testInv )
{

  tnmmatrix<double> A( 2 , 2 );
  tnmmatrix<double> Ainv( 2 , 2 , tMathTools<double>::Inf() );
  //  tnmmatrix<double> Ainv( 2 , 2 , 1.0/ 0.0);
  tnmmatrix<double> I( 2 , 2 );
  A.Set( 1 , 1 , 1.0 );
  A.Set( 1 , 2 , 0.0 );
  A.Set( 2 , 1 , 0.0 );
  A.Set( 2 , 2 , 1.0 );
  A.Inv( Ainv );

  I.Mul( A , Ainv );
  for ( int r = 1 ; r <= 2 ; r++ ) 
    for ( int c = 1 ; c <= 2 ; c++ ) 
      if ( r == c  )
	BOOST_CHECK_CLOSE_FRACTION( 1.0 , I(r,c) , 0.0001 );
      else
	BOOST_CHECK_SMALL( I(r,c) , 0.0001 );


  const int testCount = 2;
  for ( int i = 1 ; i <= testCount ; i++ ) {
    int dim = (int)RandomTools::DrawUniform( 1 , 30 );
    tnmmatrix<double> A( dim , dim );
    tnmmatrix<double> Ainv( dim , dim );
    tnmmatrix<double> I( dim , dim );
    for ( int r = 1 ; r <= dim ; r++ ) 
      for ( int c = 1 ; c <= dim ; c++ ) 
	A.Set( r , c , RandomTools::DrawUniform( -10 , 10 ) );
    if ( A.Det() < 0.00001 / (double)dim )
      continue;
    
    A.Inv( Ainv );

    I.Mul( A , Ainv );
    for ( int r = 1 ; r <= dim ; r++ ) 
      for ( int c = 1 ; c <= dim ; c++ ) 
	if ( r == c  )
	  BOOST_CHECK_CLOSE_FRACTION( 1.0 , I(r,c) , 0.0001 );
	else
	  BOOST_CHECK_SMALL( I(r,c) , 0.0001 );

  }
}

#ifdef _GSL

BOOST_AUTO_TEST_CASE( testSingularInv )
{

  tnmmatrix<double> A( 2 , 2 );
  tnmmatrix<double> Ainv( 2 , 2 , tMathTools<double>::Inf() );
  A.Set( 1 , 1 , 1.0 );
  A.Set( 1 , 2 , 0.0 );
  A.Set( 2 , 1 , 1.0 );
  A.Set( 2 , 2 , 0.0 );
  
  BOOST_CHECK_THROW ( A.Inv( Ainv ) , std::exception );
}

#endif
