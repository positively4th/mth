/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#include "randomtools.h"

#include "rand.h"


namespace P4th
{

  double RandomTools::DrawUniform( double rnr , double min , double max )
  {
    return min + rnr * ( max - min );
  }

  double RandomTools::DrawUniform( double min , double max )
  {
    return DrawUniform( Rand::StdUniform() , min , max );
  }
  
}
