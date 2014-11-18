/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#include "tparameter.h"

#include "tconstant.h"

namespace P4th
{

  
  template<class TYPE> 
  tConstant<TYPE> tParameter<TYPE>::Df( 0 );
  
  template<class TYPE> 
  const tFunction0<TYPE> *tParameter<TYPE>::Getdydx( TYPE dx , int argnr , int argnr2 , ...  ) const
  {
    if ( argnr == 0 )
      return this;
    return &Df;
  }
  
  
}

template class P4th::tParameter<float>;
template class P4th::tParameter<double>;

