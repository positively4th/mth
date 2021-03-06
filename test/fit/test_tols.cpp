#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE tOLS
#include <boost/test/unit_test.hpp>

//Always include the typedef first!
#include <misctypes.h>
#include <coretypes.h>
#include <fittypes.h>

#include <tfit.h>
#include <tols.h>
#include <twasher.h>
#include <randomtools.h>


MTH_MISC_TYPES(double);
MTH_CORE_TYPES(double);
MTH_FIT_TYPES(double);

using namespace P4th;

bool IsEqual( double d1 ,double d2 , double tolerance ) 
{
  if ( tolerance > 0 )
    return sqrt( ( d1 - d2 ) * ( d1 - d2 ) ) < tolerance; 
  return sqrt( ( d1 - d2 ) * ( d1 - d2 ) ) < tolerance * sqrt( d1 * d2 ); 

}

bool IsEqual( _m M1 , _m M2  , double tolerance  ) 
{
  for ( int i = 1 ; i <= M1.GetRows() ; i++ )
    for ( int j = 1 ; j <= M1.GetCols() ; j++ )
      if ( ! IsEqual( M1(i,j) , M2(i,j) , tolerance ) )
	return false;
  return true;
}

BOOST_AUTO_TEST_CASE( Estimate_1 )
{
  const double dist_mul = 0.0001;
  _fit fit( 3 , 3 );
  $options fitOpts = fit.GetOptions();
  $_ols ols(new _ols(&fit));
  $_estimatorOpt::set(fitOpts.get(), "estimator", ols);

  tnmmatrix<double> X(10,3 );
  tnmmatrix<double> Y(10,3 );
  

  // Column 1 - Intercept
  X.Set(1 , 1 , 1 );
  X.Set(2 , 1 , 1 );
  X.Set(3 , 1 , 1 );
  X.Set(4 , 1 , 1 );
  X.Set(5 , 1 , 1 );
  X.Set(6 , 1 , 1 );
  X.Set(7 , 1 , 1 );
  X.Set(8 , 1 , 1 );
  X.Set(9 , 1 , 1 );
  X.Set(10 , 1 , 1 );

  // Column 2 - line x = (0,1,2,3)
  X.Set(1 , 2 , -3 ); // y = 3
  X.Set(2 , 2 , 3 ); // y = 10
  X.Set(3 , 2 , -4 ); // y = 2
  X.Set(4 , 2 , -1 ); // y = 5
  X.Set(5 , 2 , 0 ); // y = 6
  X.Set(6 , 2 , -2 ); // y = 4
  X.Set(7 , 2 , 0 ); // y = 7
  X.Set(8 , 2 , 1 ); // y = 8
  X.Set(9 , 2 , 2 ); // y = 9
  X.Set(10 , 2 , 4 ); // y = 11

  // Column 3 - line x = (0,1,2,3)
  X.Set(1 , 3 , -4 );  
  X.Set(2 , 3 , -3 ); 
  X.Set(3 , 3 , -2 ); 
  X.Set(4 , 3 , -1 ); 
  X.Set(5 , 3 , 0 ); 
  X.Set(6 , 3 , 0 ); 
  X.Set(7 , 3 , 1 ); 
  X.Set(8 , 3 , 2 ); 
  X.Set(9 , 3 , 3 ); 
  X.Set(10 , 3 , 4 );

  // Column 1 - line y = 2  + x
  Y.Set(1 , 1 , 2 + X(1,2) + dist_mul * RandomTools::DrawUniform( -1 , 1 ) );  
  Y.Set(2 , 1 , 2 + X(2,2) + dist_mul * RandomTools::DrawUniform( -1 , 1 ) );  
  Y.Set(3 , 1 , 2 + X(3,2) + dist_mul * RandomTools::DrawUniform( -1 , 1 ) );  
  Y.Set(4 , 1 , 2 + X(4,2) + dist_mul * RandomTools::DrawUniform( -1 , 1 ) );  
  Y.Set(5 , 1 , 2 + X(5,2) + dist_mul * RandomTools::DrawUniform( -1 , 1 ) );  
  Y.Set(6 , 1 , 2 + X(6,2) + dist_mul * RandomTools::DrawUniform( -1 , 1 ) );  
  Y.Set(7 , 1 , 2 + X(7,2) + dist_mul * RandomTools::DrawUniform( -1 , 1 ) );  
  Y.Set(8 , 1 , 2 + X(8,2) + dist_mul * RandomTools::DrawUniform( -1 , 1 ) );  
  Y.Set(9 , 1 , 2 + X(9,2) + dist_mul * RandomTools::DrawUniform( -1 , 1 ) );  
  Y.Set(10 , 1 , 2 + X(10,2) + dist_mul * RandomTools::DrawUniform( -1 , 1 ) );  

  // Column 2 - line y = 0  - 2 * x
  Y.Set(1 , 2 , 0 - 2 * X(1,3) + dist_mul * RandomTools::DrawUniform( -1 , 1 ) );  
  Y.Set(2 , 2 , 0 - 2 * X(2,3) + dist_mul * RandomTools::DrawUniform( -1 , 1 ) );  
  Y.Set(3 , 2 , 0 - 2 * X(3,3) + dist_mul * RandomTools::DrawUniform( -1 , 1 ) );  
  Y.Set(4 , 2 , 0 - 2 * X(4,3) + dist_mul * RandomTools::DrawUniform( -1 , 1 ) );  
  Y.Set(5 , 2 , 0 - 2 * X(5,3) + dist_mul * RandomTools::DrawUniform( -1 , 1 ) );  
  Y.Set(6 , 2 , 0 - 2 * X(6,3) + dist_mul * RandomTools::DrawUniform( -1 , 1 ) );  
  Y.Set(7 , 2 , 0 - 2 * X(7,3) + dist_mul * RandomTools::DrawUniform( -1 , 1 ) );  
  Y.Set(8 , 2 , 0 - 2 * X(8,3) + dist_mul * RandomTools::DrawUniform( -1 , 1 ) );  
  Y.Set(9 , 2 , 0 - 2 * X(9,3) + dist_mul * RandomTools::DrawUniform( -1 , 1 ) );  
  Y.Set(10 , 2 , 0 - 2 * X(10,3) + dist_mul * RandomTools::DrawUniform( -1 , 1 ) );  

  // Column 3 - line y = 0 + x1 - x2
  Y.Set(1 , 3 , X(1,2) - X(1,3)  + dist_mul * RandomTools::DrawUniform( -1 , 1 ) );  
  Y.Set(2 , 3 , X(2,2) - X(2,3) + dist_mul * RandomTools::DrawUniform( -1 , 1 ) );  
  Y.Set(3 , 3 , X(3,2) - X(3,3) + dist_mul * RandomTools::DrawUniform( -1 , 1 ) );  
  Y.Set(4 , 3 , X(4,2) - X(4,3) + dist_mul * RandomTools::DrawUniform( -1 , 1 ) );  
  Y.Set(5 , 3 , X(5,2) - X(5,3) + dist_mul * RandomTools::DrawUniform( -1 , 1 ) );  
  Y.Set(6 , 3 , X(6,2) - X(6,3) + dist_mul * RandomTools::DrawUniform( -1 , 1 ) );  
  Y.Set(7 , 3 , X(7,2) - X(7,3) + dist_mul * RandomTools::DrawUniform( -1 , 1 ) );  
  Y.Set(8 , 3 , X(8,2) - X(8,3) + dist_mul * RandomTools::DrawUniform( -1 , 1 ) );  
  Y.Set(9 , 3 , X(9,2) - X(9,3) + dist_mul * RandomTools::DrawUniform( -1 , 1 ) );  
  Y.Set(10 , 3 , X(10,2) - X(10,3) + dist_mul * RandomTools::DrawUniform( -1 , 1 ) );  

  fit.AddObservation( Y , X );
  fit.AddxNames( "1,X1,X2" );
  fit.AddyNames( "Y1,Y2,Y3" );


  _m B( *fit.GetB() );
  //  fit.PrintEstimate(std::cout);

  BOOST_CHECK_CLOSE_FRACTION( 2 , fit.Getb( 1 )["1"] , 0.001 );
  BOOST_CHECK_CLOSE_FRACTION( fit.Getb( 1 )["X1"] , 1 , 0.001 );  
  BOOST_CHECK_SMALL( fit.Getb( 1 )["X2"] , 0.001 ); 

  BOOST_CHECK_SMALL( B(1,2) , 0.001 );
  BOOST_CHECK_SMALL( B(2,2) , 0.001 ); 
  BOOST_CHECK_CLOSE_FRACTION( fit.Getb( 2 )["X2"] , -2 , 0.001 ); 

  BOOST_CHECK_SMALL( B(1,3) , 0.001 );
  BOOST_CHECK_CLOSE_FRACTION( fit.Getb( 3 )["X1"] ,  1 , 0.001 );  
  BOOST_CHECK_CLOSE_FRACTION( B(3,3) , -1 , 0.001 );


}

