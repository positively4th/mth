#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE tNSolver
#include <boost/test/unit_test.hpp>

#include <misctypes.h>
#include <coretypes.h>
#include <solvertypes.h>

#include <tptroption.h>
#include <tconstant.h>
#include <tparameter.h>
#include <tselector.h>
#include <tnsolver.h>
#include <tintervalstepper.h>
#include <randomtools.h>
#include <memory>

using namespace P4th;

MTH_MISC_TYPES(double);
MTH_CORE_TYPES(double);
MTH_SOLVER_TYPES(double);

BOOST_AUTO_TEST_CASE( solveAnalytical )
{

  int testCount = 100;
  for ( int i = 1 ; i <= testCount ; i++ ) {
    
    
    double _a = RandomTools::DrawUniform( -10 , 10 );
    double _b = RandomTools::DrawUniform( -10 , 10 );
    double _c = RandomTools::DrawUniform( -10 , 10 );
    double _d = RandomTools::DrawUniform( -10 , 10 );
    
    _par a( &_a );
    _par b( &_b );
    _par c( &_c );
    _par d( &_d );

    double rootee = _c * _c - 4 * _a * _b * _d;
    if ( rootee < 0 || _a == 0.0 ) {
      i--;
      continue;
    }
    rootee /= 4 * _a * _a;
    _nsolver solver;
    

    _intOpt::set(solver.GetOptions().get(), "maxIterations", 100);
    _doubleOpt::set(solver.GetOptions().get(), "verbosity", 0.0 );
    _doubleOpt::set(solver.GetOptions().get(), "maxF", 0.00001 );
    solver.AddChoice( "x" );
    solver.AddChoice( "y" );
    solver.AddEquation($$_f(	      
			    _f::Add(
				    _f::Mul( a.Clone(), new _arg( 1 ) ) , 
				    _f::Mul( b.Clone(), new _arg( 2 ) ) , 
				    _f::Mul( c.Clone(), new tConstant<double>( -1 )) , 
				    0L
				    )
				      )
		       );
    solver.AddEquation($$_f(
			    _f::Add( 
				    _f::Mul(new _arg(1), new _arg( 2 ) ) , 
				    _f::Mul(d.Clone(), new tConstant<double>( -1 ) ) , 
				    0L 
				     )
			    )
		       );
    
    double xExp = _c / ( 2 * _a ) + sqrt( rootee );
    double yExp = _d / xExp;
    double xShock = 1.0 + RandomTools::DrawUniform( -0.1 , 0.1 );
    double yShock = 1.0 + RandomTools::DrawUniform( -0.1 , 0.1 );
    solver.Open();
    BOOST_CHECK_EQUAL( true ,  solver.Solve( tnmmatrix<double>::ColumnVector( 2 , xExp * xShock , yExp * yShock ) ) );
    BOOST_CHECK_CLOSE_FRACTION( xExp , solver.GetCurrentX().Get( 1 ,1 ) , 0.0001 );
    BOOST_CHECK_CLOSE_FRACTION( yExp , solver.GetCurrentX().Get( 2 ,1 ) , 0.0001 );
    solver.Close();
    
    
  }
}

BOOST_AUTO_TEST_CASE( solveNumerical )
{

  int testCount = 100;
  for ( int i = 1 ; i <= testCount ; i++ ) {
    
    double _a = RandomTools::DrawUniform( -10 , 10 );
    double _b = RandomTools::DrawUniform( -10 , 10 );
    double _c = RandomTools::DrawUniform( -10 , 10 );
    double _d = RandomTools::DrawUniform( -10 , 10 );
    double h = RandomTools::DrawUniform( -0.001 , 0.001 );
    
    _par a( &_a );
    _par b( &_b );
    _par c( &_c );
    _par d( &_d );

    double rootee = _c * _c - 4 * _a * _b * _d;
    if ( rootee < 0 || _a == 0.0 ) {
      i--;
      continue;
    }
    rootee /= 4 * _a * _a;
    _nsolver solver;
    _intOpt::set(solver.GetOptions().get(), "maxIterations", 100);
    _doubleOpt::set(solver.GetOptions().get(), "verbosity", 0.0 );
    _doubleOpt::set(solver.GetOptions().get(), "maxF", 0.00001 );
    _matrixOpt::set(solver.GetOptions().get(), "derivativeDeltas" , _m( 1 , 1 , h ) );
    //solver.SetVerbosity( true );
    //    solver.SetDerivativeDeltas( h );
    //    solver.printX = true;
    //    solver.printF = true;
    //    solver.printdF = true;
    solver.AddChoice( "x" );
    solver.AddChoice( "y" );

    solver.AddEquation( $$_f(
			    _f::Add( 
				    _f::Mul( a.Clone() , new _arg( 1 ) ) , 
				    _f::Mul( b.Clone() , new _arg( 2 ) ) , 
				    _f::Mul( c.Clone() , new _const( -1 )) , 0L
				     )
			    )
			);
    solver.AddEquation( $$_f(
			     _f::Add( 
				     _f::Mul( new _arg( 1 ) , new _arg( 2 ) ) , 
				     _f::Mul( d.Clone() , new _const( -1 ) ) , 0L 
				      )
			     )
			);
    
    double xExp = _c / ( 2 * _a ) + sqrt( rootee );
    double yExp = _d / xExp;
    double xShock = 1.0 + RandomTools::DrawUniform( -0.1 , 0.1 );
    double yShock = 1.0 + RandomTools::DrawUniform( -0.1 , 0.1 );

    solver.Open();
    BOOST_CHECK_EQUAL( true ,  solver.Solve( tnmmatrix<double>::ColumnVector( 2 , xExp * xShock , yExp * yShock ) ) );
    BOOST_CHECK_CLOSE_FRACTION( xExp , solver.GetCurrentX().Get( 1 ,1 ) , 0.01 );
    BOOST_CHECK_CLOSE_FRACTION( yExp , solver.GetCurrentX().Get( 2 ,1 ) , 0.01 );
    solver.Close();
    
    
  }

}


