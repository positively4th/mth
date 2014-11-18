#pragma once

#include <coretypes.h>
#include <misctypes.h>
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

    protected:
      MTH_MISC_TYPES(TYPE)
      MTH_CORE_TYPES(TYPE)
      tFit<TYPE> *estimator;
    public:
      tWasher0(tFit<TYPE> *aEstimator) {
	assert(aEstimator);
	this->estimator = aEstimator;
      }
      tWasher0(const tWasher0 &source)
	{ this->estimator = source.estimator; }
      

      //Washer must define these
      virtual shared_ptr<_m> GetX() = 0;
      virtual shared_ptr<_m> GetY() = 0;
      virtual shared_ptr<string> GetRegressorMask() = 0;
      virtual shared_ptr<string> GetObservationMask() = 0;

      /* Washers use these to read properties from the estimator */
      //Unwashed data
      const vector<tnmmatrix<TYPE> > &GetXs() const;
      const vector<tnmmatrix<TYPE> > &GetYs() const;
      //Estimatino parameters
      int GetK() const;
      int GetN() const;
      int GetM() const;
      //Statistics
      shared_ptr<_m> Getmu() const;
      shared_ptr<_m> Getsigma() const;
      shared_ptr<_m> Getrho() const;
      //Misc
      string GetxName(int index) const;
      string GetyName(int nr) const;

    };
  }

}