BOOST_AUTO_TEST_CASE( Estimate_2 )
{
  const int obsCount2 = 100;
  const int explCount2 = 4;
  const int depCount2 = 2;
  _fit fit( explCount2 , depCount2 );
  $options fitOpts = fit.GetOptions();
  $_ols ols(new _ols(&fit));
  $_estimatorOpt::set(fitOpts.get(), "estimator", ols);

  fit.AddxNames( "1,x1,x2,x3" );
  fit.AddyNames( "y1,y2" );

  _m B2( explCount2 , depCount2 );
  for ( int i = 1 ; i <= B2.GetRows() ; i++ )
    for ( int j = 1 ; j <= B2.GetCols() ; j++ )
      B2.Set( i , j , (double)(i + j) );
  for( int ctr = 0 ; ctr < obsCount2 ; ctr++ )
    {
      _m xRow( 1 , explCount2 );
      xRow.Set( 1 , 1 , 1 );
      for ( int j = 2 ; j <= xRow.GetCols() ; j++ )
	xRow.Set( 1 , j , RandomTools::DrawUniform( 0 , 1 ) );
      _m yRow( 1 , depCount2 );
      yRow.Mul( xRow , B2 );
      fit.AddObservation( yRow , xRow );
      
    }
  
  fit.GetB();
  //  fit.PrintEstimate( std::cout , "" , 20 );
  //  fit.PrintSummary( std::cout , "" , 20 );
  
  BOOST_CHECK( IsEqual( B2 , *fit.GetB() , 0.001 ) ); 
};

