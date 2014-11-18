/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#include "tpow.h"

#include <tconstant.h>
#include <tln.h>



namespace P4th
{
  

  template<class TYPE> 
  tFunction0<TYPE> *tPow<TYPE>::Reduce( int argnr , tFunction0<TYPE> *s  ) const
  {  
    Heap<tFunction0<TYPE> > h;
    h += s->Clone();
    return new tPow<TYPE>( this->left->Reduce( argnr , s ) , this->right->Reduce( argnr , h.Pop() )  ); 
  }
  
  template<class TYPE> 
  tFunction0<TYPE> *tPow<TYPE>::AnalyticalDerivative( int argnr ) const
  {
    //    return new tConstant<TYPE>( 0.0 );
    //    return this->Clone();

    const tConstant<TYPE> *gconstant = dynamic_cast<const tConstant<TYPE> *>( this->right );
    
    if ( gconstant != NULL )
      return tFunction0<TYPE>::Mul( this->left->Getdydx( argnr )->Clone() ,
				    this->right->Clone() ,
				    new tPow<TYPE>( this->left->Clone() , 
						    tFunction0<TYPE>::Sub( this->right->Clone() , 
									   new tConstant<TYPE>( 1 )   ) 
						    ) ,
				    NULL );
				    
    

      return tFunction0<TYPE>::Mul( tFunction0<TYPE>::Add( 
							  tFunction0<TYPE>::Mul( this->right->Getdydx( argnr )->Clone() ,  
										 new tLn<TYPE>( this->left->Clone() ) ) , 
							  tFunction0<TYPE>::Div( tFunction0<TYPE>::Mul( this->right->Clone() , 
													this->left->Getdydx( argnr )->Clone() ) , 
										 this->left->Clone()  ) , 
							  0L ) , 
				  this->Clone() , 
				  0L
				  );
  }
  

  template<class TYPE> 
  TYPE tPow<TYPE>::y( const tnmmatrix<TYPE> &x ) const
  {
    TYPE yf,yg;
    yf = this->left->y( x );
    yg = this->right->y( x );
    return pow( yf , yg );
  }
 

 
}

template class P4th::tPow<float>;
template class P4th::tPow<double>;
