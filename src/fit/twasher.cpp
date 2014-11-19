#include "twasher.h"


#include <vector>
#include <map>
#include <debug.h>
#include <tnmmatrix.h>
#include <tmathtools.h>
#include <stlstringtools.h>
#include <string>
#include <toption.h>

namespace P4th {

  namespace Fit {
    
  template<class TYPE>
  tWasher<TYPE>::tWasher(tFit<TYPE> *aEstimator) : 
    tWasher0<TYPE>(aEstimator) , options(new Options()) ,
    X(NULL), Y(NULL), observationMask(NULL), regressorMask(NULL)
  {
    this->ResetOptions();
  }
    
  template<class TYPE>
  tWasher<TYPE>::tWasher(const tWasher &source) : tWasher0<TYPE>(source)
  {
    this->options = source.options;
  }
    
  template<class TYPE>
  void tWasher<TYPE>::ResetOptions() {
    _boolOpt::set(options.get(), "debug", false); 
    _boolOpt::set(options.get(), "useIntercept", true);
    _typeOpt::set(options.get(), "minMu" , 0.0000000000001);
    _typeOpt::set(options.get(), "correlationMax" , 1.0);
    _typeOpt::set(options.get(), "variationmin" , 0.0000000000000);
    _typeOpt::set(options.get(), "sigmamin" , 0.0000000000000);
    _typeOpt::set(options.get(), "stddevmax" , 0.0000000000000);
    _boolOpt::set(this->options.get(), "_DB_PRINT_OBSMASK", false);
    _boolOpt::set(this->options.get(), "_DB_PRINT_XMASK", false);
    _boolOpt::set(this->options.get(), "_DB_PRINT_DISCARDS", false);
    _boolOpt::set(this->options.get(), "_DB_PRINT_VARIATION", false);
    _boolOpt::set(this->options.get(), "_DB_PRINT_X", false);
    _boolOpt::set(this->options.get(), "_DB_PRINT_Y", false);
    _intOpt::set(this->options.get(), "maxregressors", 0);
  }


  template<class TYPE>
  shared_ptr<tnmmatrix<TYPE> > tWasher<TYPE>::GetX() 
  {
    if (!X) {
      this->CopyObservations();
    }
    return X;
  }

  template<class TYPE>
  shared_ptr<tnmmatrix<TYPE> > tWasher<TYPE>::GetY() 
  {
    if (!Y) {
      this->CopyObservations();
    }
    return Y;
  }

  template<class TYPE>
      shared_ptr<string> tWasher<TYPE>::GetRegressorMask()
  {
    if (!regressorMask) {
      this->CopyObservations();
    }
    return regressorMask;
  }

  template<class TYPE>
  shared_ptr<string> tWasher<TYPE>::GetObservationMask()
  {
    if (!observationMask) {
      this->CopyObservations();
    }
    return observationMask;
  }

