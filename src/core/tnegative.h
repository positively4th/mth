/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#ifndef _P4TH_TNEGATIVE_H_
#define _P4TH_TNEGATIVE_H_

#include <tatom.h>
#include <tsquare.h>
#include <tmul.h>
#include <tconstant.h>
#include <math.h>



namespace P4th
{
  template<class TYPE> 
    class tFunction0; 


  template<class TYPE> 
    class tNegative : public tAtom<TYPE> 
  {
  protected:
    tNegative( tFunction0<TYPE> *_f  ) : tAtom<TYPE>( _f ) 
    {}
  public:
    tNegative( const tNegative<TYPE> &source  ) : tAtom<TYPE>( source ) 
    {}
    virtual ~tNegative() 
    {} 
    
    virtual tFunction0<TYPE> *Clone() const
    { return new tNegative<TYPE>( *this  ); }
    tFunction0<TYPE> *Reduce( int argnr , tFunction0<TYPE> *s  ) const
      {  return new tNegative<TYPE>( this->f->Reduce( argnr , s )  ); }
    
    virtual tFunction0<TYPE> *AnalyticalDerivative( int argnr ) const;
    
    virtual TYPE y( const tnmmatrix<TYPE> &x ) const
    {
      return - this->f->y( x );
    }

    virtual std::ostream &Dump( std::ostream &dest , string lm = "" ) const
    {
      dest << lm << __FILE__ << "(" << (void *)this << ")" << std::endl;
      lm += "\t";
      this->f != NULL ? this->f->Dump( dest , lm ) : dest << lm << "NULL" << std::endl;
      return dest;
    }


    friend class tFunction0<TYPE>;
  };

    
}

#endif
