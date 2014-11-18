/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#include "tnegative.h"

#include "tsquare.h"



namespace P4th
{
  
  template<class TYPE> 
  tFunction0<TYPE> *tNegative<TYPE>::AnalyticalDerivative( int argnr ) const
    {
      return new tNegative( this->f->Getdydx( argnr )->Clone() );
      
    }

    
}

template class P4th::tNegative<float>;
template class P4th::tNegative<double>;
