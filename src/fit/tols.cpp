#include "tols.h"

#include <string>
#include <memory>
#include <iomanip>

#include <debug.h>
#include <math.h>
#include <stlstringtools.h>
#include <texception.h>
#include <tmathtools.h>
#include <tfunctions.h>
#include <tadd.h>
#include <tmul.h>
#include <tselector.h>


namespace P4th
{

  template<class TYPE> 
  tOLS<TYPE>::tOLS( int _M , int _K  ) :
    tFit<TYPE>(_M , _K)
  {
    this->ResetOptions();
  }

  template<class TYPE> 
  void tOLS<TYPE>::ResetOptions() {
    tFit<TYPE>::ResetOptions();

    _typeOpt::set(this->options.get(), "minDeterminant" , 0.00000001);
    _boolOpt::set(this->options.get(), "_DB_PRINT_DET" , false);
    _boolOpt::set(this->options.get(), "_DB_PRINT_XTX" , false);
  }


  template<class TYPE> 
  void tOLS<TYPE>::AddObservation( const tnmmatrix<TYPE> &Y , const tnmmatrix<TYPE> &X )
  {
    if ( X.GetCols() != this->M )
      throw aException(tOLS<TYPE>, "X.GetCols() != M");
    return tFit<TYPE>::AddObservation(Y,X);
  }
  
  template<class TYPE>
  shared_ptr<tnmmatrix<TYPE> >  tOLS<TYPE>::EstimateB()
  {
    TYPE minDeterminant = _typeOpt::read(this->options.get(), "minDeterminant");
    bool _DB_PRINT_DET = _boolOpt::read(this->options.get(), "_DB_PRINT_DET");
    bool _DB_PRINT_XTX = _boolOpt::read(this->options.get(), "_DB_PRINT_XTX");

    int MM = this->GetX()->GetCols();
    int NN = this->GetX()->GetRows();
    TYPE rcond;
    TYPE det;

    _m Xtrp(MM,NN);
    _m Qinv(MM,MM);
    _m Xtrpyvec(MM,this->GetK());
    _m I(NN,NN);
    _m BB(MM,this->GetK());

    I.I();
    if ( MM > 0 )
      {
	//	tMathTools<TYPE>::PrintMatrix( std::cout , X , 6 , "" , "" , "X: " );
	//	std::cout << std::flush;
	
	this->GetX()->Trp( &Xtrp );
	Qinv.Mul( Xtrp , *this->GetX() ); 
	if (_DB_PRINT_XTX)
	  tMathTools<TYPE>::PrintMatrix( std::cout , Qinv , 6 , "" , "" , "X'X: " );

	try {
	  if ( minDeterminant >= 0 ) {
	    det = Qinv.Det();
	    if (_DB_PRINT_DET )
	      std::cout << "|X'X| = " << det << std::endl << std::flush;
	    if ( det <= minDeterminant )
	      throw aException(tOLS<TYPE>, "X'X is singular.");
	  }
	  Qinv.Inv( &rcond );
	} catch ( ... ) {
	  throw aException(tOLS<TYPE>, "Inversion failed!");
	};
	assert( rcond != 0.0 );
	cond = 1.0 / rcond;
	_DB( std::cout << "void tOLS<TYPE>::Estimate() : rcond = " << rcond << std::endl; );
	Xtrpyvec.Mul( Xtrp , *this->GetY() );
	BB.Mul(Qinv , Xtrpyvec);

	this->matrixM.reset(new _m( I - (*this->GetX()) * Qinv * Xtrp ));
	//	_DB( _M.Print( std::cout , "M: \t" , 80 ) );
	
	//	_DB( std::cout << "Computing e" << std::endl; );
	
	this->e.reset(new _m(*this->matrixM * (*this->GetY())));
	//	_DB( this->e.Print( std::cout , "e: \t" , 80 ) );

	//	_DB( std::cout << "Computing s2" << std::endl; );
	P4th::tnmmatrix<TYPE> eTrp( this->e->GetCols() , this->e->GetRows() );
	this->e->Trp( &eTrp );
	this->s2.reset( new _m((TYPE)( 1.0 / ( NN - MM ) ) * eTrp * (*this->e)));

	//	_DB( std::cout << "Computing b covariances" << std::endl; );
	this->bCovariances.clear();
	for ( int col = 1 ; col <= this->GetK() ; col++ ) {
	  //	  shared_ptr<_m> mptr = shared_ptr<_m>(new _m(MM,MM,0.0));
	  //	  this->bCovariances.push_back(mptr);
	  //	  this->bCovariances.emplace_back(new _m(MM,MM,0.0));
	  this->bCovariances.emplace_back(new _m(this->s2->Get(col,col) * Qinv));
	}
      } else
      BB.Cpy(0.0);
    this->iterations = 1;
    
    int ctr = 1; 
    this->B.reset(new _m(this->GetM(), this->GetK(),  0.0));
    for ( int i = 1 ; i <= this->B->GetRows() ; i++ )
      {
	if ( ctr > this->GetRegressors() )
	  break;
	if ( (*this->GetRegressorMask())[i-1] == '1' ) {
	  for ( int k = 1 ; k <= this->GetK() ; k++ )
	    (*this->GetB())(i,k) = BB(ctr,k);
	  ctr++;
	}
      }

    return this->B;
  }

  template<class TYPE>
  shared_ptr<tFunctions<TYPE> > tOLS<TYPE>::CreatePredictors() 
  {
    $string regressors = this->GetRegressorMask();
    unsigned int xSize = regressors->size();
    $_m B = this->GetB();

    $_fs fs(new _fs());
    for (unsigned int k = 1 ; k <= this->GetK() ; k++) { 
      auto_ptr<_add> f(new _add()); 
      for (unsigned int m = 1 ; m <= xSize ; m++ ) {
	if ((*regressors)[m-1] != '0') {
	  f->AddTerm(new _mul(new _arg(m), new _c(B->Get(m,k))));
	}
      }
      fs->AddFunction(f.release());
    }
    return fs;
  }

  template<class TYPE>
  tnmmatrix<TYPE> &tOLS<TYPE>::yhat( tnmmatrix<TYPE> &res , const tnmmatrix<TYPE> &xrow , const tnmmatrix<TYPE> &B  )
  {
    //    xrow.Print( std::cout , "xrow: \t" , 120 );
    //    B.Print( std::cout , "B: \t" , 120 );
    res.Mul( xrow , *this->GetB() );
    
    return res;
    
  }
  
}

template class P4th::tOLS<float>;
template class P4th::tOLS<double>;