  template<class TYPE>
  void tWasher<TYPE>::CopyObservations() 
  {
    bool debug = _boolOpt::read(options.get(), "debug");
    bool useIntercept = _boolOpt::read(options.get(), "useIntercept");
    TYPE _MINMU = _typeOpt::read(options.get(), "minMu");
    TYPE correlationMax = _typeOpt::read(options.get(), "correlationMax");
    TYPE variationmin = _typeOpt::read(options.get(), "variationmin");
    TYPE sigmamin = _typeOpt::read(options.get(), "sigmamin");
    TYPE stddevmax = _typeOpt::read(options.get(), "stddevmax");
    int maxregressors = _intOpt::read(options.get(), "maxregressors");


    bool _DB_PRINT_OBSMASK = _boolOpt::read(this->options.get(), "_DB_PRINT_OBSMASK");
    bool _DB_PRINT_XMASK = _boolOpt::read(this->options.get(), "_DB_PRINT_XMASK");
    bool _DB_PRINT_DISCARDS = _boolOpt::read(this->options.get(), "_DB_PRINT_DISCARDS");
    bool _DB_PRINT_VARIATION = _boolOpt::read(this->options.get(), "_DB_PRINT_VARIATION");
    bool _DB_PRINT_X = _boolOpt::read(this->options.get(), "_DB_PRINT_X");
    bool _DB_PRINT_Y = _boolOpt::read(this->options.get(), "_DB_PRINT_Y");
    int regressors;
    const vector<_m> &xs = this->GetXs();
    const vector<_m> &ys = this->GetYs();
    
    regressorMask.reset(new string(STLStringTools::Fill( this->GetM() , '0' )));
    observationMask.reset(new string(STLStringTools::Fill( this->GetN() , '1' )));

    if ( debug & _DB_PRINT_XMASK )
      std::cout << "regressorMask = " << *regressorMask << std::endl;

    if ( xs.size() < 1 )
      {
	regressors = 0;
	X.reset(new _m( this->GetN() , regressors ));
	Y.reset(new _m( this->GetN() , this->GetK()));
	return;
      }

    /*
      Compute covarince matrix
    */
    tnmmatrix<TYPE> mu(*this->Getmu());
    tnmmatrix<TYPE> sigma(*this->Getsigma());
    tnmmatrix<TYPE> rho(*this->Getrho());
    
    if ( debug & _DB_PRINT_VARIATION )
      {
	PrintVariation( sigma , std::cout , "" , 10 ); 
	PrintVariation( rho , std::cout , "" , 10 ); 
      }
    
    
    /*
      Unmask Constant Regressors
    */

    int first = 1 + this->GetK();
    int j = 0;
    if (this->GetM() > 0 && useIntercept) { 
      (*regressorMask)[0] = '1'; // Constant
      first += 1;
      j += 1;
    }
    for ( int i = first ; i <= this->GetK() + this->GetM() ; i++ , j++ ) // Skip y and constant
      {
	assert( sigma(i,i) >= 0);
	if ( sqrt(sigma(i,i)) > sigmamin || sigmamin <= 0 ) // must be to initialize regressorMask!
	  (*regressorMask)[j] = '1';
	else
	  {
	    (*regressorMask)[j] = '0';
	    if ( debug & _DB_PRINT_DISCARDS )
		std::cout << this->GetxName( i - 1 ) << " discarded since sigma = " << sigma(i,i) << " < sigmamin = " << 
		  sigmamin << std::endl;  
	  }
      }
    
    if ( debug & _DB_PRINT_XMASK )
      std::cout << "regressorMask = " << *regressorMask << std::endl;
    
    if ( variationmin > 0 ) {
      j = 1;
      for ( int i = first ; i <= this->GetK() + this->GetM() ; i++ , j++ )
	{
	  assert( sigma(i,i) >= 0);
	  if ( (*regressorMask)[j] != '0' && ( sqrt(sigma(i,i)) / tMathTools<TYPE>::Max( _MINMU , tMathTools<TYPE>::Abs( mu(1,i) ) ) > variationmin ) )
	    {
	      (*regressorMask)[j] = '1';
	    } else {
	    (*regressorMask)[j] = '0';
	    if ( debug & _DB_PRINT_DISCARDS )
	      std::cout << this->GetxName( j ) << " discarded since sigma/|mu| = " << sqrt(sigma(i,i)) / tMathTools<TYPE>::Max( _MINMU , tMathTools<TYPE>::Abs( mu(1,i) ) )  << " < variationmin = " << 
		variationmin << std::endl;  
	  }
	  
	}
    }
    if ( debug & _DB_PRINT_XMASK )
      std::cout << "regressorMask = " << *regressorMask << std::endl;

    /*

    Check against correlation max

    */

    for ( int i = 1 ; i <= this->GetM() ; i++ )
      {
	if ( (*regressorMask)[i-1] != '0' )
	  for ( int j = i + 1 ; j <= this->GetM() ; j++ )
	    if ( (*regressorMask)[j-1] != '0' )
	      if ( tMathTools<TYPE>::Abs( rho(i + this->GetK() , j + this->GetK() ) ) > correlationMax )
		{	
		  (*regressorMask)[j-1] = '0';
		  if ( debug & _DB_PRINT_DISCARDS )
		    std::cout << this->GetxName( j - 1 ) << " discarded since correlation coefficient of " 
			      << this->GetxName( j - 1 ) << " and " 
			      << this->GetxName( i - 1 ) << " is " << rho(j+ this->GetK(),i+ this->GetK()) << " > correlationMax = " << correlationMax << std::endl;  
		}
      }
    if ( debug & _DB_PRINT_XMASK )
      std::cout << "regressorMask = " << *regressorMask << std::endl;
    
    /*

    Check for outliers

    */
    
    TYPE stddev,delta;
    tnmmatrix<TYPE> xvec(1,this->GetM());
    
    if ( stddevmax > 0 )
      for ( int i = 1 ; i <= this->GetN() ; i++ )
	{
	  //	  _DB( std::cout << "Checking observation " << i - 1 << std::endl; ); 
	  xvec.tnmContainer<TYPE>::Cpy( xs[i-1] );
	  for ( int j = 2 ; j <= this->GetM() ; j++ )
	    if ( (*regressorMask)[j-1] != '0' )
	      {
		assert( sigma(j+ this->GetK() ,j+ this->GetK() ) >= 0);
		stddev = sqrt( sigma(j+ this->GetK() ,j+ this->GetK() ) );
		delta = xvec(1,j) - mu(1,j+this->GetK());
		if ( tMathTools<TYPE>::Abs( delta ) > stddev * stddevmax )  
		  {
		    (*observationMask)[i-1] = '0';
		  }
	      }
	}
  
    if ( debug & _DB_PRINT_OBSMASK )
      std::cout << "observationMask = " << observationMask << std::endl;

  /*
    
    Check degrees of freedom
    
    */

    int ctr = 0;
    for ( int i = 0 ; i < this->GetM() ; i++ )
      {
	if ( (*regressorMask)[i] == '1' )
	  ctr++;
	if ( ctr > this->GetN() )
	  (*regressorMask)[i] = '0';
      }
    
    /*
      Check against maxregressors
    */

    regressors = STLStringTools::Count( *regressorMask , '1' );
    int observations = STLStringTools::Count( *observationMask , '1' );
    if ( maxregressors > 0 && regressors > maxregressors )
      regressors = maxregressors;
    
    _DB( std::cout << "Using " << regressors << " regressors" << std::endl; ); 
    _DB( std::cout << "Using " << observations << " observations" << std::endl; ); 

    if ( debug & _DB_PRINT_XMASK )
      std::cout << "regressorMask = " << *regressorMask << std::endl;
    /* 
       Copy regressors
    */

    //    std::cout << "obsmask = " << *observationMask << std::endl << std::flush;
    Y.reset(new _m( observations , this->GetK()));
    X.reset(new _m( observations , regressors ));
    X->tnmContainer<TYPE>::Cpy(0.0);
    int obsctr = 0;
    for ( int i = 0 ; i < this->GetN() ; i++ )
      {
	if ( (*observationMask)[i] == '1' )
	  {
	    obsctr++;

	    tnmmatrix<TYPE>::Set( *Y , ys[i] , obsctr ,1 );
	    tnmmatrix<TYPE> xvec( xs[i] );
	    ctr = 1;
	    for ( int j = 0 ; j < this->GetM() ; j++ ) 
	      if ( (*regressorMask)[j] != '0' && ctr <= regressors )
		{
		  (*X.get())(obsctr,ctr) = xvec(1,j+1);
		  ctr++;
		} 
	  }
      }

    if ( debug & _DB_PRINT_Y )
      Y->Print( std::cout , "Y:\t" , 120 );
    
    if ( debug & _DB_PRINT_X )
      X->Print( std::cout , "X:\t" , 120 );

   
  }

