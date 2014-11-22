#include <testimator0.h>

#include <vector>
#include <tfit.h>

using namespace std;

namespace P4th {
  
  namespace Fit {
    
    template<class TYPE> 
    tEstimator0<TYPE>::tEstimator0(tFit<TYPE> *_owner )
    {
      assert(_owner);
      this->owner = _owner;
    }
    
    template <class TYPE>
    const vector<tnmmatrix<TYPE> > &tEstimator0<TYPE>::GetMiscItem(const string aKey)
    {
      return this->misc[aKey]; //Will return an empty _mVec if key is not found.
    }


    template <class TYPE>
    shared_ptr<tnmmatrix<TYPE> > tEstimator0<TYPE>::GetX()
    { 
      return this->owner->GetX(); 
    }
    
    template <class TYPE>
    shared_ptr<tnmmatrix<TYPE> > tEstimator0<TYPE>::GetY()
    { 
      return this->owner->GetY(); 
    }

    template <class TYPE>
    int tEstimator0<TYPE>::GetK() const
    { 
      return this->owner->GetK(); 
    }

    template <class TYPE>
    int tEstimator0<TYPE>::GetN() const
    { 
      return this->owner->GetN(); 
    }

    template <class TYPE>
    int tEstimator0<TYPE>::GetM() const
    { 
      return this->owner->GetM(); 
    }

    template class tEstimator0<float>;
    template class tEstimator0<double>;
  }
  
}
