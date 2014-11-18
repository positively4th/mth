/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#include "tdiv.h"

#include "tmul.h"
#include "tsub.h"
#include <tsquare.h>

namespace P4th
{

  template<class TYPE> 
  tFunction0<TYPE> *tDiv<TYPE>::Reduce( int argnr , tFunction0<TYPE> *s  ) const
  {  
    Heap<tFunction0<TYPE> > h;
    h += s->Clone();
    
    return tFunction0<TYPE>::Div( this->left->Reduce( argnr , s ) , 
				  this->right->Reduce( argnr , h.Pop() ) ); 
  }

  template<class TYPE> 
  tFunction0<TYPE> *tDiv<TYPE>::AnalyticalDerivative( int argnr ) const
  {
    
    return tFunction0<TYPE>::Div( 
				 tFunction0<TYPE>::Sub( 
						       tFunction0<TYPE>::Mul( this->right->Clone() , 
									      this->left->Getdydx( argnr )->Clone() , 0L ) , 
						       tFunction0<TYPE>::Mul( this->left->Clone() , 
									      this->right->Getdydx( argnr )->Clone() , 0L ) ) ,
				 tFunction0<TYPE>::Mul( this->right->Clone() , this->right->Clone() ) 
				 );
  }

  template<class TYPE> 
  TYPE tDiv<TYPE>::y( const tnmmatrix<TYPE> &x ) const
  {
    //    _DB( std::cout << "num = " << this->left->y( x ) << ", den = " << this->right->y( x ) << std::endl; );
    return this->left->y( x ) / this->right->y( x );
  }


  
}

template class P4th::tDiv<float>;
template class P4th::tDiv<double>;


