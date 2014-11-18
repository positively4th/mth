/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#ifndef _P4TH_TLINEAR_H_
#define _P4TH_TLINEAR_H_

#include <debug.h>
#include "tderivatives0.h"
#include "tconstant.h"

//class istream;
//class ostream;


/*

y = a + b . x

*/

namespace P4th
{

  template<class TYPE> 
    class tLinear : public tDerivatives0<TYPE>
  {
  public:
    virtual int Nodes() const
    { return 1; }
  protected:
    TYPE a;
    tnmmatrix<TYPE> b;

    virtual tFunction0<TYPE> *Reduce( int argnr , tFunction0<TYPE> *s  ) const
    { _DBTHROW( "Reduce not supported for tLinear!" ); }
  public:
    tLinear( const TYPE &_a , const tnmmatrix<TYPE> &_b ) : 
    tDerivatives0<TYPE>() , b( _b )  
    { 
      a = _a;  
    }
    tLinear( const tLinear<TYPE> &source ) : 
      tDerivatives0<TYPE>( source ) , b( source.b )  
    { 
      a = source.a;   
    }
    virtual ~tLinear()  
    {}
    
    virtual tLinear<TYPE> *Clone() const
    { return new tLinear( *this ); }
    
    virtual bool DimOk( int d ) const
      { if ( d < 0 || d > b.GetRows() ) return true; return false; }
    
    virtual tFunction0<TYPE> *AnalyticalDerivative( int argnr ) const
    {
      return new tConstant<TYPE>( this->b.Get(argnr,1) );
    }

    virtual TYPE y( const tnmmatrix<TYPE> &x ) const
    { return a + b.Dot( x ); }
    
    virtual std::ostream &Dump( std::ostream &dest , string lm = "" ) const
    {
      dest << lm << __LINE__ << "(" << (void *)this << ")" << std::endl; 
      lm += "\t";
      dest << lm << "a = " << a << std::endl; 
      b.PrintTrp( dest , lm , 40 );
      return dest;
    }

  };



}


#endif
