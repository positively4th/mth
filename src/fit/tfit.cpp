#include "tfit.h"

#include <iostream>
#include <debug.h>
#include <math.h>
#include <stlstringtools.h>
#include <stdlib.h>
#include <assert.h>
#include <texception.h>
#include <iomanip>
#include <tmathtools.h>
#include <tfunctions.h>
#include <tselector.h>
#include <twasher0.h>
#include <tnowasher.h>
#include <tols.h>


namespace P4th
{

  using namespace Fit;

  template<class TYPE> 
  tFit<TYPE>::tFit( int _M , int _K , int _P) :
    X(NULL), Y(NULL), B(NULL) , EY(NULL) , SST(NULL) , SSE(NULL) , MST(NULL) , MSE(NULL) , R2(NULL) , R2Adj(NULL) , 
    options(new Options())
  {
    M = _M;
    K = _K;
    P = _P >= 0 ? _P : _M;

    this->ResetOptions();
  }

  template<class TYPE> 
  void tFit<TYPE>::ResetOptions() {
    $_washerOpt::set(GetOptions().get(), "washer" , new _noWasher(this));
  }


  template<class TYPE> 
  void tFit<TYPE>::AddObservation( const _m &Y , const _m &X )
  {
#ifdef _SAFE
    if ( X.GetRows() != Y.GetRows()  )
      throw (string)"void tFit<TYPE>::AddObservation( tnmmatrix<double> Y , const _m &X ) : X.GetRows() != Y.GetRows()";
    if ( X.GetCols() != M )
      throw (string)"void tFit<TYPE>::AddObservation( tnmmatrix<double> Y , const _m &X ) : X.GetCols() != M";
    if ( Y.GetCols() != K )
      throw (string)"void tFit<TYPE>::AddObservation( tnmmatrix<double> Y , const _m &X ) : Y.GetCols() != K";
#endif

    _m xrow(1 , GetM() ); 
    _m yrow(1 , GetK() ); 
    for ( int i = 1 ; i <= X.GetRows() ; i++ ) {
      _m::Get( xrow , X , i , 1 );
      _m::Get( yrow , Y , i , 1 );
      xs.push_back( xrow );
      ys.push_back( yrow );
    }
  }



  template<class TYPE> 
  map<string,double> tFit<TYPE>::Getb( int k )
  {
    if ( k < 1 || k > K )
      throw aException(tFit<TYPE>, "Getb() const : k < 1 || k > K" );
    
    map<string,double> b;
    $_m B( GetB() );
    for ( int i = 0 ; i < xnames.size() ; i++ )
      b.insert( pair<string,double>( xnames[i] , (*GetB())(i+1,k) ) );
    return b;
  }

  template<class TYPE> 
  void tFit<TYPE>::AddxName( string names ) // separated by ","
  {
    xnames.push_back( names );
  }

  template<class TYPE> 
  void tFit<TYPE>::AddxNames( string names ) // separated by ","
  {
    int i;
    while ( names.size() > 0 )
      {
	//	_DB( std::cout << names << std::endl; );
	i = STLStringTools::Index( names , "," );
	if ( i < 0 )
	  {
	    //	    xnames.push_back( names );
	    this->AddxName(names);
	    names = (string)"";
	  } else {
	  //	  xnames.push_back( STLStringTools::LeftOfExc( names, i  ) );
	  this->AddxName(STLStringTools::LeftOfExc( names, i  ));
	  names = STLStringTools::RightOfInc( names , i + 1 );
	}
      }
    //    _DB( std::cout << xnames.GetSize() << std::endl; );
  }


  template<class TYPE> 
  void tFit<TYPE>::AddyNames( string names ) // separated by ","
  {
    int i;
    while ( names.size() > 0 )
      {
	//	_DB( std::cout << names << std::endl; );
	i = STLStringTools::Index( names , "," );
	if ( i < 0 )
	  {
	    ynames.push_back( names );
	    names = (string)"";
	  } else {
	  ynames.push_back( STLStringTools::LeftOfExc( names, i  ) );
	  names = STLStringTools::RightOfInc( names , i + 1 );
	}
      }
    //    _DB( std::cout << xnames.GetSize() << std::endl; );
  }

