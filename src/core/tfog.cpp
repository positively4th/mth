/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#include "tfog.h"

#include <tconstant.h>
#include <tparameter.h>

namespace P4th
{

  template<class TYPE> 
  tFoG<TYPE>::tFoG( tFunction0<TYPE> *_f , tFunctions<TYPE> *_g  ) : 
    tDerivatives0<TYPE>()
  {
    //    _DB( std::cout << "tFoG<TYPE>::tFoG( const tFunction0<TYPE> *_f , const tFunction0s<TYPE> *_g )" << std::endl; );
    f = _f;
    g = _g;
  }
  
  template<class TYPE> 
  tFoG<TYPE>::tFoG( tFunction0<TYPE> *_f , tFunction0<TYPE> *_g1 , tFunction0<TYPE> *_g2 ) : 
    tDerivatives0<TYPE>()
  {
    f = _f;
    g = new tFunctions<TYPE>;
    if ( _g1 != 0L )
      AddFunction( _g1 );
    if ( _g2 != 0L )
      AddFunction( _g2 );
  }

  
  template<class TYPE> 
  tFoG<TYPE>::tFoG( const tFoG<TYPE> &source ) : 
    tDerivatives0<TYPE>( source ) 
  {
      if ( source.f != 0L ) 
	f = source.f->Clone(); 
      else 
	f = 0L;
      g = source.g->Clone();
    }

  template<class TYPE> 
  tFoG<TYPE>::~tFoG() 
  {
    if ( f != 0L )
      delete f;
    if ( g != 0L )
      delete g;
  }

  template<class TYPE> 
  tFunction0<TYPE> *tFoG<TYPE>::Reduce( int argnr , tFunction0<TYPE> *s  ) const
  {
    if ( f != 0L )
      return tFunction0<TYPE>::Composite( this->f->Clone() , this->g->Reduce( argnr , s )  ); 
    return new tFoG<TYPE>( 0L , this->g->Reduce( argnr , s ) ); 
  }

  template<class TYPE> 
  tFunction0<TYPE> *tFoG<TYPE>::Simplify() 
  {
    for ( int j = g->GetSize() ; j >= 1  ; j-- )
      {
	tFunction0<TYPE> *c = NULL; 
	this->g->Replace( j , g->Get( j ) );
	
	if ( ! c )
	  c = dynamic_cast<tConstant<TYPE> *>( g->Get( j ) );
	if ( ! c )
	  c = dynamic_cast<tParameter<TYPE> *>( g->Get( j ) );
	if ( c ) {
	  //	  std::cout << "tFunction0<TYPE> *tFoG<TYPE>::Simplify(): Reducing " << j << std::endl;
	  g->Steal( j );
	  c = f->Reduce( j , c );
	  delete f;
	  f = c;
	}
      }
    f = f->Simplify();
    return this;
  }
  
  template<class TYPE> 
  int tFoG<TYPE>::Nodes() const
  {
    int res = 1;
    if ( f != 0L )
      res += f->Nodes();
    for ( int i = 1 ; i <= g->GetSize() ; i++ )
      res += this->g->Get( i )->Nodes(); 
    return res;
  }

  template<class TYPE> 
  void tFoG<TYPE>::AddFunction( tFunction0<TYPE> *_g )
  {
    if ( _g == 0L )
      _DBTHROW( "_g == 0L" );
    g->AddFunction( _g );
  }
  
  template<class TYPE> 
  tFunction0<TYPE> * tFoG<TYPE>::AnalyticalDerivative( int argnr ) const
  {
    Heap<tFunction0<TYPE> > h;

    
    tAdd<TYPE> *res = new tAdd<TYPE>;
    
    for ( int j = 1 ; j <= g->GetSize() ; j++ )
      {
	res->AddTerm( tFunction0<TYPE>::Mul( 
					    tFunction0<TYPE>::Composite( f->Getdydx( j )->Clone() , 
									 g->Clone()  ) , 
					    g->Get( j )->Getdydx( argnr )->Clone() ));
      }
    return res;
  }



  template<class TYPE> 
  TYPE tFoG<TYPE>::y( const tnmmatrix<TYPE> &x ) const
  {
    /*
    if ( ! f->DimOk( g->GetDimension() ) )
      _DBTHROW( "Dimension of the domain of f is less then the dimension of value range of g." ); 
    */

    tnmmatrix<TYPE> gy( g->GetDimension() , 1 );
      
    g->y( gy , x );
    
    return f->y( gy );
  }


  template<class TYPE> 
  std::ostream &tFoG<TYPE>::Dump( std::ostream &dest , string lm ) const 
  {
    dest << lm << __FILE__ << "(" << (void *)this << ")" << std::endl; 
    lm += "\t";
    dest << lm << "f:" << std::endl;
    if ( f != 0L )
      f->Dump( dest , lm + "\t"  ); 
    else
      dest << lm << "0L" << std::endl;
    dest << lm << "g:" << std::endl;
    g->Dump( dest , lm  + "\t" ); 
    return dest;
  }
  

  
}

template class P4th::tFoG<float>;
template class P4th::tFoG<double>;


