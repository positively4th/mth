#include <twasher0.h>

#include <vector>
#include <tfit.h>

using namespace std;

namespace P4th {
  
  namespace Fit {
    
    template <class TYPE>
    const vector<tnmmatrix<TYPE> > &tWasher0<TYPE>::GetXs() const
    { 
      return this->estimator->xs; 
    }

    template <class TYPE>
    const vector<tnmmatrix<TYPE> > &tWasher0<TYPE>::GetYs() const
    { 
      return this->estimator->ys; 
    }

    template <class TYPE>
    int tWasher0<TYPE>::GetK() const
    { 
      return this->estimator->GetK(); 
    }

    template <class TYPE>
    int tWasher0<TYPE>::GetN() const
    { 
      return this->estimator->GetN(); 
    }

    template <class TYPE>
    int tWasher0<TYPE>::GetM() const
    { 
      return this->estimator->GetM(); 
    }

    template <class TYPE>
    string tWasher0<TYPE>::GetxName(int i) const
    { 
      return this->estimator->GetxName(i); 
    }

    template <class TYPE>
    string tWasher0<TYPE>::GetyName(int i) const
    { 
      return this->estimator->GetyName(i); 
    }
    
    template <class TYPE>
    shared_ptr<tnmmatrix<TYPE> > tWasher0<TYPE>::Getmu() const
    {
      return estimator->Getmu(); 
    }
    
    template <class TYPE>
    shared_ptr<tnmmatrix<TYPE> > tWasher0<TYPE>::Getsigma() const
    {
      return estimator->Getsigma();
    }

    template <class TYPE>
    shared_ptr<tnmmatrix<TYPE> > tWasher0<TYPE>::Getrho() const {
      return estimator->Getrho();
    }
  
    template class tWasher0<float>;
    template class tWasher0<double>;
}

}
