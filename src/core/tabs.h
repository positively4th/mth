/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#ifndef _P4TH_TABS_H_
#define _P4TH_TABS_H_

#include <tatom.h>
#include <treciprocal.h>
#include <math.h>
#include <algorithm>

//class istream;
//class ::std::ostream;


namespace P4th
{

  template<class TYPE> 
    class tAbs : public tAtom<TYPE> 
  {
  protected:
  public:
    tAbs( tFunction0<TYPE> *_f ) : tAtom<TYPE>( _f ) 
    {} 
    virtual ~tAbs() 
    {} 
    tAbs( const tAbs<TYPE> &source  ) : tAtom<TYPE>( source ) 
    {}
    
    virtual tAbs *Clone() const
    { return new tAbs( *this  ); }
    virtual tFunction0<TYPE> *Reduce( int argnr , tFunction0<TYPE> *s  ) const
    {  return new tAbs<TYPE>( this->f->Reduce( argnr , s )  ); }
    
    virtual tFunction0<TYPE> *AnalyticalDerivative( int argnr ) const;

    virtual TYPE y( const tnmmatrix<TYPE> &x ) const
    {
      TYPE res = this->f->y( x );
      if ( res >= 0 )
	return res;
      return -res;
    }

    virtual std::ostream &Dump( std::ostream &dest , string lm = "" ) const
    {
      dest << __FILE__ << "(" << (void *)this << ")" << std::endl;
      lm += "\t";
      this->f != NULL ? this->f->Dump( dest , lm ) : dest << lm << "NULL" << std::endl;
      return dest;
    }
    
  };


}


#endif
