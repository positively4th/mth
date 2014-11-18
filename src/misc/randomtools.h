/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#ifndef _CC_RANDOMTOOLS_H_
#define _CC_RANDOMTOOLS_H_

namespace P4th
{

  class RandomTools
    {
    protected:
      
    public:
      static double DrawUniform( double min , double max ); 
      static double DrawUniform( double rnr , double min , double max ); 
    };
}

#endif
