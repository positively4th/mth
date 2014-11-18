/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#include "tquadratic.h"

namespace P4th
{
  
  template<class TYPE>
  tQuadratic<TYPE>::tQuadratic( const TYPE &_c , 
				const tnmmatrix<TYPE> &_C ) :
    tDerivatives0<TYPE>()
  {
    C.Resize( _C.GetRows(),_C.GetCols() );
    SetMatrix( _C );
    c = _c;
  }

  template<class TYPE>
  tQuadratic<TYPE>::tQuadratic( const tQuadratic<TYPE> &source ) :
    tDerivatives0<TYPE>( source )
  {
    C.Resize( source.C.GetRows(),source.C.GetCols() );
    SetMatrix( source.C );
    c = source.c;
  }

  
  template<class TYPE>
  void tQuadratic<TYPE>::SetMatrix( tnmmatrix<TYPE> _C )
  {
    //    std::cout << "ROWS = " << C.GetRows() << ", COLS = " << C.GetCols() << std::endl;
    // Make C symmemtric!
    C.tnmContainer<TYPE>::Cpy( &_C );
    C.tnmContainer<TYPE>::Trp( &_C );
    C.Add( _C );
    C.Mul( 0.5 );
  }

  template<class TYPE>
  TYPE tQuadratic<TYPE>::y( const tnmmatrix<TYPE> &x ) const
  {
    tnmmatrix<TYPE> _res(1,1);
    tnmmatrix<TYPE> tmp( x.GetRows() , 1 );
    tnmmatrix<TYPE> xT( 1 , x.GetRows() );
    x.Trp( &xT );
    tmp.Mul( C , x );
    _res.Mul( xT , tmp );
    //    std::cout << "res = " << _res << std::endl;
    return c + (TYPE)_res;
  }

  template<class TYPE>
  tFunction0<TYPE> *tQuadratic<TYPE>::AnalyticalDerivative( int argnr ) const
  {
    tnmmatrix<TYPE> tmp(this->C.GetRows(),1);
    tnmmatrix<TYPE>::Get( tmp , this->C , 1 , argnr );
    return tFunction0<TYPE>::Mul( new tConstant<TYPE>( 2 ) ,
				  new tLinear<TYPE>( 0 , tmp ) ); 
      
  }

  
}

template class P4th::tQuadratic<float>;
template class P4th::tQuadratic<double>;
