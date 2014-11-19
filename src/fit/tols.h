#ifndef _P4TH_TOLS_H_
#define _P4TH_TOLS_H_

#include <misctypes.h>
#include <coretypes.h>
#include <fittypes.h>

#include <tfit.h>
#include <testimator0.h>

namespace P4th
{

  namespace Fit {

    template<class TYPE> 
      class tOLS : public tEstimator0<TYPE> 
      {
      public:
	MTH_MISC_TYPES(TYPE);
	MTH_CORE_TYPES(TYPE);
	MTH_FIT_TYPES(TYPE);
      protected:
	$$options options;
	$_m B;
	$_fs predictors;
      public:
	tOLS( tFit<TYPE> *owner );
	
	void ResetOptions();
	
	virtual $_m GetB();
	virtual $_fs GetPredictors(); 
	virtual tnmmatrix<TYPE> &yhat( tnmmatrix<TYPE> &res , const  tnmmatrix<TYPE> &xrow , const  tnmmatrix<TYPE> &B );
	
	
      };
    
  }

}


#endif