  template<class TYPE> 
  shared_ptr<typename tFit<TYPE>::_m> tFit<TYPE>::Getmu()
  {
    assert( GetN() > 0 );
    if (!mu) {
      mu.reset(new _m(1,GetM() + GetK()));
      _m x(1,GetM(),0.0);
      _m y(1,GetK(),0.0);
      for ( int i = 0 ; i < GetN() ; i++ )
	{
	  y.Add( ys[i] );
	  x.Add( xs[i] );
	}
      y.Mul( 1.0 / (TYPE)GetN() );
      x.Mul( 1.0 / (TYPE)GetN() );
      
    _m::Set( *mu , y , 1 , 1 );
    _m::Set( *mu , x , 1 , 1 + GetK() );

    }
    return mu;
  }

  

  template<class TYPE> 
  shared_ptr<typename tFit<TYPE>::_m> tFit<TYPE>::Getsigma()
  {
    if (!sigma) {
      assert( GetN() > 0 );
      _m row(1,GetM() + GetK());
      _m col(GetM() + GetK(),1);
      _m tmp(GetM() + GetK(),GetM() + GetK());
      sigma.reset(new _m(GetM() + GetK(),GetM() + GetK(),0.0));
      
      if ( GetN() < 1 )
	throw aException( tFit<TYPE>, "sigma() const : GetN() < 1" );
      for ( int i = 0 ; i < GetN() ; i++ )
	{
	  _m::Set( row , ys[i] , 1 , 1 );
	  _m::Set( row , xs[i] , 1 , 1 + GetK() );
	  row.Sub( *mu );
	  row.Trp( &col );
	  tmp.Mul( col , row );
	  sigma->Add( tmp );
	}
      sigma->Mul( 1.0 / GetN() );
    }
    return sigma;
  }

  template<class TYPE> 
  shared_ptr<typename tFit<TYPE>::_m> tFit<TYPE>::Getrho()
  {
    if ( GetN() < 1 )
      aException( tFit<TYPE>, "rho() : GetN() < 1" );
    if (!rho) {
      rho.reset(new _m(Getsigma()->GetRows(),Getsigma()->GetCols() , 0.0 ));
      
      for ( int r = 1 ; r <= rho->GetRows() ; r++ )
	for ( int c = 1 ; c <= r ; c++ )
	  {
	    if ( sigma->Get(r,r) != (TYPE)0.0 && sigma->Get(c,c) != (TYPE)0.0 ) 
	      {
		assert( sigma->Get(r,r) * sigma->Get(c,c) >= 0 );
		(*rho)(r,c) = sigma->Get(r,c) / sqrt( sigma->Get(r,r) * sigma->Get(c,c) );
		(*rho)(c,r) = (*rho)(r,c);
	      } else {
	      (*rho)(r,c) = 0.0;
	      (*rho)(c,r) = 0.0;
	    }
	  }
    }
    return rho;
  }

  template<class TYPE> 
  shared_ptr<typename tFit<TYPE>::_m> tFit<TYPE>::GetX() 
  {
    shared_ptr<_washer0> washer = $_washerOpt::read(GetOptions().get(), "washer");
    assert(washer);
    return washer->GetX();
  }

  template<class TYPE> 
  shared_ptr<typename tFit<TYPE>::_m> tFit<TYPE>::GetY() 
  {
    shared_ptr<_washer0> washer = $_washerOpt::read(GetOptions().get(), "washer");
      assert(washer);
      return washer->GetY();
  }

  template<class TYPE> 
  shared_ptr<string> tFit<TYPE>::GetRegressorMask() 
  {
    shared_ptr<_washer0> washer = $_washerOpt::read(GetOptions().get(), "washer");
    assert(washer);
    return washer->GetRegressorMask();
  }

  template<class TYPE> 
  shared_ptr<string> tFit<TYPE>::GetObservationMask() 
  {
    shared_ptr<_washer0> washer = $_washerOpt::read(GetOptions().get(), "washer");
      assert(washer);
      return washer->GetObservationMask();
  }

  template<class TYPE> 
  shared_ptr<typename tFit<TYPE>::_m> tFit<TYPE>::GetB()
  {
    shared_ptr<_washer0> washer = $_washerOpt::read(GetOptions().get(), "washer");
    if (!B) {
      $_estimator0 estimator = GetEstimator();;
      estimator->GetB()->Print(std::cout, "B:\t", 80);

      washer->unWashB(estimator->GetB().get()).release()->Print(std::cout, "B:\t", 80);
      B = $_m(washer->unWashB(estimator->GetB().get()).release());
      assert(B);
    }
    return B;
  }

