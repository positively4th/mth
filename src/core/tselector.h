/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#ifndef _P4TH_TSELECTOR_H_
#define _P4TH_TSELECTOR_H_

#include "tfunction0.h"
#include "tderivatives0.h"
#include <iostream>
#include <string>

//class istream;
//class ::std::ostream;


namespace P4th
{

  template<class TYPE> 
    class tSelector : public tDerivatives0<TYPE>
  {
  public:
    virtual int Nodes() const
    { return 1; }
  protected:
    static tConstant<TYPE> zero;
    static tConstant<TYPE> unity;

    int nr;
    virtual tFunction0<TYPE> *Reduce( int argnr , tFunction0<TYPE> *s  ) const;
  public:
    tSelector( int _nr  );
    
    tSelector( const tSelector<TYPE> &source  ) : tDerivatives0<TYPE>( source )
    { nr = source.nr; }
    
    virtual tSelector *Clone() const
    { return new tSelector<TYPE>( *this ); }

    virtual int GetNr() const
    { return nr; }
    virtual void SetNr(int nr)
    { this->nr = nr; }

    virtual bool DimOk( int d ) const
      { if ( d >= 1 ) return true; return false; }
    virtual void Dim( int d ) 
    {}

    virtual void ResetDerivatives()
      {}
    virtual bool InitDerivatives( TYPE dx , int depth , int dim )
    { return true;}

    virtual tFunction0<TYPE> *AnalyticalDerivative( int argnr ) const
    { 
      if ( argnr == nr )
	return unity.Clone();
      return zero.Clone();
    }
    
    virtual const tFunction0<TYPE> *Getdydx( TYPE dx , int argnr , int argnr2 = 0L , ...  ) const;

    
    virtual TYPE y( const tnmmatrix<TYPE> &x ) const
    { 
      TYPE res = 0;
      if ( x.GetRows() >= nr  )
	res = x.Get(nr,1);
      return res; 
    }


    virtual std::ostream &Dump( std::ostream &dest , string lm = "" ) const
    { 
      dest << lm << __FILE__ << "(" << (void *)this << ")" << std::endl; 
      lm += "\t";
      dest << lm << "arg_" << nr << std::endl;

      return dest;
    }

    
    
  };


}


#endif
