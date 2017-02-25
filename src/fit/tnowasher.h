#pragma once

#include <misctypes.h>
#include <coretypes.h>
#include <vector>
#include <memory>
#include <map>
#include <debug.h>
#include <tnmmatrix.h>
#include <tfunctions.h>
#include <tselector.h>
#include <twasher0.h>
#include <options.h>
#include <string>

namespace P4th {


  template<class TYPE>
    class tFit;

  namespace Fit {

    template<class TYPE>
      class tNoWasher : public tWasher0<TYPE> {
      MTH_MISC_TYPES(TYPE)
      MTH_CORE_TYPES(TYPE)
    protected:
      shared_ptr<_m> X;
      shared_ptr<_m> Y;
      shared_ptr<string> regressorMask;
      shared_ptr<string> observationMask;

      virtual void CopyObservations();
    public:
      tNoWasher(tFit<TYPE> *aEstimator);

      virtual $_m GetX();
      virtual $_m GetY();
      virtual $string GetRegressorMask();
      virtual $string GetObservationMask();

      virtual $$_m unWashB(const _m *washedM);
      virtual $$_fs unWash(const _fs *washedFunctions);

    };
  }

}
