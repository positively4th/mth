/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#ifndef _P4TH_TSQRT_H_
#define _P4TH_TSQRT_H_

#include <tatom.h>
#include <tmul.h>
#include <math.h>

//class istream;
//class ::std::ostream;


namespace P4th
{
  
  template<class TYPE> 
    class tSqrt : public tAtom<TYPE> 
  {
  protected:
  public:
    tSqrt( tFunction0<TYPE> *_f  ) : 
      tAtom<TYPE>( _f  ) 
      {} 
      tSqrt( const tSqrt<TYPE> &source  ) : tAtom<TYPE>( source ) 
	{}
	virtual ~tSqrt() 
	  {} 
	virtual tSqrt *Clone() const 
	{ return new tSqrt<TYPE>( *this ); }
	tFunction0<TYPE> *Reduce( int argnr , tFunction0<TYPE> *s  ) const
	  {  return new tSqrt<TYPE>( this->f->Reduce( argnr , s )  ); }
	
	virtual TYPE y( const tnmmatrix<TYPE> &x ) const
	{
	  TYPE res = this->f->y( x );
	  return sqrt( res );
	}
	
	virtual tFunction0<TYPE> *AnalyticalDerivative( int argnr ) const;
	
	virtual std::ostream &Dump( std::ostream &dest , string lm = "" ) const
	{
	  dest << lm << __FILE__ << "(" << (void *)this << ")" << std::endl; 
	  lm += "\t";
	  this->f != NULL ? this->f->Dump( dest , lm ) : dest << lm << "NULL" << std::endl;
	  return dest;
	}
    
  };


}


#endif
