/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#include "option0.h"
#include <options.h>

namespace P4th
{
  

  Options *Option0::set(Options * aOptions, string aName, Option0 *aOption) {
    return aOptions->set(aName, aOption);
  }
  
  $option0 Option0::get(Options * const aOptions, string aName) {
    return aOptions->get(aName);
  }
  
}
