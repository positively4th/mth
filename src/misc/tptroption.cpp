/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#include "tptroption.h"

#include <polymorphic.h>

#include <memory>
#include <assert.h>
#include <texception.h>

using namespace std;

namespace P4th
{

  template<class SPTR, typename T>
  tPtrOption<SPTR,T>::tPtrOption(T *aPtr) : SPTR(aPtr)  {
  }

  template<class SPTR, typename T>
  tPtrOption<SPTR,T>::tPtrOption(const SPTR &aPtr) : SPTR(aPtr)  {
  }

  template<class SPTR, typename T>
  shared_ptr<tPtrOption<SPTR,T> > tPtrOption<SPTR,T>::get(Options * const aOptions, string aName) {
    shared_ptr<Option0> opt0 = Option0::get(aOptions, aName);
    shared_ptr<tPtrOption<SPTR,T> > ptrOpt = dynamic_pointer_cast<tPtrOption<SPTR,T> >(opt0);
    if (!ptrOpt) {
      throw aException(tPtrOption<SPTR aComma T>, "PtrOption " + aName + " exist but is of the wrong pointer type.");
    }
    return ptrOpt;
  }

  template<class SPTR, typename T>
  SPTR tPtrOption<SPTR,T>::read(Options * const aOptions, string aName) 
  { 
    shared_ptr<tPtrOption<SPTR,T> > ptr0 = tPtrOption<SPTR,T>::get(aOptions, aName); //<- dynamically casted from shared_ptr<tOption0>
    assert(ptr0);
    tPtrOption<SPTR,T> ptr1(*ptr0);  //<- shared_ptr<T>
    assert(ptr1);
    SPTR ptr2 = dynamic_pointer_cast<T>(ptr1);
    assert(ptr2);
    return ptr2;
  }


  template<class SPTR, typename T>
  void tPtrOption<SPTR,T>::set(Options * aOptions, string aName, const SPTR &aPtr) 
  {
    Option0::set(aOptions, aName, new tPtrOption<SPTR,T>(aPtr));
  }

  template<class SPTR, typename T>
  void tPtrOption<SPTR,T>::set(Options * aOptions, string aName, T *aPtr) {
    Option0::set(aOptions, aName, new tPtrOption<SPTR,T>(aPtr));
  }
  
}

template class P4th::tPtrOption<std::shared_ptr<P4th::Polymorphic>, P4th::Polymorphic>;
