/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#ifndef _P4TH_TFOG_H_
#define _P4TH_TFOG_H_

#include <tderivatives0.h>
#include <tfunctions.h>
#include <tadd.h>
#include <tmul.h>

//class istream;
//class ostream;


namespace P4th
{
  template<class TYPE> 
    class tIntegral;

  namespace Statistics {
    namespace Bivariate {
      template<class TYPE> 
	class tGaussianNCDF;
    }
  }

  template<class TYPE> 
    class tFoG : public tDerivatives0<TYPE>
  {
  public:
    virtual int Nodes() const;
  protected:
    tFunction0<TYPE> *f;
    tFunctions<TYPE> *g;


    
    tFoG( tFunction0<TYPE> *_f , tFunctions<TYPE> *_g );
    tFoG( tFunction0<TYPE> *_f , tFunction0<TYPE> *_g1 = 0L , tFunction0<TYPE> *_g2 = 0L ); 
  public:
    tFoG( const tFoG<TYPE> &source );
      
    virtual ~tFoG();

    virtual tFunction0<TYPE> *Reduce( int argnr , tFunction0<TYPE> *s  ) const;
    virtual tFunction0<TYPE> *Simplify();

    virtual void AddFunction( tFunction0<TYPE> *_g ); 
    virtual tFunction0<TYPE> *GetFunction( int nr )
    { return g->Get( nr ); }
    virtual int GetSize()
    { return g->GetSize(); }


    virtual const tFunction0<TYPE> *GetF() const
    { return f; }

    virtual const tFunctions<TYPE> *GetG() const
    { return g; }


    virtual tFoG<TYPE> *Clone() const
    { return new tFoG( *this ); }
    
    virtual bool DimOk( int d ) const
      {
	return g->DimOk( d );
      }


    virtual tFunction0<TYPE> *AnalyticalDerivative( int argnr ) const;


    virtual TYPE y( const tnmmatrix<TYPE> &x ) const;

    virtual std::ostream &Dump( std::ostream &dest , string lm = "" ) const;


    friend class tFunction0<TYPE>;
    friend class tIntegral<TYPE>;
    friend class Statistics::Bivariate::tGaussianNCDF<TYPE>;
  };




}


#endif
