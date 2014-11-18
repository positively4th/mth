/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#include "tsub.h"
#include "tconstant.h"




namespace P4th
{

  template<class TYPE> 
  tFunction0<TYPE> *tSub<TYPE>::Reduce( int argnr , tFunction0<TYPE> *s  ) const
  {  
    Heap<tFunction0<TYPE> > h;
    h += s->Clone();
    
    return tFunction0<TYPE>::Sub( this->left->Reduce( argnr , s ) , 
				  this->right->Reduce( argnr , h.Pop() ) ); 
  }
  
  template<class TYPE> 
  TYPE tSub<TYPE>::y( const tnmmatrix<TYPE> &x ) const
  {
    return this->left->y( x ) - this->right->y( x );
  }
  

  template<class TYPE> 
  tFunction0<TYPE> *tSub<TYPE>::AnalyticalDerivative( int argnr ) const
  {
    
    return tFunction0<TYPE>::Sub(  this->left->Getdydx( argnr )->Clone() , 
				   this->right->Getdydx( argnr )->Clone() );
  }

  
}

template class P4th::tSub<float>;
template class P4th::tSub<double>;


