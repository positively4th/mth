/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#include "tln.h"

#include "tconstant.h"
#include "treciprocal.h"
#include "tdiv.h"



namespace P4th
{
  
  
  template<class TYPE> 
  tFunction0<TYPE> *tLn<TYPE>::AnalyticalDerivative( int argnr ) const
  {
    return tFunction0<TYPE>::Div( this->f->Getdydx( argnr )->Clone() , this->f->Clone() ); 
  }
  
  
}

template class P4th::tLn<float>;
template class P4th::tLn<double>;
