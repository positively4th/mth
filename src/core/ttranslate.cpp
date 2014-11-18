/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#include "ttranslate.h"
#include "treciprocal.h"




namespace P4th
{
  

  template<class TYPE> 
  tFunction0<TYPE> *tTranslate<TYPE>::AnalyticalDerivative( int dargnr ) const
  {
    
    return new tTranslate( this->f->Getdydx( 0.0 , dargnr , -1 )->Clone() , this->argnr , this->delta );
    
  }
  
  
}

template class P4th::tTranslate<float>;
template class P4th::tTranslate<double>;
