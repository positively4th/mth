#pragma once

#include <memory>
#include <string>


using namespace std;

namespace P4th {


  class Options;				

  class Option0;				

  template <class TYPE>
    class tOption;

  template <class TYPE>
    class tOption;

  typedef shared_ptr<Options> $options;
  typedef unique_ptr<Options> $$options;
  typedef shared_ptr<Option0> $option0;
  typedef tOption<int> _intOpt;
  typedef tOption<string> _strOpt;
  typedef tOption<float> _floatOpt;
  typedef tOption<double> _doubleOpt;
  typedef tOption<bool> _boolOpt;
  typedef shared_ptr<string> $string;


}

#define MTH_MISC_TYPES(TYPE) \
  typedef P4th::tOption<TYPE> _typeOpt;				\


  
