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

  template<class T>
    class tOption : public Option0 {
  protected:
    T value;
  public:
    tOption(T aValue);
    
    operator T() const {
      return this->value;
    };

    //Use set to insert or replace an option
    static void set(Options * aOptions, string aName, T aValue);
    //Use read to get the actual value 
    static T read(Options * const aOptions, string aName) 
    { return (T)(*tOption<T>::get(aOptions, aName).get()); };
    //Use get (then set) to modify an option
    static shared_ptr<tOption<T> > get(Options * const aOptions, string aName);

  };
  
}
