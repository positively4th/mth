#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE tFitLS
#include <boost/test/unit_test.hpp>

//Always include the typedef first!
#include <misctypes.h>
#include <coretypes.h>
#include <fittypes.h>

#include <memory>

#include <tfit.h>
#include <twasher.h>
#include <tfitls.h>
#include <tselector.h>
#include <tnsolver.h>
#include <tptroption.h>
#include <randomtools.h>

MTH_MISC_TYPES(double);
MTH_CORE_TYPES(double);
MTH_SOLVER_TYPES(double);
MTH_FIT_TYPES(double);

using namespace P4th;

BOOST_AUTO_TEST_CASE( Estimate_1 )
{


  //Create a test function = f(x,z) = z
  std::unique_ptr<_f> f(new _arg(1));

  _fit fit(0, 1, 1 );
  //Get the options for the fit instance
  $options fitOpts = fit.GetOptions();

  //Use the (possibly) non-linear Least Square estimator tFitLS
  $_fitLS ls(new _fitLS(&fit, std::move(f)));
  //Get the options for the tFitLS estimator
  $options lsOptions = ls->GetOptions();
  _matrixOpt::set(lsOptions.get(), "guess", _m::ColumnVector(1, -1.2345));

  //Create a solver to use
  $_nsolver solver = $_nsolver(new _nsolver());
  //Get the optionsd for the solver
  $options solverOptions = solver->GetOptions();
  //Set options for solver
  _intOpt::set(solverOptions.get(), "maxIterations", 100);
  _doubleOpt::set(solverOptions.get(), "verbosity", 1.0 );
  _doubleOpt::set(solverOptions.get(), "maxF", 0.00001 );
  _boolOpt::set(solverOptions.get(), "simplify", false );
  _boolOpt::set(solverOptions.get(), "printIteration", false );
  _boolOpt::set(solverOptions.get(), "printF", false );
  _boolOpt::set(solverOptions.get(), "printdF", false );
  _boolOpt::set(solverOptions.get(), "printDeterminant", false );
  //  _matrixOpt::set(solverOptions.get(), "derivativeDeltas" , _m( 1 , 1 , 0.0 ) );
  //Register the solver in the least square options set.
  $_nsolverOpt::set(lsOptions.get(), "solver", solver);

  //Register the least square estimator in the fit options set
  $_estimatorOpt::set(fitOpts.get(), "estimator", ls);

  
  tnmmatrix<double> X(2,0);
  tnmmatrix<double> Y(2,1);
  
  // Column 1 - line y = 
  Y.Set(1 , 1 ,  7);  
  Y.Set(2 , 1 ,  13);  //z = Average = 10

  fit.AddObservation( Y , X );
  fit.AddxNames( "z" );
  fit.AddyNames( "y" );

  _m b( *fit.GetB() );
  fit.PrintEstimate(std::cout, "", 10);
  BOOST_CHECK_CLOSE_FRACTION( 10 , b.Get(1,1) , 0.001 );

}


BOOST_AUTO_TEST_CASE( Estimate_2 )
{

  // f = (x1-z1)^2 + (x2-z2)^2 
  std::unique_ptr<_f > f(
			 _f::Add(
				 _f::Square(
					    _f::Sub(
						    /* x1 */ new _arg(1),
						    /* z1 */ new _arg(3))),
				 _f::Square(
					    _f::Sub(
						    /* x2 */ new _arg(2),
						    /* z2 */ new _arg(4)))
				 ));

  _fit fit(2, 1, 2 );
  //Get the options for the fit instance
  $options fitOpts = fit.GetOptions();
  
  //Use the (possibly) non-linear Least Square estimator tFitLS
  $_fitLS ls(new _fitLS(&fit, std::move(f)));
  //Get the options for the tFitLS estimator
  $options lsOptions = ls->GetOptions();
  _matrixOpt::set(lsOptions.get(), "guess", _m::ColumnVector(2, -1.2345, 4));

  //Create a solver to use
  $_nsolver solver = $_nsolver(new _nsolver());
  //Get the optionsd for the solver
  $options solverOptions = solver->GetOptions();
  //Set options for solver
  _intOpt::set(solverOptions.get(), "maxIterations", 100);
  _doubleOpt::set(solverOptions.get(), "verbosity", 1.0 );
  _doubleOpt::set(solverOptions.get(), "maxF", 0.00001 );
  _boolOpt::set(solverOptions.get(), "simplify", false );
  _boolOpt::set(solverOptions.get(), "printIteration", false );
  _boolOpt::set(solverOptions.get(), "printX", true );
  _boolOpt::set(solverOptions.get(), "printF", false );
  _boolOpt::set(solverOptions.get(), "printdF", false );
  _boolOpt::set(solverOptions.get(), "printDeterminant", false );
  _matrixOpt::set(solverOptions.get(), "derivativeDeltas" , _m( 1 , 1 , 0.01 ) );
  //Register the solver in the least square options set.
  $_nsolverOpt::set(lsOptions.get(), "solver", solver);

  //Register the least square estimator in the fit options set
  $_estimatorOpt::set(fitOpts.get(), "estimator", ls);

  

  tnmmatrix<double> X(4,2);
  tnmmatrix<double> Y(4,1);
  
  //z1 = 1;
  //z2 = -2;
  X.Set(1 , 1 ,  1); X.Set(1 , 2 , -2); Y.Set(1 , 1 ,  0);
  X.Set(2 , 1 ,  3); X.Set(2 , 2 , -2); Y.Set(2 , 1 ,  4);
  X.Set(3 , 1 ,  1); X.Set(3 , 2 , -5); Y.Set(3 , 1 ,  9);
  X.Set(4 , 1 ,  3); X.Set(4 , 2 , -5); Y.Set(4 , 1 ,  13);

  fit.AddObservation(Y, X);
  fit.AddxNames( "z1,z2" );
  fit.AddyNames( "y" );
  _m b( *fit.GetB() );
  fit.PrintEstimate(std::cout, "", 10);
  
  BOOST_CHECK_CLOSE_FRACTION( 1 , b.Get(1,1) , 0.001 );
  BOOST_CHECK_CLOSE_FRACTION( -2 , b.Get(2,1) , 0.001 );

}
