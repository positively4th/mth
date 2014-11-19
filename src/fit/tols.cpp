#include <tols.h>

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

  namespace Fit {

    template<class TYPE> 
    tOLS<TYPE>::tOLS(tFit<TYPE> *_owner ) : 
      tEstimator0<TYPE>( _owner ), options(new Options())
    {
      this->ResetOptions();
    }
    
    template<class TYPE> 
    void tOLS<TYPE>::ResetOptions() {
      _typeOpt::set(this->options.get(), "minDeterminant" , 0.00000001);
      _boolOpt::set(this->options.get(), "_DB_PRINT_DET" , false);
      _boolOpt::set(this->options.get(), "_DB_PRINT_XTX" , false);
    }
    
    
    template<class TYPE>
    shared_ptr<tnmmatrix<TYPE> >  tOLS<TYPE>::GetB()
    {
      if (B) {
	return B;
      }
      
      this->misc.clear();
      TYPE minDeterminant = _typeOpt::read(this->options.get(), "minDeterminant");
      bool _DB_PRINT_DET = _boolOpt::read(this->options.get(), "_DB_PRINT_DET");
      bool _DB_PRINT_XTX = _boolOpt::read(this->options.get(), "_DB_PRINT_XTX");
      
      int MM = this->GetX()->GetCols();
      int NN = this->GetX()->GetRows();
      int KK = this->GetK();
      TYPE rcond;
      TYPE det;
      
      _m Xtrp(MM,NN);
      _m Qinv(MM,MM);
      _m Xtrpyvec(MM,KK);
      _m I(NN,NN);
      B.reset(new _m(MM,KK));
      _mVec bCov;
      _m matrixM(NN,NN);
      _m e(NN,KK);
      _m eTrp(KK,NN);
      _m s2(KK,KK);
      TYPE cond = 0;
      
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
	Xtrpyvec.Mul( Xtrp , *this->GetY() );
	B->Mul(Qinv , Xtrpyvec);

	matrixM = I - (*this->GetX()) * Qinv * Xtrp;
	e = matrixM * (*this->GetY());
	e.Trp( &eTrp );
	s2 = (TYPE)( 1.0 / ( NN - MM ) ) * eTrp * e;
	for ( int col = 1 ; col <= KK ; col++ ) {
	  bCov.emplace_back(s2(col,col) * Qinv);
	}
      } else
      B->Cpy(0.0);
    
    
    //Relay non standard fit data to owner
    this->misc["e"] = _mVec(1, e);
    this->misc["s2"] = _mVec(1, s2);
    this->misc["determinant"] = _mVec(1, _m(1,1,det));
    this->misc["conditionNumber"] = _mVec(1, _m(1,1,cond));
    this->misc["bCov"] = _mVec(bCov);
    return B;  
  }

  template<class TYPE>
  shared_ptr<tFunctions<TYPE> > tOLS<TYPE>::GetPredictors() 
  {
    if (!this->predictors) {
      $_m B = this->GetB();
      
      predictors.reset(new _fs());
      unique_ptr<_add> sum((_add *)NULL); 
      for (unsigned int k = 1 ; k <= this->GetK() ; k++) { 
	sum.reset(new _add());
	for (unsigned int m = 1 ; m <= this->GetX()->GetCols() ; m++ ) {
	  sum->AddTerm(new _mul(new _arg(m), new _c(B->Get(m,k))));
	}
	predictors->AddFunction(sum.release());
      }
    }
    return predictors;
  }

  template<class TYPE>
  tnmmatrix<TYPE> &tOLS<TYPE>::yhat( tnmmatrix<TYPE> &res , const tnmmatrix<TYPE> &xrow , const tnmmatrix<TYPE> &B  )
  {
    //    xrow.Print( std::cout , "xrow: \t" , 120 );
    //    B.Print( std::cout , "B: \t" , 120 );
    res.Mul( xrow , *this->GetB() );
    
    return res;
    
  }


    template class tOLS<float>;
    template class tOLS<double>;

  }  

}




