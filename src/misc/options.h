/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#pragma once

#include <misctypes.h>

#include <option0.h>
#include <string>
#include <map>
#include <vector>
#include <memory>

using namespace std;

namespace P4th
{


  class Options {
    typedef map<string,shared_ptr<Option0> > _optionMap;
  protected:
    _optionMap optionMap;
    Options *set(string aName, Option0 *aOption);
    $option0 get(string aName);
  public:
    Options();
    ~Options();

    friend class Option0;
  };

}
