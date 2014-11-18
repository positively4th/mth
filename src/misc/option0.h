/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#pragma once


#include <misctypes.h>
#include <string>

using namespace std;

namespace P4th
{

  class Options;

  class Option0 {
  protected:
    //Relay to Options, who option0 is a friend of.
    static Options *set(Options * aOptions, string aName, Option0 *aOption);
    static $option0 get(Options * const aOptions, string aName);

  public:
    virtual ~Option0() {}; //To make this class polymorphic 

  };

}