BOOST_AUTO_TEST_CASE( SetVariationMin_1 )
{
  const double dist_mul = 0.0001;
  _fit fit(3,3);
  //Get the Options instance for the tOLS estimation:
  $options options = fit.GetOptions();
  assert(options);

  //tNoWasher is the default washer for tFit, replace with tWasher
  $_washerOpt::set(options.get(), "washer", new _washer(&fit));
  //The Washer has its own Options instance, get it:
  $options washerOptions = $_washerOpt::read(options.get(), "washer")->GetOptions();
  assert(washerOptions);
  //Discard the (rightmost) explanatory, it is highly correlated with another explanatory  
  _typeOpt::set(washerOptions.get(), "variationmin" , 0.01);
  //Use the ols estimator
  $_ols ols(new _ols(&fit));
  $_estimatorOpt::set(options.get(), "estimator", ols);

  //Print discarded explanatories  
  _boolOpt::set(washerOptions.get(), "debug" , true); //Need debug to print anything
  _boolOpt::set(washerOptions.get(), "_DB_PRINT_DISCARDS" , true);

  /* The data */
  // Explanatories
  _m X(10,3 );

  // i
  X.Set(1 , 1 , 1 );
  X.Set(2 , 1 , 1 );
  X.Set(3 , 1 , 1 );
  X.Set(4 , 1 , 1 );
  X.Set(5 , 1 , 1 );
  X.Set(6 , 1 , 1 );
  X.Set(7 , 1 , 1 );
  X.Set(8 , 1 , 1 );
  X.Set(9 , 1 , 1 );
  X.Set(10 , 1 , 1 );

  // x_1
  X.Set(1 , 2 , 1000 ); // y = 3
  X.Set(2 , 2 , 1001 ); // y = 10
  X.Set(3 , 2 , 1000 ); // y = 2
  X.Set(4 , 2 , 1001 ); // y = 5
  X.Set(5 , 2 , 1000 ); // y = 6
  X.Set(6 , 2 , 1001 ); // y = 4
  X.Set(7 , 2 , 1000 ); // y = 7
  X.Set(8 , 2 , 1001 ); // y = 8
  X.Set(9 , 2 , 1000 ); // y = 9
  X.Set(10 , 2 , 1001 ); // y = 11

  // x_2
  X.Set(1 , 3 , -4 );  
  X.Set(2 , 3 , -3 ); 
  X.Set(3 , 3 , -2 ); 
  X.Set(4 , 3 , -1 ); 
  X.Set(5 , 3 , 0 ); 
  X.Set(6 , 3 , 0 ); 
  X.Set(7 , 3 , 1 ); 
  X.Set(8 , 3 , 2 ); 
  X.Set(9 , 3 , 3 ); 
  X.Set(10 , 3 , 4 );

  _m Y(10,3 );
  // Column 1 - line y = 2  + x_1
  Y.Set(1 , 1 , 2 + X(1,2) + dist_mul * RandomTools::DrawUniform( -1 , 1 ) );  
  Y.Set(2 , 1 , 2 + X(2,2) + dist_mul * RandomTools::DrawUniform( -1 , 1 ) );  
  Y.Set(3 , 1 , 2 + X(3,2) + dist_mul * RandomTools::DrawUniform( -1 , 1 ) );  
  Y.Set(4 , 1 , 2 + X(4,2) + dist_mul * RandomTools::DrawUniform( -1 , 1 ) );  
  Y.Set(5 , 1 , 2 + X(5,2) + dist_mul * RandomTools::DrawUniform( -1 , 1 ) );  
  Y.Set(6 , 1 , 2 + X(6,2) + dist_mul * RandomTools::DrawUniform( -1 , 1 ) );  
  Y.Set(7 , 1 , 2 + X(7,2) + dist_mul * RandomTools::DrawUniform( -1 , 1 ) );  
  Y.Set(8 , 1 , 2 + X(8,2) + dist_mul * RandomTools::DrawUniform( -1 , 1 ) );  
  Y.Set(9 , 1 , 2 + X(9,2) + dist_mul * RandomTools::DrawUniform( -1 , 1 ) );  
  Y.Set(10 , 1 , 2 + X(10,2) + dist_mul * RandomTools::DrawUniform( -1 , 1 ) );  

  // Column 2 - line y = 0  - 2 * x_2
  Y.Set(1 , 2 , 0 - 2 * X(1,3) + dist_mul * RandomTools::DrawUniform( -1 , 1 ) );  
  Y.Set(2 , 2 , 0 - 2 * X(2,3) + dist_mul * RandomTools::DrawUniform( -1 , 1 ) );  
  Y.Set(3 , 2 , 0 - 2 * X(3,3) + dist_mul * RandomTools::DrawUniform( -1 , 1 ) );  
  Y.Set(4 , 2 , 0 - 2 * X(4,3) + dist_mul * RandomTools::DrawUniform( -1 , 1 ) );  
  Y.Set(5 , 2 , 0 - 2 * X(5,3) + dist_mul * RandomTools::DrawUniform( -1 , 1 ) );  
  Y.Set(6 , 2 , 0 - 2 * X(6,3) + dist_mul * RandomTools::DrawUniform( -1 , 1 ) );  
  Y.Set(7 , 2 , 0 - 2 * X(7,3) + dist_mul * RandomTools::DrawUniform( -1 , 1 ) );  
  Y.Set(8 , 2 , 0 - 2 * X(8,3) + dist_mul * RandomTools::DrawUniform( -1 , 1 ) );  
  Y.Set(9 , 2 , 0 - 2 * X(9,3) + dist_mul * RandomTools::DrawUniform( -1 , 1 ) );  
  Y.Set(10 , 2 , 0 - 2 * X(10,3) + dist_mul * RandomTools::DrawUniform( -1 , 1 ) );  

  // Column 3 - line y = 0 + x1 - x2
  Y.Set(1 , 3 , X(1,2) - X(1,3)  + dist_mul * RandomTools::DrawUniform( -1 , 1 ) );  
  Y.Set(2 , 3 , X(2,2) - X(2,3) + dist_mul * RandomTools::DrawUniform( -1 , 1 ) );  
  Y.Set(3 , 3 , X(3,2) - X(3,3) + dist_mul * RandomTools::DrawUniform( -1 , 1 ) );  
  Y.Set(4 , 3 , X(4,2) - X(4,3) + dist_mul * RandomTools::DrawUniform( -1 , 1 ) );  
  Y.Set(5 , 3 , X(5,2) - X(5,3) + dist_mul * RandomTools::DrawUniform( -1 , 1 ) );  
  Y.Set(6 , 3 , X(6,2) - X(6,3) + dist_mul * RandomTools::DrawUniform( -1 , 1 ) );  
  Y.Set(7 , 3 , X(7,2) - X(7,3) + dist_mul * RandomTools::DrawUniform( -1 , 1 ) );  
  Y.Set(8 , 3 , X(8,2) - X(8,3) + dist_mul * RandomTools::DrawUniform( -1 , 1 ) );  
  Y.Set(9 , 3 , X(9,2) - X(9,3) + dist_mul * RandomTools::DrawUniform( -1 , 1 ) );  
  Y.Set(10 , 3 , X(10,2) - X(10,3) + dist_mul * RandomTools::DrawUniform( -1 , 1 ) );  

  fit.AddObservation( Y , X );
  fit.AddxNames( "1,X1,X2" );
  fit.AddyNames( "Y1,Y2,Y3" );

  //  fit.PrintEstimate( std::cout , "" , 20 );
  //  fit.PrintSummary( std::cout , "" , 20 );
 
  _m B( *fit.GetB() );
  BOOST_CHECK_SMALL( B(2,1) , 0.001 );  
  BOOST_CHECK_SMALL( B(2,2) , 0.001 ); 
  BOOST_CHECK_SMALL( B(2,3) , 0.001 );  


}

