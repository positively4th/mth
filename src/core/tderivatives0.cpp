/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#include "tderivatives0.h"

#include <stdarg.h>
#include <debug.h>
#include <math.h>
#include <string>
#include <stlstringtools.h>

#include <stdarg.h>

#include <tnumericalderivative.h>
#include <tmathtools.h>
#include <tadd.h>
#include <tsub.h>
#include <tmul.h>


#include "tfmatrix.h"


namespace P4th
{

  template<class TYPE> 
  tDerivatives0<TYPE>::tDerivatives0()
  {
  }

  template<class TYPE> 
  tDerivatives0<TYPE>::tDerivatives0( const tDerivatives0<TYPE> &source  ) :
    dydxs( source.dydxs )
  {
    //    analytical = source.analytical;
  }

  template<class TYPE> 
  tDerivatives0<TYPE>::~tDerivatives0()
  {
    //    _DB( std::cout << "tDerivatives0<TYPE>::~tDerivatives0()" << std::endl; );

    for ( int i = 1 ; i <= dydxs.GetSize() ; i++ )
      {
	if ( dydxs.Get( i ) != 0L )
	  delete dydxs.Get( i );
      }
    dydxs.Delete();
    /*
    dydxs.Free();
    dydxs.Delete();
    */
  }

  template<class TYPE> 
  inline void tDerivatives0<TYPE>::PrepareFor( int argnr ) const
  {
    while ( dydxs.GetSize() < argnr )
      dydxs.AddFunction( (tFunction0<TYPE> *)0L );
  }


  template<class TYPE> 
  bool tDerivatives0<TYPE>::InitDerivatives( TYPE dx , int depth , int dim )
  {
    //    this->Dump( std::cout , "InitD:\t " );
    if ( depth <= 0 )
      return dim;
    
    int i = 1;
    while ( this->DimOk( i ) && i <= dim )
      {
	//	_DB( std::cout << "Adding numerical derivative for argument " << i << std::endl; );
	
	this->Getdydx( dx , i , -1 );
	i++;
      }
    
    if ( i <= dim )
      return false;
    return this->dydxs.InitDerivatives( dx , depth - 1 , dim );
  }

  template<class TYPE> 
  void tDerivatives0<TYPE>::ResetDerivatives()
  {
    dydxs.Free();
  }
  
  template<class TYPE> 
  tFunction0<TYPE> *tDerivatives0<TYPE>::NumericalDerivative( int argnr , const TYPE &dx ) const
  {
    return new tNumericalDerivative<TYPE>( argnr , 0L , this->Clone() , dx );
    //    return new tNumericalDerivative<TYPE>( argnr , this , 0L , dx );
  }


