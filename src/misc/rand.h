/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#ifndef _P4TH_RAND_H_
#define _P4TH_RAND_H_

namespace P4th
{

  class Rand
    {
    protected:
    public:
      Rand( unsigned int seed0 = 0 );
      static double StdUniform(); 
    };
}

#endif