  template<class TYPE> 
  std::ostream &tWasher<TYPE>::PrintVariation( const _m &var , std::ostream &dest , string lm , int width )
  {

    for ( int i = 0 ; i < this->GetK() ; i++ )
      dest << lm << std::setw( width ) << STLStringTools::Blank( width - 1 ) << STLStringTools::LeftOfExc( this->GetyName(i) , width - 1 ).c_str();
    for ( int i = 0 ; i < this->GetM() ; i++ )
      dest << std::setw( width ) << STLStringTools::LeftOfExc( this->GetxName(i) , width - 1 ).c_str();
    dest << std::endl;
    
#ifdef _SAFE
    if ( var.GetCols() != var.GetRows() )
      throw (string)"std::ostream &tFit<TYPE>::PrintVariation( const _m &var , std::ostream &dest , string lm , int width ) const : var.GetCols() != var.GetRows()";
    if ( var.GetCols() != this->GetM() + this->GetK()  )
      throw (string)"std::ostream &tFit<TYPE>::PrintVariation( const _m &var , std::ostream &dest , string lm , int width ) const : var.GetCols() != GetM() + GetK()";
    
    //    Validate( "std::ostream &tFit<TYPE>::PrintVariation( const _m &var , std::ostream &dest , string lm , int width ) const" );
#endif

    for ( int i = 1 ; i <= var.GetRows() ; i++ )
      {
	dest << lm;
	if ( i <= this->GetK() )
	  dest << std::setw( width ) << STLStringTools::LeftOfExc( this->GetyName(i-1) , width - 1 ).c_str();
	else
	  dest << std::setw( width ) << STLStringTools::LeftOfExc( this->GetxName(i- this->GetK() - 1) , width - 1 ).c_str();
	for ( int j = 1 ; j <= var.GetCols() ; j++ )
	  {
	    dest << std::setw( width ) << 
	      STLStringTools::LeftOfExc( STLStringTools::AsString( var.Get(i,j) , 10 , '.' ) , width - 1 ).c_str();
	  }
	dest << std::endl;
      }
    return dest;
  }


    template class tWasher<float>;
    template class tWasher<double>;
  }

}