BOOST_AUTO_TEST_CASE( SetRhoMax_1 )
{
  const double dist_mul = 0.0001;

  _fit fit(4,2);
  //Get the Options instance for the tOLS estimation:
  $options options = fit.GetOptions();
  assert(options);
  //tNoWasher is the default washer for tFit, replace with tWasher
  $_washerOpt::set(options.get(), "washer", new _washer(&fit));
  //The Washer has its own Options instance, get it:
  $options washerOptions = $_washerOpt::read(options.get(), "washer")->GetOptions();
  //Discard the (rightmost) explanatory it is highly correlated with another explanatory  
  _typeOpt::set(washerOptions.get(), "correlationMax" , 0.99);
  //Use the ols estimator
  $_ols ols(new _ols(&fit));
  $_estimatorOpt::set(options.get(), "estimator", ols);



  _m X(10,4 );
  _m Y(10,2 );
  

  // x_1
  X.Set(1 , 1 , 1 );
  X.Set(2 , 1 , 1 );
  X.Set(3 , 1 , 1 );
  X.Set(4 , 1 , 1 );
  X.Set(5 , 1 , 1 );
  X.Set(6 , 1 , 1 );
  X.Set(7 , 1 , 1 );
  X.Set(8 , 1 , 1 );
  X.Set(9 , 1 , 1 );
  X.Set(10 , 1 , 1 );

  // x_2
  X.Set(1 , 2 , -4 ); // y = 3
  X.Set(2 , 2 , -3 ); // y = 10
  X.Set(3 , 2 , -2 ); // y = 2
  X.Set(4 , 2 , -1 ); // y = 5
  X.Set(5 , 2 , 0 ); // y = 6
  X.Set(6 , 2 , 0 ); // y = 4
  X.Set(7 , 2 , 1 ); // y = 7
  X.Set(8 , 2 , 2 ); // y = 8
  X.Set(9 , 2 , 3 ); // y = 9
  X.Set(10 , 2 , 4 ); // y = 11

  // x_3
  X.Set(1 , 3 , -4 );  
  X.Set(2 , 3 , -3 ); 
  X.Set(3 , 3 , -2 ); 
  X.Set(4 , 3 , -1 ); 
  X.Set(5 , 3 , 0 ); 
  X.Set(6 , 3 , 0 ); 
  X.Set(7 , 3 , 1 ); 
  X.Set(8 , 3 , 2 ); 
  X.Set(9 , 3 , 3 ); 
  X.Set(10 , 3 , 4 );

  // x_4
  X.Set(1 , 4 , -4 );  
  X.Set(2 , 4 , 3 ); 
  X.Set(3 , 4 , -2 ); 
  X.Set(4 , 4 , 1 ); 
  X.Set(5 , 4 , 0 ); 
  X.Set(6 , 4 , 0 ); 
  X.Set(7 , 4 , -1 ); 
  X.Set(8 , 4 , 2 ); 
  X.Set(9 , 4 , -3 ); 
  X.Set(10 , 4 , 4 );


  // y_1 = x1 + 2 x_2 + 3 x3 + 4 x_4 
  // y_2 = -x1 - 2 x_2 - 3 x3 - 4 x_4 
  for ( int row = 1 ; row <= 10 ; row++ ) {
    Y.Set(row , 1 , X(row,1) + 2 * X(row,2) + 3 * X(row,3) + 4 * X(row,4) 
	  + dist_mul * RandomTools::DrawUniform( -1 , 1 ) );  
    Y.Set(row , 2 , - X(row,1) - 2 * X(row,2) - 3 * X(row,3) - 4 * X(row,4) 
	  + dist_mul * RandomTools::DrawUniform( -1 , 1 ) );  
  }
  fit.AddObservation( Y , X );
  fit.AddxNames( "X1=i,X2,X3,X4" );
  fit.AddyNames( "Y1,Y2" );
  fit.GetB();

  //  fit.PrintEstimate( std::cout , "" , 20 );
  //  fit.PrintSummary( std::cout , "" , 20 );
 
  _m B( *fit.GetB() );
  BOOST_CHECK_CLOSE_FRACTION( B(2,1) , 2 + 3 , 0.001 );  
  BOOST_CHECK_SMALL( B(3,1) , 0.001 );  
  BOOST_CHECK_CLOSE_FRACTION( B(2,2) , - 2 - 3 , 0.001 );  
  BOOST_CHECK_SMALL( B(3,2) , 0.001 );  



}


