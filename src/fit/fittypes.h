#pragma once

#include <memory>

namespace P4th {
  
  template<class SPTR, class T>
    class tPtrOption;				
  
  namespace Fit {
    
    template<class TYPE>
      class tWasher0;				
    
    template<class TYPE>
      class tNoWasher;				
    
    template<class TYPE>
      class tWasher;				
    
    template<class TYPE>
      class tEstimator0;				
    
    template<class TYPE>
      class tOLS;				
    
    template<class T>
      class tFitLS;				
    
  }
  
}


#define MTH_FIT_TYPES(TYPE)						\
  /* MTH_CORE_TYPES(TYPE): BEGIN */					\
  typedef ::P4th::tFit<TYPE> _fit;					\
  typedef ::P4th::Fit::tWasher0<TYPE> _washer0;				\
  typedef ::P4th::Fit::tNoWasher<TYPE> _noWasher;			\
  typedef ::P4th::Fit::tWasher<TYPE> _washer;				\
  typedef ::P4th::Fit::tEstimator0<TYPE> _estimator0;			\
  									\
  typedef ::std::shared_ptr<_washer0> $_washer0;			\
  typedef ::std::shared_ptr<_noWasher> $_noWasher;			\
  typedef ::std::shared_ptr<_washer> $_washer;				\
  typedef ::std::shared_ptr<_estimator0> $_estimator0;			\
  typedef ::P4th::Fit::tOLS<TYPE> _ols;					\
  typedef ::P4th::Fit::tFitLS<TYPE> _fitLS;				\
  typedef ::std::shared_ptr<_ols> $_ols;				\
  typedef ::std::shared_ptr<_fitLS> $_fitLS;				\
									\
  typedef ::P4th::tPtrOption<$_washer0, _washer0> $_washerOpt;		\
  typedef ::P4th::tPtrOption<$_estimator0, _estimator0> $_estimatorOpt; \
  typedef ::P4th::tPtrOption<$_ols, _ols> $_olsOpt;			\
  typedef ::P4th::tPtrOption<$_fitLS, _ols> $_FitLSOpt;			\
  /* MTH_CORE_TYPES(TYPE): END */ 