  template<class TYPE> 
  shared_ptr<typename tFit<TYPE>::_m> tFit<TYPE>::GetEY() 
  {
    if (!EY) {
      EY.reset(new _m(1, GetK(), 0.0));
      _m *_EY = EY.get();
      for ( int i = 0 ; i < ys.size() ; i++ ) {
	if ( (*GetObservationMask())[i] != '0' )
	  _EY->Add( ys[i] );
      }
      assert( (TYPE)STLStringTools::Count( (*this->GetObservationMask()) , '1' ) > 0 );
      _EY->Mul( 1.0 / (TYPE)STLStringTools::Count( (*this->GetObservationMask()) , '1' ) );

    }
    return EY;
  }
  
  template<class TYPE> 
  shared_ptr<typename tFit<TYPE>::_m> tFit<TYPE>::GetSST() 
  {
    if (!SST) {
      SST.reset(new _m(1, GetK(), 0.0));
      _m *_SST = SST.get();
      for ( int i = 0 ; i < ys.size() ; i++ )
	if ( (*this->GetObservationMask())[i] != '0' )
	  for( int k = 1 ; k <= GetK() ; k++ ) 
	    _SST->Set( 1 , k , SST->Get( 1 , k ) + pow( ys[i].Get(1,k) - GetEY()->Get(1,k) , 2 ) );
    }
    return SST;
  }

  template<class TYPE> 
  shared_ptr<typename tFit<TYPE>::_m> tFit<TYPE>::GetSSE() 
  {
    if (!SSE) {
      _m predictions = this->GetPredictions();
      SSE.reset(new _m(1, GetK(), 0.0));
      for ( int i = 0 ; i < ys.size() ; i++ )
	if ( (*this->GetObservationMask())[i] != '0' )
	  for ( int k = 1 ; k <= GetK() ; k++ )
	    SSE->Set(1 , k , SSE->Get( 1 , k ) + pow( ys[i].Get(1,k) - predictions.Get(i+1,k) , 2 ) );
    }
    return SSE;
  }

  template<class TYPE> 
  shared_ptr<typename tFit<TYPE>::_m> tFit<TYPE>::GetMST() 
  {
    if ( ys.size() < 2 )
      throw aException(tFit<TYPE>,  "GetMST() : ys.size() < 2" );

    if (!MST) {
      assert( STLStringTools::Count( (*this->GetObservationMask()) , '1' ) - 1 > 0 );
      MST.reset(new _m(1, GetK()));
      MST->Mul( 1.0 / (TYPE)( STLStringTools::Count( (*this->GetObservationMask()) , '1' ) - 1 ) , *GetSST() );
    }
    return MST;
  }

  template<class TYPE> 
  shared_ptr<typename tFit<TYPE>::_m> tFit<TYPE>::GetMSE() 
  {
    unsigned int regressors = GetRegressors();
    if ( ys.size() + regressors < 2 )
      throw aException(tFit<TYPE>, "GetMSE() : ys.size() + regressors < 2" );
    if (!MSE) {
      MSE.reset(new _m(1, GetK()));
      assert( STLStringTools::Count( (*this->GetObservationMask()) , '1' ) - regressors - 1 > 0 );
      MSE->Mul( 1.0 / (double)( STLStringTools::Count( (*this->GetObservationMask()) , '1' ) - regressors - 1 ), *GetSSE() );
    }
    return MSE;
  }
  
  template<class TYPE> 
  shared_ptr<typename tFit<TYPE>::_m> tFit<TYPE>::GetR2() 
  {
    if (!R2) {
      R2.reset(new _m(1,GetK()));
      for ( int k = 1 ; k <= GetK() ; k++ )
	if ( GetSST()->Get(1,k) > 0 )
	  R2->Set( 1 , k , 1 - GetSSE()->Get(1,k) / GetSST()->Get(1,k) );
	else
	  R2->Set( 1 , k , NAN );
    }
    return R2;
  }

  template<class TYPE> 
  shared_ptr<typename tFit<TYPE>::_m> tFit<TYPE>::GetR2Adj() 
  {
    if (!R2Adj) {
      R2Adj.reset(new _m(1,GetK()));
      for ( int k = 1 ; k <= GetK() ; k++ )
	if ( GetMST()->Get(1,k) > 0 )
	  R2Adj->Set(1,k , 1 - GetMSE()->Get(1,k) / GetMST()->Get(1,k) );
	else
	  R2Adj->Set(1,k , NAN );
    }
    return R2Adj;
  }
 

  template<class TYPE> 
  shared_ptr<tFunctions<TYPE> > tFit<TYPE>::GetPredictors() 
  {
    shared_ptr<_washer0> washer = $_washerOpt::read(GetOptions().get(), "washer");
    if (!this->predictors) {
      $_estimator0 estimator = this->GetEstimator();
      $_fs washedPredictors = estimator->GetPredictors();
      predictors = washer->unWash(washedPredictors.get());
      assert(predictors);
    }
    return predictors;
  }