BOOST_AUTO_TEST_CASE( intervalStepper1 )
{
  double rootee = -1;
  while (rootee < 0 ) {
    double _a = RandomTools::DrawUniform( -10 , 10 );
    double _b = RandomTools::DrawUniform( -10 , 10 );
    double _c = RandomTools::DrawUniform( -10 , 10 );
    double _d = RandomTools::DrawUniform( -10 , 10 );
    
    _par a( &_a );
    _par b( &_b );
    _par c( &_c );
    _par d( &_d );
    
    rootee = _c * _c - 4 * _a * _b * _d;
    if ( rootee < 0 || _a == 0.0 ) {
      continue;
    }
    rootee /= 4 * _a * _a;

  _nsolver solver;
  
  _intOpt::set(solver.GetOptions().get(), "maxIterations", 200);
  _doubleOpt::set(solver.GetOptions().get(), "verbosity", 0.0 );
  _doubleOpt::set(solver.GetOptions().get(), "maxF", 0.00001 );
  $$_f eq1($$_f(	      
			  _f::Add(
				  _f::Mul( a.Clone(), new _arg( 1 ) ) , 
				  _f::Mul( b.Clone(), new _arg( 2 ) ) , 
				  _f::Mul( c.Clone(), new tConstant<double>( -1 )) , 
				  0L
				  )
				      )
		     );
  $$_f eq2($$_f(
			  _f::Add( 
				  _f::Mul(new _arg(1), new _arg( 2 ) ) , 
				  _f::Mul(d.Clone(), new tConstant<double>( -1 ) ) , 
				  0L 
				   )
		)
	   );

  double xExp = _c / ( 2 * _a ) + sqrt( rootee );
  double yExp = _d / xExp;
  double xShock = 1.0 + RandomTools::DrawUniform( -0.1 , 0.1 );
  double yShock = 1.0 + RandomTools::DrawUniform( -0.1 , 0.1 );

  $intervalStepper stepper(new _intervalStepper(1.0));
  $_stepperOpt::set(solver.GetOptions().get(), "stepper", dynamic_pointer_cast<_stepper>(stepper)); 
  solver.AddChoice( "x" );
  solver.AddChoice( "y" );
  solver.AddEquation($$_f(eq1->Clone()));
  solver.AddEquation($$_f(eq2->Clone()));
  solver.Open();
  BOOST_CHECK_EQUAL( true ,  solver.Solve( tnmmatrix<double>::ColumnVector( 2 , xExp * xShock , yExp * yShock ) ) );
  BOOST_CHECK_CLOSE_FRACTION( xExp , solver.GetCurrentX().Get( 1 ,1 ) , 0.0001 );
  BOOST_CHECK_CLOSE_FRACTION( yExp , solver.GetCurrentX().Get( 2 ,1 ) , 0.0001 );
  BOOST_CHECK_GT( 100 , solver.GetCurrentIteration() );
  solver.Close();

  stepper.reset(new _intervalStepper(0.0));
  stepper->AddIterFractionPair(99, 0.0);
  stepper->AddIterFractionPair(100, 1.0);
  solver.AddChoice( "x" );
  solver.AddChoice( "y" );
  solver.AddEquation($$_f(eq1->Clone()));
  solver.AddEquation($$_f(eq2->Clone()));
  $_stepperOpt::set(solver.GetOptions().get(), "stepper", dynamic_pointer_cast<_stepper>(stepper)); 
  solver.Open();
  BOOST_CHECK_EQUAL( true ,  solver.Solve( tnmmatrix<double>::ColumnVector( 2 , xExp * xShock , yExp * yShock ) ) );
  BOOST_CHECK_CLOSE_FRACTION( xExp , solver.GetCurrentX().Get( 1 ,1 ) , 0.0001 );
  BOOST_CHECK_CLOSE_FRACTION( yExp , solver.GetCurrentX().Get( 2 ,1 ) , 0.0001 );
  BOOST_CHECK_LT( 100 , solver.GetCurrentIteration() );
  solver.Close();
  }
  
}

