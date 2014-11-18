/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#ifndef _P4TH_TPARAMETER_H_
#define _P4TH_TPARAMETER_H_

#include "tfunction0.h"

//class istream;
//class ostream;


namespace P4th
{

  template<class TYPE> 
    class tParameter : public tFunction0<TYPE>
  {
  public:
    virtual int Nodes() const
    { return 1; }

  protected:
    static tConstant<TYPE> Df;
    
    const TYPE *c;

    virtual tFunction0<TYPE> *Reduce( int argnr , tFunction0<TYPE> *s  ) const
    { 
      if ( s != 0L )
	delete s;
      return new tParameter<TYPE>( c  ); 
      
    }
    
  public:
    tParameter( const TYPE *_c  ) : tFunction0<TYPE>() 
      { 
	c = _c;
      }
      tParameter( const tParameter<TYPE> &source ) : tFunction0<TYPE>( source ) 
	{
	  c = source.c;  
	}
	virtual ~tParameter()
	  {}

    virtual tParameter &operator=( const TYPE _c )
    {
      c = &_c;
      return *this;
    }
    
    virtual bool DimOk( int d ) const
    { return true; }

    virtual operator TYPE() const
    {
      return *c;
    }
    
    virtual tParameter<TYPE> *Clone() const
    { return new tParameter( *this ); }
    
    virtual void ResetDerivatives()
      {}
    virtual bool InitDerivatives( TYPE dx , int depth , int dim )
    { return true; }

    virtual const tFunction0<TYPE> *Getdydx( TYPE dx , int argnr , int argnr2 = 0L , ...  ) const;
    virtual tFunction0<TYPE> *AnalyticalDerivative( int argnr ) const
    { return Df.Clone(); }
    virtual void NumericalDerivatives() 
    {}
    virtual void AnalyticalDerivatives()
    {}
      

    virtual TYPE y( const tnmmatrix<TYPE> &x ) const
    { return *c; }

    virtual std::ostream &Dump( std::ostream &dest , string lm = "" ) const
    {
      dest << lm << __FILE__ << "(" << (void *)this << ")" << std::endl; 
      lm += "\t";
      dest << lm << c << " = " << *c << std::endl; 
      return dest;
    }
  };



}


#endif
