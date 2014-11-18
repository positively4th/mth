/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#include <options.h>
#include <texception.h>

namespace P4th
{

  Options::Options() 
  {
  }

  Options::~Options() {
  }

  Options *Options::set(string aName, Option0 *aOption) {
    this->optionMap[aName] = shared_ptr<Option0>(aOption);
    return this;
  }

  shared_ptr<Option0> Options::get(string aName) {
    _optionMap::const_iterator it = this->optionMap.find(aName);
    if ( it == this->optionMap.end()) {
      throw aException(Options, "Option " + aName + " is not defined.");
    }
    return it->second;
  }
  
}
