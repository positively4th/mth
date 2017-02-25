/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#include "tnsolver.h"
#include <tstepper.h>

#include <debug.h>
#include <time.h>
#include <iomanip>
#include <algorithm>
#include <tmathtools.h>
#include <tfmatrix.h>
#include <stlstringtools.h>
#include <tnmmatrix.h>
#include <tptroption.h>
#include <signal.h>
#include <cstdlib>
#include <exception>

namespace P4th
{
  
  template<class TYPE>
  tNSolver<TYPE>::tNSolver() : 
    equations( new _fs ),  
    F( (_fm0 *)NULL ) , 
    dF( (_fm0 *)NULL ) , 
    options(new Options())
  {
    ResetOptions();
  }
  
  template<class TYPE>
  tNSolver<TYPE>::~tNSolver()
  {
  }

  
  template<class TYPE>
  Options *tNSolver<TYPE>::ResetOptions()
  {
    //Smallest acceptable determinant 
    _typeOpt::set(this->GetOptions().get(), "minDeterminant" , 0.0);
    // Do not terminate unless the error is less then maxF  
    _typeOpt::set(this->GetOptions().get(), "maxF" , -1);
    // Allow at most maxIterationns
    _intOpt::set(this->GetOptions().get(), "maxIterations" , -1);
    // Print numbers with at moste scalarWidth charcters
    _intOpt::set(this->GetOptions().get(), "scalarWidth" , 8);
    // Print numbers with at moste scalarWidth charcters

    //Probability of being verbose in a single iteration 
    _typeOpt::set(this->GetOptions().get(), "verbosity" , 0.0);
    //If verbose, print x
    _boolOpt::set(this->GetOptions().get(), "printX" , false );
    //If printIteration, print header at start of each iteration
    _boolOpt::set(this->GetOptions().get(), "printIteration" , false );
    //If verbose, print F (the error)
    _boolOpt::set(this->GetOptions().get(), "printF" , false );
    //If verbose, print dF
    _boolOpt::set(this->GetOptions().get(), "printdF" , false );
    //If verbose, print det(F)
    _boolOpt::set(this->GetOptions().get(), "printDeterminant" , false );
    //The change in x when computing dF, (0.0 change implies analytical derivative)
    _matrixOpt::set(this->GetOptions().get(), "derivativeDeltas" , _m( 1 , 1 , 0.0 ) );
    //Simplify equations and derivatives before iterating
    _boolOpt::set(this->GetOptions().get(), "simplify" , false );
    
    $_stepperOpt::set(opts(), "stepper",  $_stepper(new _stepper()));
    
  }

  template<class TYPE>
  void tNSolver<TYPE>::InitF()
  {

    F.reset( new _fm( GetN() , 1 ) );
    for ( int i = 1 ; i <= GetN() ; i++ ) {
      F->Set( i , 1 , equations->Get( i )->Clone() );
    }
    currentF.Resize( GetN() , 1 );

  }

  template<class TYPE>
  void tNSolver<TYPE>::InitdF()
  {
    assert( F.get() != NULL );

    dF.reset( _fm::ColumnDerivatives( F.get() , _matrixOpt::read(opts(), "derivativeDeltas") ) ); 
    currentdF.Resize( GetN() , GetN() );
    currentJ.Resize( GetN() , GetN() );
  }

  template<class TYPE>
  tnmmatrix<TYPE> tNSolver<TYPE>::Step( const _m &x ) const
  {
    return _m( 1, 1 );
  }

  template<class TYPE>
  void tNSolver<TYPE>::AddChoice( string name ) {
    choices.push_back( name );
  }

  template<class TYPE>
  void tNSolver<TYPE>::AddEquation( unique_ptr<_f> aEquation , string name )
  {
    equations->AddFunction( aEquation.release() );
    if ( name != "" ) 
      equationNames.push_back( name );
    else
      equationNames.push_back( "equation_" + STLStringTools::AsString( equations->GetDimension() ) );
  }

  template<class TYPE>
  string tNSolver<TYPE>::TrimScalar( TYPE scalar , int width)
  {
    return STLStringTools::AsString( (TYPE)scalar ).substr( 0 , width );
  }

  template<class TYPE>
  void tNSolver<TYPE>::PrintRowVector(std::ostream &dest, const vector<string> &labels, const tnmmatrix<TYPE> &vec, int width)
  {
    for ( vector<string>::const_iterator it = labels.begin() ; it != labels.end() ; it++ ) {
      std::cout << setw(width) 
		<< it->substr(0, width).c_str();
      
    }
    std::cout << std::endl;
    for ( int c = 1 ; c <= vec.GetRows()  ; c++  ) {
      std::cout << setw(width) 
		<< tMathTools<TYPE>::PrettyNumber( 
					    vec.Get( c , 1 ) ).c_str();
      
    }
    std::cout << std::endl;
  }

  template<class TYPE>
  void tNSolver<TYPE>::PrintCurrentX(const tNSolver &solver)
  {
    return tNSolver<TYPE>::PrintRowVector(std::cout, solver.choices, solver.GetCurrentX(), 
					  _intOpt::read(solver.opts(), "scalarWidth"));
  }
  