  template<class TYPE> 
  shared_ptr<tFunction0<TYPE> > tFit<TYPE>::GetPredictor(int ith)
  {
    //Todo: Try to avoid needing to clone....
    return $_f(this->GetPredictors()->Get(ith)->Clone());
  } 


  template<class TYPE> 
  tnmmatrix<TYPE> tFit<TYPE>::GetPredictions() 
  {
    if (!predictions) {
      _fs *predictors = GetPredictors().get(); 
      predictions.reset(new _m(xs.size(), GetK()));
      int row = 1;
      for( _mVecee xt = xs.begin(); xt != xs.end(); xt++) {
	_m::Set(*predictions, predictors->y(xt->Trp()).Trp(), row++ ,1 );
      } 
    }
    return *predictions;
  }


  template<class TYPE> 
  std::ostream &tFit<TYPE>::PrintObservations( std::ostream &dest , string lm , int width )
  {
    for ( int i = 0 ; i < ys.size() ; i++ )
      if ( i < ynames.size() )
	dest << lm << std::setw( width ) << (const char *)"X";
      else
	dest << lm << std::setw( width ) << STLStringTools::LeftOfExc( GetyName( i ) , width - 1 ).c_str();
    dest << " =";
    for ( int i = 0 ; i < xs.size() ; i++ )
      if ( i < xnames.size() )
	dest << lm << std::setw( width ) << (const char *)"X";
      else
	dest << lm << std::setw( width ) << STLStringTools::LeftOfExc( GetxName( i ) , width - 1 ).c_str();
    dest << std::endl;
    
    for ( int i = 0 ; i < GetN() ; i++ )
      {
	_m y( ys[i] ); 
	for ( int k = 0 ; k < GetK() ; k++ )
	  {
	    dest << std::setw( width ) << 
	      STLStringTools::LeftOfExc( STLStringTools::AsString( y.Get(1,k + 1) , 10 , '.' ) , width - 1 ).c_str();
	  }
	_m x( xs[i] ); 
	dest << " =";
	for ( int j = 0 ; j < GetM() ; j++ )
	  {
	    dest << std::setw( width ) << 
	      STLStringTools::LeftOfExc( STLStringTools::AsString( x.Get(1,j + 1) , 10 , '.' ) , width - 1 ).c_str();
	  }
	dest << std::endl;
      }
    return dest;
  }

  template<class TYPE> 
  std::ostream &tFit<TYPE>::PrintMu( const _m &mu , std::ostream &dest , string lm , int width )
  {
    for ( int i = 0 ; i < ynames.size() ; i++ )
      dest << lm << std::setw( width ) <<STLStringTools::LeftOfExc( GetyName(i) , width - 1 ).c_str();
    for ( int i = 0 ; i < xnames.size() ; i++ )
      dest << std::setw( width ) << STLStringTools::LeftOfExc( GetxName(i) , width - 1 ).c_str();
    dest << std::endl;
    
    dest << lm;
    for ( int j = 1 ; j <= mu.GetCols() ; j++ )
      {
	dest << std::setw( width ) << 
	  STLStringTools::LeftOfExc( STLStringTools::AsString( mu.Get(1,j) , 10 , '.' ) , width - 1 ).c_str();
      }
    dest << std::endl;
    
    return dest;
  }


