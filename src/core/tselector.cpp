/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#include "tselector.h"

#include <stdarg.h>
#include <debug.h>
#include <math.h>
#include <string>
#include <stlstringtools.h>

#include "tconstant.h"


namespace P4th
{

  template<class TYPE> 
  tConstant<TYPE> tSelector<TYPE>::zero( (TYPE)0.0 );

  template<class TYPE> 
  tConstant<TYPE> tSelector<TYPE>::unity( (TYPE)1.0 );
  
  template<class TYPE> 
  tSelector<TYPE>::tSelector( int _nr  ) : tDerivatives0<TYPE>::tDerivatives0()
  { 
    nr = _nr; 
  }

  template<class TYPE> 
  tFunction0<TYPE> *tSelector<TYPE>::Reduce( int argnr , tFunction0<TYPE> *s  ) const
  {
    //    _DB( std::cout << "tFunction0<TYPE> *tSelector<TYPE>::Reduce( int argnr , tFunction0<TYPE> *s  ) const" << std::endl; );
      if ( ! this->DimOk( argnr ) || argnr < 1 )
	_DBTHROW( "! DimOk( argnr) || argnr < 1" );
      if ( this->nr == argnr ) 
	{
	  //	  _DB( std::cout << "this->nr == argnr" << std::endl; );
	  return s; 
	}
      if ( s != NULL )
	delete s;
      if ( argnr < this->nr )
	return new tSelector( nr - 1 );
      return new tSelector( nr );
      }
  

  template<class TYPE> 
  const tFunction0<TYPE> *tSelector<TYPE>::Getdydx( TYPE dx , int argnr , int argnr2 , ...  ) const
  {
    //	_DB( std::cout << "const tFunction0<TYPE> *tSelector<TYPE>::Getdydx( TYPE dx , int argnr , int argnr2  , ...  )" << std::endl; );


#ifdef _SAFE
    if ( argnr < 0  )
      _DBTHROW( "tFunction<TYPE> *tDerivatives0<TYPE>::Getdydx( TYPE dx , int argnr , int argnr2  , ... ) : argnr < 1" );
#endif

	const tFunction0<TYPE> *res = this;

	if ( argnr > 0 )
	  if ( argnr == nr )
	    res = &unity;
	  else
	    return &zero;
	

	int tmp = argnr2;
	if ( argnr2 < 0 )
	  return res;

	va_list arglist;
	va_start(arglist,argnr2);

	
	

	for ( ;; )
	  {

	    if ( tmp != 0 )
	      {
		if ( res == this )
		  {
		    if ( tmp == nr )
		      res = &unity;
		    else
		      res = &zero;
		  }
		if ( res == &unity )
		  {
		    res = &zero;
		  }
	      }
	    tmp = va_arg(arglist, int );
	    if ( tmp < 0 )
	      break;
	  }
	va_end( arglist );

	return res;
      } 



  
}

template class P4th::tSelector<float>;
template class P4th::tSelector<double>;



