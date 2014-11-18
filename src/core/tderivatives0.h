/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#ifndef _P4TH_TDERIVATIVES0_H_
#define _P4TH_TDERIVATIVES0_H_

#include <tfunction0.h>
#include <tfunctions.h>
#include <tnmmatrix.h>
#include <linkedptrlist.h>
#include <string>

//class istream;
//class ::std::ostream;


namespace P4th
{
  template<class TYPE>
    class tfMatrix;

  template<class TYPE>
    class tDerivatives0 : public tFunction0<TYPE>
  {
  public:
  protected:
    mutable tFunctions<TYPE> dydxs;
    void PrepareFor( int argnr ) const;
    //    bool analytical;
  public:
    tDerivatives0();
    tDerivatives0( const tDerivatives0<TYPE> &source  );
    virtual ~tDerivatives0();

    virtual void ResetDerivatives();
    virtual bool InitDerivatives( TYPE dx , int depth , int dim );
    virtual const tFunction0<TYPE> *Getdydx( TYPE dx , int argnr , int argnr2 = 0L , ...  ) const;
    virtual const tFunction0<TYPE> *Setdydx( int argnr , tFunction0<TYPE> *dfdx  ) const;
    /*
    virtual void NumericalDerivatives()
    { analytical = false;  }
    virtual void AnalyticalDerivatives()
    { analytical =true; }
    */
    virtual tFunction0<TYPE> *NumericalDerivative( int argnr , const TYPE  &dx ) const;
    virtual tFunction0<TYPE> *AnalyticalDerivative( int argnr ) const = 0;


 public:
    virtual std::ostream &Dump( std::ostream &dest , string lm = "" ) const;




  };



}


#endif
