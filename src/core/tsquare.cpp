/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#include "tsquare.h"
#include "treciprocal.h"




namespace P4th
{
  

  template<class TYPE> 
  tFunction0<TYPE> *tSquare<TYPE>::AnalyticalDerivative( int argnr ) const
  {
    
    return tFunction0<TYPE>::Mul(  
				 this->f->Getdydx( 0.0 , argnr , - 1 )->Clone() , 
				 tFunction0<TYPE>::Mul( new tConstant<TYPE>( (TYPE)2 ) , 
							this->f->Clone() ) );
    
    
    
  }
  
  
}

template class P4th::tSquare<float>;
template class P4th::tSquare<double>;
