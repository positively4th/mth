/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#ifndef _P4TH_TINTEGRAL_H_
#define _P4TH_TINTEGRAL_H_

#include <tderivatives0.h>
#include <tfog.h>
#include <tselector.h>
#include <tnmmatrix.h>
#include <string>


namespace P4th
{

  template<class TYPE> 
    class tIntegral;

  template<class TYPE> 
    class tFunction0;


  template<class TYPE> 
    class tIntegralSelector : public tDerivatives0<TYPE>
  {
  public:
    const static int _REDUCENR = 0x7fffffff;
    virtual int Nodes() const
    { return 1; }
  protected:
    virtual tFunction0<TYPE> *Reduce( int argnr , tFunction0<TYPE> *s  ) const
    { if ( argnr == _REDUCENR ) return s; delete s; return this->Clone(); }
  public:
    tIntegralSelector()
      {}
    
    tIntegralSelector( const tIntegralSelector<TYPE> &source  ) : tDerivatives0<TYPE>( source )
      {}
    
    virtual tIntegralSelector *Clone() const
    { return new tIntegralSelector<TYPE>( *this ); }

    virtual bool DimOk( int d ) const
      { if ( d >= 1 ) return true; return false; }

    virtual tFunction0<TYPE> *AnalyticalDerivative( int argnr ) const
    { return new tConstant<TYPE>( 0.0 ); }
    
    virtual TYPE y( const tnmmatrix<TYPE> &x ) const
    { return x.Get( x.GetRows() , 1 ); }


    virtual std::ostream &Dump( std::ostream &dest , string lm = "" ) const
    { 
      dest << lm << __FILE__ << "(" << (void *)this << ")" << std::endl; 

      return dest;
    }

    
    
  };


  /*

  The definite integral

  F(x) = I_[lb(x)]^[ub[x]] f(y(z)) dz_i

  F:  n -> 1
  lb: n -> 1
  ub: n -> 1

  y: n -> m
  f: m -> 1

  */
  template<class TYPE> 
    class tIntegral : public tDerivatives0<TYPE>  
  {
  public:
    const static int _MAGICSELECTOR = 0;
    const static int _MAXINTERVALS = 1000;
    const static double _MAXABSOLUTEERROR = 0.01;
    const static double _MAXRELATIVEERROR = 0.01;
    virtual int Nodes() const
    { return 1 + f->Nodes() + lb->Nodes() + ub->Nodes(); }
  protected:
    //    int argnr;


    tFunction0<TYPE> *f;
    tFunction0<TYPE> *lb;
    tFunction0<TYPE> *ub;
    
    // Use tFunction0<TYPE>::Integrate instead 
  public:
    tIntegral( const tnmmatrix<int> &mapping , 
	       tFunction0<TYPE> *_f ,  
	       tFunction0<TYPE> *_lb , tFunction0<TYPE> *_ub , 
	       int maxints = -1 , 
	       TYPE maxabserr = -1 , 
	       TYPE maxrelerr = -1 );
    tIntegral( const tnmmatrix<int> &mapping , 
	       tFunction0<TYPE> *_f ,  
	       TYPE _lb , TYPE _ub ,
	       int maxints = -1 , 
	       TYPE maxabserr = -1 , 
	       TYPE maxrelerr = -1 );
    tIntegral( tFunction0<TYPE> *_foy , 
	       tFunction0<TYPE> *_lb , 
	       tFunction0<TYPE> *_ub , 
	       int maxints , 
	       TYPE maxabserr , 
	       TYPE maxrelerr 
	       );


    tIntegral( const tIntegral<TYPE> &source );
    tnmmatrix<TYPE> *activex;

    TYPE maxabsoluteerror;
    TYPE maxrelativeerror;
    int maxintervals;
    virtual ~tIntegral();
    
    virtual tFunction0<TYPE> *Reduce( int argnr , tFunction0<TYPE> *s  ) const;

    virtual void SetMaxInterval( int mi )
    { maxintervals = mi; }
    virtual void SetAbsoluteAccruancy( int aa )
    { maxabsoluteerror = aa; }
    virtual void SetRelativeAccruancy( int ra )
    { maxrelativeerror = ra; }

    virtual tIntegral<TYPE> *Clone() const
    { return new tIntegral( *this ); }

    virtual bool DimOk( int d ) const
    { if ( lb->DimOk( d ) && ub->DimOk( d ) ) 
	return true; 
      return false; 
      }
    
    virtual tFunction0<TYPE> *AnalyticalDerivative( int argnr ) const;

    virtual TYPE y( const tnmmatrix<TYPE> &x  ) const;
    

    virtual std::ostream &Dump( std::ostream &dest , string lm = "" ) const; 


    static double integrand( double *xi , void *_this_ )
      {
	tIntegral<TYPE> *_this = dynamic_cast<tIntegral<TYPE> *>( (tIntegral<TYPE> *)_this_ );
	if ( _this == 0L )
	  _DBTHROW( "_this == 0L" );
	TYPE res;
	
	_this->activex->Set( _this->activex->GetRows() , 1 , (TYPE)(*xi) );
	res =_this->f->y(  (*_this->activex) );
	/*	
	_DB( _this->activex->PrintTrp( std::cout , "x: \t" ); );
	_DB( std::cout << "y = " << res << std::endl; );
	*/
	return (double)res;
      }

    friend class tFunction0<TYPE>;
  };


}


#endif
