/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#pragma once

#include <option0.h>
#include <texception.h>
#include <string>
#include <vector>


namespace P4th
{

  class Options;

  template<typename SPTR, typename T>
    class tPtrOption : public SPTR, public Option0 {
  protected:
  public:
    //    tPtrOption(const SPTR &aPtr);
    tPtrOption(T *aPtr);
    tPtrOption(const SPTR &aPtr);
    
    //Use set to insert or replace an option
    static void set(Options * aOptions, string aName, T *aPtr);
    //Use read to get the actual value 
    static void set(Options * aOptions, string aName, const SPTR &aPtr);
    static SPTR read(Options * const aOptions, string aName);
    //Use get (then set) to modify an option
    static shared_ptr<tPtrOption<SPTR,T> > get(Options * const aOptions, string aName);

  };
  
}
