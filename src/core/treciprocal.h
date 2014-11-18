/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#ifndef _P4TH_TRECIPROCAL_H_
#define _P4TH_TRECIPROCAL_H_

#include <tatom.h>
#include <tsquare.h>
#include <tmul.h>
#include <tconstant.h>
#include <math.h>



namespace P4th
{


  template<class TYPE> 
    class tReciprocal : public tAtom<TYPE> 
  {
  protected:
  public:
    tReciprocal( tFunction0<TYPE> *_f  ) : tAtom<TYPE>( _f  ) 
    {}
    tReciprocal( const tReciprocal<TYPE> &source  ) : tAtom<TYPE>( source ) 
    {}
    virtual ~tReciprocal() 
    {} 
    
    virtual tFunction0<TYPE> *Clone() const
    { return new tReciprocal<TYPE>( *this  ); }
    virtual tFunction0<TYPE> *Reduce( int argnr , tFunction0<TYPE> *s  ) const
      {  return new tReciprocal<TYPE>( this->f->Reduce( argnr , s )  ); }
    
    virtual tFunction0<TYPE> *AnalyticalDerivative( int argnr ) const;
    
    virtual TYPE y( const tnmmatrix<TYPE> &x ) const
    {
      return (TYPE)1.0 / this->f->y( x );
    }

    virtual std::ostream &Dump( std::ostream &dest , string lm = "" ) const {
      dest << lm << __FILE__ << "(" << (void *)this << ")" << std::endl;
      lm += "\t";
      this->f != NULL ? this->f->Dump( dest , lm ) : dest << lm << "NULL" << std::endl;
      return dest;
    }
    
  };

    
}

#endif