  template<class TYPE> 
  std::ostream &tFit<TYPE>::PrintEstimate( std::ostream &dest , string lm , int width )
  {
    $_estimator0 estimator = $_estimatorOpt::read(GetOptions().get(), "estimator");

    typename _estimator0::_miscMap misc = estimator->GetMisc();
    typename _estimator0::_miscMap::iterator bCovsIt = misc.find("bCov");
    bool useBCovs = bCovsIt != misc.end();


    dest << lm << std::setw( 4 ) << (const char *)"i";
    dest << std::setw( width ) << STLStringTools::LeftOfExc( "Explanatory" , width - 1 ).c_str() << " = ";
    for ( int i = 0 ; i < ynames.size() ; i++ )
      dest << std::setw( 4 ) << std::setw( width ) << STLStringTools::LeftOfExc( GetyName(i) , width - 1 ).c_str();
    dest << std::endl;

    int jj = 0;
    for ( int j = 0 ; j < GetP() ; j++ )
      {
	if ( (*this->GetRegressorMask())[j] == '0' )
	  continue;
	dest << lm << std::setw( 4 ) << STLStringTools::AsString( j+1 ).c_str();
	dest << std::setw( width ) << STLStringTools::LeftOfExc( GetxName(j) , width - 1 ).c_str() << " = ";
	for( int k = 0 ; k < GetK() ; k++ )
	  dest << std::setw( width ) << 
	    STLStringTools::LeftOfExc( STLStringTools::AsString( GetB()->Get(j + 1,k + 1) , 10 , '.' ) , width - 1 ).c_str();
	dest << std::endl;

	if (useBCovs) {
	  dest << lm << std::setw( 4 ) << " ";
	  dest << std::setw( width ) << " " << "   ";
	  for( int k = 0 ; k < GetK() ; k++ ) {
	    TYPE s = (bCovsIt->second)[k].Get( jj + 1 , jj + 1 );
	    TYPE b = GetB()->Get(j + 1,k + 1);
	    assert( s >= 0);
	    dest << std::setw( width - 1 ) << 
	      STLStringTools::LeftOfExc( (string)"(" + STLStringTools::AsString( b / sqrt(s) , 10 , '.' ) , width - 2 ).c_str()
		 << ")";
	  }
	  dest << std::endl;
	}
	jj++;
      }
    dest << std::endl;
    return dest;
  }

  template<class TYPE> 
  std::ostream &tFit<TYPE>::PrintSummary( std::ostream &dest , string lm , int width )
  {
    int regressors = GetRegressors();

    dest << lm 
	 << std::setw( width ) << STLStringTools::LeftOfExc( "Statistics" , width - 1 );
    for ( int k = 0 ; k < GetK() ; k++ )
      dest << std::setw( width ) 
	   << STLStringTools::LeftOfExc( GetyName(k) , width - 1 ).c_str(); 
    dest << std::endl;
    
    dest << lm 
	 << std::setw( width ) << STLStringTools::LeftOfExc( "EY =" , width -1 );
    for ( int k = 1 ; k <= GetK() ; k++ )
      dest << std::setw( width ) 
	   << STLStringTools::LeftOfExc( STLStringTools::AsString( GetEY()->Get(1,k) , 10 , '.' ) , width - 1 ); 
    dest << std::endl;

    dest << lm 
	 << std::setw( width ) << STLStringTools::LeftOfExc( "SST = " , width -1 );
    for ( int k = 1 ; k <= GetK() ; k++ )
      dest << std::setw( width ) << STLStringTools::LeftOfExc( STLStringTools::AsString( GetSST()->Get(1,k) , 10 , '.' ) , width - 1 );
    dest << std::endl;

    dest << std::setw( width ) << "SSE = ";
    for ( int k = 1 ; k <= GetK() ; k++ )
      dest << std::setw( width ) << STLStringTools::LeftOfExc( STLStringTools::AsString( GetSSE()->Get(1,k) , 10 , '.' ) , width - 1 ); 
    dest << std::endl;

    dest << lm 
	 << std::setw( width ) << STLStringTools::LeftOfExc( "MST = " , width -1 );
    for ( int k = 1 ; k <= GetK() ; k++ )
      dest << std::setw( width ) << STLStringTools::LeftOfExc( STLStringTools::AsString( GetMST()->Get(1,k) , 10 , '.' ) , width - 1 ); 
    dest << std::endl;

    dest << std::setw( width ) << STLStringTools::LeftOfExc( "MSE = " , width -1 ); 
    for ( int k = 1 ; k <= GetK() ; k++ )
      dest << std::setw( width ) << STLStringTools::LeftOfExc( STLStringTools::AsString( GetMSE()->Get(1,k) , 10 , '.' ) , width - 1 ); 
    dest << std::endl;
    
    dest << lm 
	 << std::setw( width ) << STLStringTools::LeftOfExc( "R2 = " , width -1 );
    for ( int k = 1 ; k <= GetK() ; k++ )
      dest << std::setw( width ) << STLStringTools::LeftOfExc( STLStringTools::AsString( GetR2()->Get(1,k) , 10 , '.' ) , width - 1 );
    dest << std::endl;
  
    dest << std::setw( width ) << STLStringTools::LeftOfExc( "R2Adj = " , width -1 );
    for ( int k = 1 ; k <= GetK() ; k++ )
      dest << std::setw( width ) << STLStringTools::LeftOfExc( STLStringTools::AsString( GetR2Adj()->Get(1,k) , 10 , '.' ) , width - 1 );
    dest << std::endl;
    return dest;
  }
  
}


template class ::P4th::tFit<float>;
template class ::P4th::tFit<double>;






