#include "tnowasher.h"


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
  tNoWasher<TYPE>::tNoWasher(tFit<TYPE> *aEstimator) : 
      tWasher0<TYPE>(aEstimator) 
  {
  }
    
  template<class TYPE>
  shared_ptr<tnmmatrix<TYPE> > tNoWasher<TYPE>::GetX() 
  {
    std::cout << (string)__func__ << std::endl << std::flush;
    if (!X) {
      this->CopyObservations();
    }
    return X;
  }

  template<class TYPE>
  shared_ptr<tnmmatrix<TYPE> > tNoWasher<TYPE>::GetY() 
  {
    if (!Y) {
      this->CopyObservations();
    }
    return Y;
  }

  template<class TYPE> 
  unique_ptr<tFunctions<TYPE> > tNoWasher<TYPE>::unWash(const _fs *washedFunctions)
  {
    $$_fs inner(new _fs());
    for (int i = 0 ; i < this->GetM() ; i++) {
      inner->AddFunction(new _arg(i+1));
    } 
    //Todo: Move this into tFunctions::Composite!
    $$_fs res(new _fs());
    for (int k = 1 ; k <= this->GetK() ; k++) {
      res->AddFunction(_f::Composite(washedFunctions->Get(k)->Clone(), inner->Clone()));
    }
    return res;
  }

  template<class TYPE> 
  unique_ptr<tnmmatrix<TYPE> > tNoWasher<TYPE>::unWashB(const _m *washedM )
  {
    //    $$_m res(new _m(*washedM)); 
    //    return res;
    return $$_m(new _m(*washedM)); 
  }


    template<class TYPE>
      shared_ptr<string> tNoWasher<TYPE>::GetRegressorMask()
  {
    if (!regressorMask) {
      this->CopyObservations();
    }
    return regressorMask;
  }

  template<class TYPE>
  shared_ptr<string> tNoWasher<TYPE>::GetObservationMask()
  {
    if (!observationMask) {
      this->CopyObservations();
    }
    return observationMask;
  }

  template<class TYPE>
  void tNoWasher<TYPE>::CopyObservations() 
  {
    std::cout << "CopyObservations()" << std::endl << std::flush;
    const vector<_m> &xs = this->GetXs();
    const vector<_m> &ys = this->GetYs();

    regressorMask.reset(new string(STLStringTools::Fill( this->GetP() , '1' )));
    observationMask.reset(new string(STLStringTools::Fill( this->GetN() , '1' )));

    int regressors = this->GetM();
    int observations = this->GetN();
    if ( xs.size() < 1 )
      {
	regressors = 0;
	X.reset(new _m( this->GetN() , regressors ));
	Y.reset(new _m( this->GetN() , this->GetK()));
	return;
      }

    /* 
       Copy regressors
    */

    std::cout << "obsmask = " << *observationMask << std::endl << std::flush;
    Y.reset(new _m( observations , this->GetK()));
    X.reset(new _m( observations , regressors ));
    for ( int i = 0 ; i < this->GetN() ; i++ )
      {
	
	tnmmatrix<TYPE>::Set( *Y , ys[i] , i + 1 ,1 );
	tnmmatrix<TYPE> xvec( xs[i] );
	for ( int j = 0 ; j < this->GetM() ; j++ ) 
	  (*X.get())(i+1,j+1) = xvec(1,j+1);
      }
   
  }


    template class tNoWasher<float>;
    template class tNoWasher<double>;
  }

}

