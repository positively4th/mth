#pragma once

#include <misctypes.h>
#include <coretypes.h>
#include <vector>
#include <memory>
#include <map>
#include <debug.h>
#include <tnmmatrix.h>
#include <twasher0.h>
#include <options.h>
#include <string>

namespace P4th {


  template<class TYPE>
    class tFit;

  namespace Fit {

    template<class TYPE>
      class tWasher : public tWasher0<TYPE> {
      MTH_MISC_TYPES(TYPE)
      MTH_CORE_TYPES(TYPE)
    protected:
      shared_ptr<_m> X;
      shared_ptr<_m> Y;
      shared_ptr<string> regressorMask;
      shared_ptr<string> observationMask;

      virtual void ResetOptions();
      virtual void CopyObservations();
      virtual std::ostream &PrintVariation( const _m &var , std::ostream &dest , string lm , int width );
    public:
      tWasher(tFit<TYPE> *aEstimator);
      tWasher(const tWasher &source);

      virtual shared_ptr<_m> GetX();
      virtual shared_ptr<_m> GetY();
      virtual shared_ptr<string> GetRegressorMask();
      virtual shared_ptr<string> GetObservationMask();

      virtual $$_m unWash(const _m *washedM, const string &rowMask, const string &colMask, TYPE nullValue);
      virtual $$_m unWashB(const _m *washedM);
      virtual $$_fs unWash(const _fs *washedFunctions);
      

    };
  }

}
