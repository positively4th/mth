#pragma once

#include <coretypes.h>
#include <misctypes.h>
#include <fittypes.h>

#include <memory>
#include <vector>
#include <map>
#include <debug.h>
#include <tnmmatrix.h>
#include <string>
#include <assert.h>   

namespace P4th {


  template<class TYPE>
    class tFit;

  namespace Fit {
    
    template<class TYPE>
      class tWasher0 {
    public:
      MTH_MISC_TYPES(TYPE)
      MTH_CORE_TYPES(TYPE)
      MTH_FIT_TYPES(TYPE)
    protected:
      tFit<TYPE> *estimator;
    public:
      tWasher0(tFit<TYPE> *aEstimator) {
	assert(aEstimator);
	this->estimator = aEstimator;
      }
      tWasher0(const tWasher0 &source)
	{ this->estimator = source.estimator; }
      

      //Any tWasher must define these
      virtual shared_ptr<_m> GetX() = 0;
      virtual shared_ptr<_m> GetY() = 0;
      virtual shared_ptr<string> GetRegressorMask() = 0;
      virtual shared_ptr<string> GetObservationMask() = 0;

      /* tFit API */
      //Unwashed data
      const vector<tnmmatrix<TYPE> > &GetXs() const;
      const vector<tnmmatrix<TYPE> > &GetYs() const;
      //Estimation parameters
      int GetK() const;
      int GetN() const;
      int GetM() const;
      //Statistics
      $_m Getmu() const;
      $_m Getsigma() const;
      $_m Getrho() const;
      //Misc
      string GetxName(int index) const;
      string GetyName(int nr) const;

    };
  }

}
