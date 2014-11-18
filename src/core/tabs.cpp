/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#include "tabs.h"

#include "tconstant.h"
#include "tsign.h"



namespace P4th
{
  
  
  template<class TYPE> 
  tFunction0<TYPE> *tAbs<TYPE>::AnalyticalDerivative( int argnr ) const
  {
    return tFunction0<TYPE>::Mul( this->f->Getdydx( argnr )->Clone()  , new tSign<TYPE>( this->f->Clone() ) );  
  }
  
  
}

template class P4th::tAbs<float>;
template class P4th::tAbs<double>;
