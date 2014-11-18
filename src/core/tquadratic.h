/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#ifndef _P4TH_TQUADRATIC_H_
#define _P4TH_TQUADRATIC_H_

#include <debug.h>
#include <texception.h>
#include <tderivatives0.h>
#include <tlinear.h>
#include <tmul.h>

//class istream;
//class ostream;


/*

y = cT x c

*/

namespace P4th
{

  template<class TYPE> 
    class tQuadratic : public tDerivatives0<TYPE>
  {
  public:
    virtual int Nodes() const
    { return 1; }
  protected:
    TYPE c;
    tnmmatrix<TYPE> C;

    virtual tFunction0<TYPE> *Reduce( int argnr , tFunction0<TYPE> *s  ) const
    { throw aException(tQuadratic<TYPE>, "Reduce not supported for tQuadratic!" ); }
  public:
    tQuadratic( const TYPE &_c , const tnmmatrix<TYPE> &_C ); 
    tQuadratic( const tQuadratic<TYPE> &source ); 
    virtual ~tQuadratic()
    {} 
    
    virtual void SetConstant( TYPE _c )
    { c = _c; }
    virtual void SetMatrix( tnmmatrix<TYPE> _C );
    
    virtual tQuadratic<TYPE> *Clone() const
    { return new tQuadratic( *this ); }

    virtual bool DimOk( int d ) const
      { if ( d < 0 || d > C.GetRows() ) return true; return false; }
    
    virtual tFunction0<TYPE> *AnalyticalDerivative( int argnr ) const;

    virtual TYPE y( const tnmmatrix<TYPE> &x ) const;

    virtual std::ostream &Dump( std::ostream &dest , string lm = "" ) const
    {
      dest << lm << __LINE__ << "(" << (void *)this << ")" << std::endl; 
      lm += "\t";
      dest << lm << "c = " << c << std::endl;
      dest << lm << "C :" << std::endl;
      C.Print( dest , lm  , 50 );
      return dest;
    }

  };



}


#endif
