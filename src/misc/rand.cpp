/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#include "rand.h"

#include <stdio.h>
#include <stdlib.h>
#ifndef _WIN32
#include <unistd.h>
#endif
#include <time.h>

namespace P4th
{

  Rand::Rand( unsigned int seed0 )
  {
    if ( seed0 == 0 )
      seed0 = (unsigned int)time( 0L );
    srand( seed0 );
  }

  double Rand::StdUniform()
  {
    return (double)rand() / RAND_MAX;
  }

}
