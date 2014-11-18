/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#include "treciprocal.h"

#include "tsquare.h"



namespace P4th
{
  
  
  template<class TYPE> 
  tFunction0<TYPE> *tReciprocal<TYPE>::AnalyticalDerivative( int argnr ) const
  {
    return tFunction0<TYPE>::Mul(  tFunction0<TYPE>::Mul( new tConstant<TYPE>( -1  ) ,
							  this->f->Getdydx( argnr )->Clone() ) , 
				   new tReciprocal<TYPE>(  tFunction0<TYPE>::Mul( this->f->Clone() , 
										  this->f->Clone() ) ) );
    
  }

    
}

template class P4th::tReciprocal<float>;
template class P4th::tReciprocal<double>;
