/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#include "tsign.h"

#include "tconstant.h"
#include "treciprocal.h"



namespace P4th
{
  
  
  template<class TYPE> 
  tFunction0<TYPE> *tSign<TYPE>::AnalyticalDerivative( int argnr ) const
  {
    return new tConstant<TYPE>( 0.0 );
  }
  
  
}

template class P4th::tSign<float>;
template class P4th::tSign<double>;
