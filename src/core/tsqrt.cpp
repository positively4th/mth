/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#include "tsqrt.h"

#include "tconstant.h"
#include "treciprocal.h"



namespace P4th
{
  
  
  template<class TYPE> 
  tFunction0<TYPE> *tSqrt<TYPE>::AnalyticalDerivative( int argnr ) const
  {
    return tFunction0<TYPE>::Mul( 
				 new tConstant<TYPE>( 0.5  ) , 
				 tFunction0<TYPE>::Div( this->f->Getdydx( 0 , argnr , -1 )->Clone() , 
							new tSqrt<TYPE>( this->f->Clone() ) ) ,
				 0L 
				   );
  }
  
  
}

template class P4th::tSqrt<float>;
template class P4th::tSqrt<double>;
