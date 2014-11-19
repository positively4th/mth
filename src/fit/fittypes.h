#pragma once

#include <memory>

namespace P4th {
  
  template<class SPTR, class T>
    class tPtrOption;				
  
  namespace Fit {
    
    template<class TYPE>
      class tWasher0;				
    
    template<class TYPE>
      class tWasher;				
    
    template<class TYPE>
      class tEstimator0;				
    
    template<class TYPE>
      class tOLS;				
    
  }
  
}


#define MTH_FIT_TYPES(TYPE)						\
  /* MTH_CORE_TYPES(TYPE): BEGIN */					\
  typedef ::P4th::Fit::tWasher<TYPE> _washer0;				\
  typedef ::P4th::Fit::tWasher<TYPE> _washer;				\
  typedef ::P4th::tPtrOption<::std::shared_ptr<_washer0>, _washer0> $_washerOpt; \
  typedef ::P4th::Fit::tEstimator0<TYPE> _estimator0;			\
									\
  typedef ::std::shared_ptr<_washer> $_washer;				\
  typedef ::std::shared_ptr<_estimator0> $_estimator0;			\
  typedef ::P4th::Fit::tOLS<TYPE> _ols;					\
  typedef ::P4th::tPtrOption<::std::shared_ptr<_estimator0>, _ols> $_estimatorOpt; \
  /* MTH_CORE_TYPES(TYPE): EMD */ 
