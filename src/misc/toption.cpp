/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#include "toption.h"

#include <memory>
//#include "options.h"

using namespace std;

namespace P4th
{

  template<class T>
  tOption<T>::tOption(T aValue) {
    this->value = aValue; 
  }

  template<class T>
  shared_ptr<tOption<T> > tOption<T>::get(Options * const aOptions, string aName) {
    shared_ptr<Option0> opt0 = Option0::get(aOptions, aName);
    shared_ptr<tOption<T> > opt = dynamic_pointer_cast<tOption<T> >(opt0);
    if (!opt) {
      throw aException(tOption<T>, "Option " + aName + " exist but is of wrong type.");
    }
    return opt;
  }

  template<class T>
  void tOption<T>::set(Options * aOptions, string aName, T aValue) {
    Option0::set(aOptions, aName, new tOption<T>(aValue));
  }
  
  template class tOption<int>;
  template class tOption<double>;
  template class tOption<float>;
  template class tOption<string>;
  template class tOption<bool>;
  template class tOption<vector<string> >;
}
