#pragma once

#include <memory>
#include <string>

namespace P4th {
  
  class Options;				
  
  class Option0;				
  
  template <class TYPE>
    class tOption;
  
  template <class TYPE>
    class tOption;

}

typedef std::shared_ptr<::std::string> $string;

typedef ::std::shared_ptr<::P4th::Options> $options;
typedef ::std::unique_ptr<::P4th::Options> $$options;
typedef ::std::shared_ptr<::P4th::Option0> $option0;
typedef ::P4th::tOption<int> _intOpt;
typedef ::P4th::tOption<::std::string> _strOpt;
typedef ::P4th::tOption<float> _floatOpt;
typedef ::P4th::tOption<double> _doubleOpt;
typedef ::P4th::tOption<bool> _boolOpt;

#define MTH_MISC_TYPES(TYPE)					\
  /* MTH MISC Types Begin */ 					\
  typedef ::P4th::tOption<TYPE> _typeOpt;			\
  /* MTH MISC Types End */ 					


  
