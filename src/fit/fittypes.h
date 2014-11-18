#pragma once

template<class TYPE>
class tWasher;				

#define MTH_FIT_TYPES(TYPE)	    \
  /* MTH_CORE_TYPES(TYPE): BEGIN */	    \
  typedef P4th::Fit::tWasher<TYPE> _washer;	\
  typedef P4th::tPtrOption<shared_ptr<_washer>, _washer> $_washerOpt;
  /* MTH_CORE_TYPES(TYPE): EMD */