  template<class TYPE>
  void tNSolver<TYPE>::PrintCurrentF(const tNSolver &solver )
  {
    return tNSolver<TYPE>::PrintRowVector(std::cout, solver.choices, solver.GetCurrentF(), 
					  _intOpt::read(solver.opts(), "scalarWidth"));
  }
  
  template<class TYPE>
  void tNSolver<TYPE>::PrintCurrentdF(const tNSolver &solver )
  {
    for ( int c = 1 ; c <= solver.currentdF.GetRows()  ; c++  ) {
      for ( int r = 1 ; r <= solver.currentdF.GetCols()  ; r++  ) {
	std::cout << setw(_intOpt::read(solver.opts(), "scalarWidth")) 
		  << tMathTools<TYPE>::PrettyNumber( solver.GetCurrentdF().Get( r , c ) ).c_str();
      }
      std::cout << std::endl;
    }
  }

  template<class TYPE>
  bool tNSolver<TYPE>::AcceptGuess( const tNSolver &solver )
  {
    return solver.GetCurrentF().Nrm() < _typeOpt::read(solver.opts(), "maxF");
  }

  template<class TYPE>
  bool tNSolver<TYPE>::TryNextGuess( const tNSolver &solver )
  {
    return solver.GetCurrentIteration() < _intOpt::read(solver.opts(), "maxIterations");
  }


  template<class TYPE>
  void tNSolver<TYPE>::Open()
  {
    this->EnsureChoices();
    InitF();
    InitdF();
    if ( _boolOpt::read(opts(), "simplify") ) {
      if ( _typeOpt::read(opts(), "verbosity") > 0.0 )
	std::cout << "Simplifying F from " << F->Nodes();
      F.reset( F.release()->Simplify() );
      if ( _typeOpt::read(opts(), "verbosity") > 0.0 )
	std::cout << " to " << F->Nodes() << " nodes." << std::endl;
      
      if ( _typeOpt::read(opts(), "verbosity") > 0.0 )
	std::cout << "Simplifying dF from " << dF->Nodes();
      dF.reset( dF.release()->Simplify() );
      if ( _typeOpt::read(opts(), "verbosity") > 0.0 )
	std::cout << " to " << dF->Nodes() << " nodes." << std::endl;
    }
    $_stepperOpt::read(opts(), "stepper").get()->Open( this );
  }

  template<class TYPE>
  void tNSolver<TYPE>::Close()
  {
    $_stepperOpt::read(opts(), "stepper").get()->Close( this );
    choices.clear();
    equations.reset(new _fs());
    equationNames.clear();
    F.reset( NULL );
    dF.reset( NULL );
  }

  template<class TYPE>
  void tNSolver<TYPE>::EnsureChoices()
  {
    while(choices.size() < equations->GetSize()) {
      this->AddChoice("x" + STLStringTools::AsString((int)(choices.size() + 1)));
    }
  }

  template<class TYPE>
  bool tNSolver<TYPE>::Solve( const _m &x )
  {
    
    TYPE minDeterminant = _typeOpt::read(opts(), "minDeterminant");
    $_stepper stepper($_stepperOpt::read(opts(), "stepper"));
    assert( x.GetCols() == 1 );
    assert( equations->GetSize() == choices.size() );
    assert( x.GetRows() == choices.size() );
    assert( _typeOpt::read(opts(), "maxF") > 0 );
    assert( _intOpt::read(opts(), "maxIterations") > 0 );
    currentIteration = 0;
    currentX.SafeCopy( x );
    _m nextX( currentX.GetRows() , currentX.GetCols() );
    TYPE currentReciprocalCond;
    try {
      do {


	verboseIteration = (double)(rand() % 1000) < 1000.0 * _typeOpt::read(opts(), "verbosity");

	if ( verboseIteration && _boolOpt::read(opts(), "printIteration") ) {
	  std::cout << "starting itertaion " << currentIteration << std::endl; 
	}

	if ( verboseIteration && _boolOpt::read(opts(), "printX") ) {
	  PrintCurrentX( *this );
	}

	F->y( currentF , currentX );
	
	if ( verboseIteration && _boolOpt::read(opts(), "printF") ) {
	  PrintCurrentF( *this );
	}
	
	if ( AcceptGuess( *this ) )
	  return true;

	if ( not TryNextGuess( *this ) ) 
	  return false;

	dF->y( currentdF , currentX );
	
	if ( verboseIteration && _boolOpt::read(opts(), "printdF") ) {
	  PrintCurrentdF( *this );
	}
	
	currentDeterminant = currentdF.Det();
	if ( verboseIteration && _boolOpt::read(opts(), "printDeterminant"))
	  std::cout << "|dF| = " << currentDeterminant << std::endl;

	if ( (minDeterminant > 0) && (abs(currentDeterminant) < minDeterminant) )
	  return false;
	
	currentdF.Inv( &currentJ , &currentReciprocalCond );
	
	nextX.Mul( currentJ , currentF );
	nextX.Sub( currentX );
	nextX.Mul( -1 );
	
	stepper->Step( this , currentX , nextX );

	currentIteration++;

      } while ( true );
    }
    catch ( const std::exception &e ) {
      std::cerr << "Catched exception: " << e.what() << std::endl;
    }
    catch (...) {
      std::cerr << "Catched exception!" << std::endl << std::flush;
    }
    
    return false;
  }
}

template class P4th::tNSolver<float>;
template class P4th::tNSolver<double>;