  template<class TYPE> 
  const tFunction0<TYPE> *tDerivatives0<TYPE>::Getdydx( TYPE dx , int argnr , int argnr2  , ...  ) const
  {
    //    _DB( std::cout << "dx = " << dx << std::endl; );
    if ( argnr2 > 0 )
      {

	//	_DB( std::cout << "const tFunction0<TYPE> *tDerivatives0<TYPE>::Getdydx( TYPE dx , int argnr , int argnr2  , ...  )" << std::endl; );
	LinkedList<int> argnrlist;
	
	argnrlist.Add( argnr2 );
	va_list arglist;
	va_start(arglist,argnr2);

	
	int tmp;
	for ( ;; )
	  {
	    tmp = va_arg(arglist, int );
	    if ( tmp < 0 )
	      break;
	    
	    argnrlist.Add( tmp );
	  }
	va_end( arglist );

	//	_DB( std::cout << "* * * * * * Getdydx * * * * * )" << std::endl; );
	//	_DB( this->Dump( std::cout , (string)"D0" + ":" ); );
	tFunction0<TYPE> *tmpf = (tFunction0<TYPE> *)(void *)this->Getdydx( dx , argnr ); 
	//	_DB( tmpf->Dump( std::cout , (string)"D1" + ":" ); );
	for ( int i = 0 ; i < argnrlist.GetSize() ; i++ )
	  {
	    tmpf = (tFunction0<TYPE> *)(void *)tmpf->Getdydx( dx , argnrlist.Get( i ) );
	    //	    _DB( tmpf->Dump( std::cout , (string)"D" + STLStringTools::Int2String( i + 2 ) + ":" ); );
	  }
	return tmpf;
      } 


#ifdef _SAFE
    if ( argnr < 0  )
      _DBTHROW( "tFunction<TYPE> *tDerivatives0<TYPE>::Getdydx( TYPE dx , int argnr , int argnr2  , ... ) : argnr < 1" );
#endif
    if ( argnr < 1 )
      return this;

    if ( argnr > dydxs.GetSize() )
      PrepareFor( argnr );

    tFunction0<TYPE> *df = dydxs.Get( argnr ); 
    if ( df != NULL )
      {
	//	_DB( std::cout << "Derivative exists!" << std::endl; );
	TYPE currentdx = 0;
	
	tNumericalDerivative<TYPE> *currentD = dynamic_cast<tNumericalDerivative<TYPE> *>( df );
	if ( currentD != NULL ) 
	  {
	    //	    _DB( std::cout << "Existing derivative is numerical!" << std::endl; );
	    currentdx = currentD->Geth();
	  } 
	else {
	  //	  _DB( std::cout << "Existing derivative is analytical!" << std::endl; );
	}

	// NaN implies default dx in tnumerical derivative, do not replace if new and old is nan	
	if ( ! ( tMathTools<TYPE>::IsNaN( dx ) &&  tMathTools<TYPE>::IsNaN( currentdx ) )  ) 
	  if ( dx != currentdx )
	    {
	      _DB( std::cout << "Replacing derivative " << argnr << std::endl; );
	      dydxs.Replace( argnr , NULL );
	      //	    delete df;
	      df = NULL;
	  }
      }

    if ( df == NULL )
      {
	Heap<tFunction0<TYPE> > h;
	if ( dx == 0 )
	  {
	  //	  h += this->NumericalDerivative( argnr );
	    h += this->AnalyticalDerivative( argnr );
	  } else {
	  h += this->NumericalDerivative( argnr , dx );
	}
	if ( h.Last() == 0L )
	  _DBTHROW( "tDerivatives0<TYPE>::dydx( ... ) failed!" );
	dydxs.Replace( argnr , h.Pop() );
      }

    return dydxs.Get( argnr );


  }




  template<class TYPE> 
  const tFunction0<TYPE> *tDerivatives0<TYPE>::Setdydx( int argnr , tFunction0<TYPE> *dfdx  ) const
  {

#ifdef _SAFE
    if ( argnr < 0  )
      _DBTHROW( "tFunction<TYPE> *tDerivatives0<TYPE>::Getdydx( TYPE dx , int argnr , int argnr2  , ... ) : argnr < 1" );
#endif
    if ( argnr < 1 )
      return this;

    if ( argnr > dydxs.GetSize() )
      PrepareFor( argnr );

    _DB( 
	if ( dydxs.Get( argnr ) != NULL )
	  std::cout << "Relpacing " << (void *)dydxs.Get( argnr ) << " with " << (void *)dfdx << std::endl;  
	);


    dydxs.Replace( argnr , dfdx  );
    return dfdx;

  }



  template<class TYPE> 
  std::ostream &tDerivatives0<TYPE>::Dump( std::ostream &dest , string lm  ) const 
  {
    dest << lm << __FILE__ << "(" << (void *)this << ")" << std::endl;
    lm = lm + "\t";
    for ( int i = 1 ; i <=  dydxs.GetSize() ; i++ )
      {
	dest << lm << "Derivative " << i << std::endl;
	dydxs.Get( i )->Dump( dest , lm );
      }
    return dest;
  }




}

template class P4th::tDerivatives0<float>;
template class P4th::tDerivatives0<double>;



