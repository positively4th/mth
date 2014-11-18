/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#ifndef _P4TH_TNUMERICALDERIVATIVE_H_
#define _P4TH_TNUMERICALDERIVATIVE_H_

#include <tderivatives0.h>
#include <tnmmatrix.h>

//class istream;
//class ::std::ostream;

/*

Interpretation of h (dx )

h > 0   -> h is an absolute number
h < 0   -> h relative, i.e. dx = (-h) * x
h = NaN -> h = sqrt(eps) * x

*/

namespace P4th
{

  template<class TYPE> 
    class tNumericalDerivative : public tDerivatives0<TYPE> 
  {
  public:
    virtual int Nodes() const
    { return 1 + GetPrimitive()->Nodes(); }
  protected:
    int argnr;
    tFunction0<TYPE> *deletablef;
    const tFunction0<TYPE> *nondeletablef;
    
    bool isowner;
    TYPE h;
    virtual tFunction0<TYPE> *Reduce( int argnr , tFunction0<TYPE> *s  ) const;
    
    
    tNumericalDerivative( int argnr , const tFunction0<TYPE> *nondeletable , 
			  tFunction0<TYPE> *deletable , TYPE _d = (TYPE)0.0 );

    const tFunction0<TYPE> *GetPrimitive() const
      { if ( deletablef != 0L ) return deletablef; return nondeletablef; }
  public:
    tNumericalDerivative( const tNumericalDerivative &source );
    virtual ~tNumericalDerivative();
    
    virtual tNumericalDerivative<TYPE> *Clone() const
    { return new tNumericalDerivative( *this  ); }

    virtual bool DimOk( int d ) const
    { if ( GetPrimitive()->DimOk( d )) return true; return false; }
    
    virtual tFunction0<TYPE> *AnalyticalDerivative( int argnr ) const; 

    virtual TYPE y( TYPE res , const tnmmatrix<TYPE> &x ) const;
    
    virtual TYPE Geth() const
    { return h; }
    virtual TYPE y( const tnmmatrix<TYPE> &x ) const
    { TYPE res; return y( res , x ); }

    virtual std::ostream &Dump( std::ostream &dest , string lm = "" ) const; 


    
    friend class tDerivatives0<TYPE>;
  };


}


#endif
