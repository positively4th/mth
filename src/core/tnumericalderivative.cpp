/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#include "tnumericalderivative.h"

#include <debug.h>
#include <math.h>
#include "tconstant.h"
#include "ttranslate.h"
#include "tmathtools.h"



namespace P4th
{

  template<class TYPE> 
  tNumericalDerivative<TYPE>::tNumericalDerivative( int anr  , const tFunction0<TYPE> *nondeletable , 
						    tFunction0<TYPE> *deletable , TYPE _h ) :
    tDerivatives0<TYPE>()
  {
    if ( _h == 0 )
      h = tFunction0<TYPE>::Epsilon();
    else
      h = _h;

    argnr = anr;
    deletablef = deletable;
    nondeletablef = nondeletable;
  }

  template<class TYPE> 
  tNumericalDerivative<TYPE>::tNumericalDerivative( const tNumericalDerivative &source ) :
    tDerivatives0<TYPE>()
  {
    h = source.h;
    argnr = source.argnr;
    nondeletablef = source.nondeletablef;
    if ( source.deletablef != 0L )
      deletablef = source.deletablef->Clone();
    else
      deletablef = source.deletablef;

  }

  template<class TYPE> 
  tNumericalDerivative<TYPE>::~tNumericalDerivative()
  {
    if ( deletablef != 0L )
      delete deletablef;
  }

  template<class TYPE> 
  tFunction0<TYPE> *tNumericalDerivative<TYPE>::AnalyticalDerivative( int argnr ) const
  {
    _DBTHROW( "Check!!!!" );

    return tFunction0<TYPE>::Div( tFunction0<TYPE>::Sub( new tTranslate<TYPE>( this->Clone() , argnr , 
									       this->_EPSILON ) ,
							 new tTranslate<TYPE>( this->Clone() , argnr , 
									       - this->_EPSILON ) ) ,
				  new tConstant<TYPE>( this->_EPSILONx2 ) );
    
    
  }


  template<class TYPE> 
  tFunction0<TYPE> *tNumericalDerivative<TYPE>::Reduce( int argnr , tFunction0<TYPE> *s  ) const
  {  
    if ( argnr == this->argnr )
      _DBTHROW( "cannot reduce changing variable variable." );

    if ( argnr > this->argnr )
      return new tNumericalDerivative<TYPE>( this->argnr , 0L , GetPrimitive()->Reduce( argnr , s )  ); 
    else
      return new tNumericalDerivative<TYPE>( this->argnr - 1 , 0L , GetPrimitive()->Reduce( argnr , s )  ); 


  }

  template<class TYPE> 
  TYPE tNumericalDerivative<TYPE>::y( TYPE res , const tnmmatrix<TYPE> &x ) const
  {
    //    _DB( std::cout << "TYPE &tNumericalDerivative<TYPE>::y( TYPE &res , const tnmmatrix<TYPE> &x ) const" << std::endl; );
    tnmmatrix<TYPE> xtmp( x );
    
    /*
    _DB( x.PrintTrp( std::cout , "x: \t" ); );
    _DB( std::cout << "argnr = " << argnr << std::endl;  );
    */

    volatile TYPE add;
#ifdef _SAFE
    if ( h == 0 )
      _DBTHROW( "h == 0" );
#endif
    if ( tMathTools<TYPE>::IsNaN( h ) )
      {
	//	_DB( std::cout << "h is NaN" << std::endl; );
	add = sqrt( tFunction0<TYPE>::Epsilon() ) * xtmp.Ref(argnr,1);
	//	_DB( std::cout << "add = " << add << ", epsilon = " << tFunction0<TYPE>::Epsilon() << std::endl; );
	if ( tMathTools<TYPE>::Abs( (TYPE)add ) <= tFunction0<TYPE>::Epsilon() )
	  add = sqrt( tFunction0<TYPE>::Epsilon() );
      }
    else if ( h < 0 )
      {
	//	_DB( std::cout << "h is < 0" << std::endl; );
	add = tMathTools<TYPE>::Max( -h , -h * xtmp.Ref(argnr,1) );
      }
    else
      {
	//	_DB( std::cout << "h is > 0" << std::endl; );
	add = h;
      }
    //    _DB( std::cout << "add = " << add << std::endl; );

    // See http://en.wikipedia.org/wiki/Numerical_differentiation
    volatile TYPE tmp = xtmp.Ref(argnr,1) + add;
    //    _DB( std::cout << "tmp = " << tmp << std::endl; );
    add = tmp -  xtmp.Ref(argnr,1);

    //    _DB( std::cout << "add = " << add << std::endl; );
    //    _DB( xtmp.PrintTrp( std::cout , "x1:\t " ); );
    this->OnChangex( xtmp );
    res = GetPrimitive()->y( xtmp );
    //    _DB( std::cout << "res = " << res << std::endl; );
    xtmp.Ref(argnr,1) -= add + add;
    //    _DB(     xtmp.PrintTrp( std::cout , "x0:\t " ); );

    this->OnChangex( xtmp );
    res -= GetPrimitive()->y( xtmp );

    //    _DB( std::cout << "res = " << res << std::endl; );
    
    res = res / ( add + add );
    //    _DB( std::cout << "res = " << res << std::endl; );
    return res;
  }

  template<class TYPE> 
  std::ostream &tNumericalDerivative<TYPE>::Dump( std::ostream &dest , string lm ) const
  {
    dest << lm << __FILE__ << "(" << (void *)this << ")" << std::endl; 
    lm += "\t";
    dest << lm << "argnr = " << argnr << ". primitive = 0x" << (void *)GetPrimitive() << std::endl;
    return dest;
  } 

  
}


template class P4th::tNumericalDerivative<float>;
template class P4th::tNumericalDerivative<double>;