BOOST_AUTO_TEST_CASE( Greene_p_240 )
{
  //Estimate 6 explanatories for 1 dependent variable:
  _fit fit(6,1);

  //Get the general Options for this fit instance and the washer:
  $options options = fit.GetOptions();

  //tNoWasher is the default washer for tFit, replace with tWasher
  $_washerOpt::set(options.get(), "washer", new _washer(&fit));

  $options washerOptions = $_washerOpt::read(options.get(), "washer")->GetOptions();

  //Prevent pairs of regressor with a sufficently high correlation coefficient:
  _typeOpt::set(washerOptions.get(), "variationmin" , 0.01);

  //Create and use an OLS estimator
  $_ols ols(new _ols(&fit));
  $_estimatorOpt::set(options.get(), "estimator", ols);

  //Setup the data:
  _m X(15,6 );
  _m Y(15,1 );
  
  /* Dependents */
  Y.Set(1 , 1 , 0.161 );
  Y.Set(2 , 1 , 0.172 );
  Y.Set(3 , 1 , 0.158 );
  Y.Set(4 , 1 , 0.173 );
  Y.Set(5 , 1 , 0.195 );
  Y.Set(6 , 1 , 0.217 );
  Y.Set(7 , 1 , 0.199 );
  Y.Set(8 , 1 , 0.163 );
  Y.Set(9 , 1 , 0.195 );
  Y.Set(10 , 1 , 0.231 );
  Y.Set(11 , 1 , 0.257 );
  Y.Set(12 , 1 , 0.259 );
  Y.Set(13 , 1 , 0.225 );
  Y.Set(14 , 1 , 0.241 );
  Y.Set(15 , 1 , 0.204 );

  /* Regressors */
  // 1
  X.Set(1 , 1 , 1 );
  X.Set(2 , 1 , 1 );
  X.Set(3 , 1 , 1 );
  X.Set(4 , 1 , 1 );
  X.Set(5 , 1 , 1 );
  X.Set(6 , 1 , 1 );
  X.Set(7 , 1 , 1 );
  X.Set(8 , 1 , 1 );
  X.Set(9 , 1 , 1 );
  X.Set(10 , 1 , 1 );
  X.Set(11 , 1 , 1 );
  X.Set(12 , 1 , 1 );
  X.Set(13 , 1 , 1 );
  X.Set(14 , 1 , 1 );
  X.Set(15 , 1 , 1 );
  // i (this regressor will be ignored)
  X.Set(1 , 2 , 1 );
  X.Set(2 , 2 , 1 );
  X.Set(3 , 2 , 1 );
  X.Set(4 , 2 , 1 );
  X.Set(5 , 2 , 1 );
  X.Set(6 , 2 , 1 );
  X.Set(7 , 2 , 1 );
  X.Set(8 , 2 , 1 );
  X.Set(9 , 2 , 1 );
  X.Set(10 , 2 , 1 );
  X.Set(11 , 2 , 1 );
  X.Set(12 , 2 , 1 );
  X.Set(13 , 2 , 1 );
  X.Set(14 , 2 , 1 );
  X.Set(15 , 2 , 1 );
  // T
  X.Set(1 , 3 , 1 ); // y = 3
  X.Set(2 , 3 , 2 ); // y = 10
  X.Set(3 , 3 , 3 ); // y = 2
  X.Set(4 , 3 , 4 ); // y = 5
  X.Set(5 , 3 , 5 ); // y = 6
  X.Set(6 , 3 , 6 ); // y = 4
  X.Set(7 , 3 , 7 ); // y = 7
  X.Set(8 , 3 , 8 ); // y = 8
  X.Set(9 , 3 , 9 ); // y = 9
  X.Set(10 , 3 , 10 ); // y = 11
  X.Set(11 , 3 , 11 ); // y = 3
  X.Set(12 , 3 , 12 ); // y = 10
  X.Set(13 , 3 , 13 ); // y = 2
  X.Set(14 , 3 , 14 ); // y = 5
  X.Set(15 , 3 , 15 ); // y = 6
  // G
  X.Set(1 , 4 , 1.058 );  
  X.Set(2 , 4 , 1.088 ); 
  X.Set(3 , 4 , 1.086 ); 
  X.Set(4 , 4 , 1.122 ); 
  X.Set(5 , 4 , 1.186 ); 
  X.Set(6 , 4 , 1.254 ); 
  X.Set(7 , 4 , 1.246 ); 
  X.Set(8 , 4 , 1.232 ); 
  X.Set(9 , 4 , 1.298 ); 
  X.Set(10 , 4 , 1.370 );
  X.Set(11 , 4 , 1.439 );  
  X.Set(12 , 4 , 1.479 ); 
  X.Set(13 , 4 , 1.474 ); 
  X.Set(14 , 4 , 1.503 ); 
  X.Set(15 , 4 , 1.475 ); 
  // R
  X.Set(1 , 5 , 5.16  );  
  X.Set(2 , 5 , 5.87 ); 
  X.Set(3 , 5 , 5.95 ); 
  X.Set(4 , 5 , 4.88 ); 
  X.Set(5 , 5 , 4.50 ); 
  X.Set(6 , 5 , 6.44 ); 
  X.Set(7 , 5 , 7.83 ); 
  X.Set(8 , 5 , 6.25 ); 
  X.Set(9 , 5 , 5.50 ); 
  X.Set(10 , 5 , 5.46 );
  X.Set(11 , 5 , 7.46 );  
  X.Set(12 , 5 , 10.28 ); 
  X.Set(13 , 5 , 11.77 ); 
  X.Set(14 , 5 , 13.42 ); 
  X.Set(15 , 5 , 11.02 ); 
  // P
  X.Set(1 , 6 , 4.40  );  
  X.Set(2 , 6 , 5.15 ); 
  X.Set(3 , 6 , 5.37 ); 
  X.Set(4 , 6 , 4.99 ); 
  X.Set(5 , 6 , 4.16 ); 
  X.Set(6 , 6 , 5.75 ); 
  X.Set(7 , 6 , 8.82 ); 
  X.Set(8 , 6 , 9.31 ); 
  X.Set(9 , 6 , 5.21 ); 
  X.Set(10 , 6 , 5.83 );
  X.Set(11 , 6 , 7.40 );  
  X.Set(12 , 6 , 8.64 ); 
  X.Set(13 , 6 , 9.31 ); 
  X.Set(14 , 6 , 9.44 ); 
  X.Set(15 , 6 , 5.99 ); 

  //Add all data in one call (can also be added row by row)
  fit.AddObservation( Y , X );

  //Set the names of the regressors (optional):
  fit.AddxNames( "1,i,T,G,R,P" );

  //Set the names of the dependent variables (optional):
  fit.AddyNames( "Y" );

 
  //Get the estimated coefficients
  _m B( *fit.GetB() );

  //Print Estimation Summary
  fit.PrintEstimate( std::cout , "" , 20 );
  fit.PrintSummary( std::cout , "" , 20 );

  /* Check agains Green */
  // p. 266
  BOOST_CHECK_CLOSE_FRACTION( -0.50907 , B(1,1) , 0.001 );  
  BOOST_CHECK_SMALL( B(2,1) , 0.001 );  
  BOOST_CHECK_CLOSE_FRACTION( -0.01658 , B(3,1) , 0.001 );  
  BOOST_CHECK_CLOSE_FRACTION( 0.67038 , B(4,1) , 0.001 );  
  BOOST_CHECK_CLOSE_FRACTION( -0.0023259 , B(5,1) , 0.001 );  
  BOOST_CHECK_CLOSE( -0.000094012 , B(6,1) , 1 );  
  // p 254
  BOOST_CHECK_CLOSE( 0.97244 , fit.GetR2()->Get(1,1) , 1 );  
  // p 266
  //The b covariance matrices are not considered general fit estiamte data.
  //To get them from the OLS estimation, get from the OLS misc property 
  //(all estimators have misc data):
  _mVec bCov = ols->GetMiscItem("bCov");
  //The every misc item is a vector with matrices. In this case 1 for 
  //every dependent variable.
  BOOST_CHECK_CLOSE( 0.0030391 , bCov[0].Get(1,1) , 1 );  
  BOOST_CHECK_CLOSE( 0.00010234 , bCov[0].Get(2,1) , 1 );  
  BOOST_CHECK_CLOSE( 0.0000038878 , bCov[0].Get(2,2) , 1 );  
  BOOST_CHECK_CLOSE( -0.0030102 , bCov[0].Get(3,1) , 1 );  
  BOOST_CHECK_CLOSE( -0.00010177 , bCov[0].Get(3,2) , 1 );  
  BOOST_CHECK_CLOSE( 0.0030247 , bCov[0].Get(3,3) , 1 );  
  BOOST_CHECK_CLOSE( 0.000005592 , bCov[0].Get(4,1) , 1 );  
  BOOST_CHECK_CLOSE( -0.0000002885 , bCov[0].Get(4,2) , 1 );  
  BOOST_CHECK_CLOSE( -0.0000072788 , bCov[0].Get(4,3) , 1 );  
  BOOST_CHECK_CLOSE( 0.0000014856 , bCov[0].Get(4,4) , 1 );  
  BOOST_CHECK_CLOSE( -0.0000032077 , bCov[0].Get(5,1) , 1 );  
  // misprint?  BOOST_CHECK_CLOSE( -0.0000002573 , bCov[0].Get(5,2) , 1 );  
  BOOST_CHECK_CLOSE( -0.000002279 , bCov[0].Get(5,3) , 1 );  
  BOOST_CHECK_CLOSE( -0.00000075071 , bCov[0].Get(5,4) , 1 );  
  BOOST_CHECK_CLOSE( 0.0000018157 , bCov[0].Get(5,5) , 1 );  

}
