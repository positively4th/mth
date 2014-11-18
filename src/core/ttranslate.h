/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#ifndef _P4TH_TTRANSLATE_H_
#define _P4TH_TTRANSLATE_H_

#include "tatom.h"
#include "tmul.h"
#include "tconstant.h"
#include <math.h>

//class istream;
//class ::std::ostream;


namespace P4th
{

  template<class TYPE> 
    class tTranslate : public tAtom<TYPE> 
  {
  protected:
    int argnr;
    TYPE delta;
    tFunction0<TYPE> *Reduce( int argnr , tFunction0<TYPE> *s  ) const
      {  return new tTranslate<TYPE>( this->f->Reduce( argnr , s ) , this->argnr , this->delta ); }
  public:
    tTranslate( tFunction0<TYPE> *f , int anr , TYPE d ) : tAtom<TYPE>( f ) 
      { argnr = anr; delta = d; }
      tTranslate( const tTranslate<TYPE> &source  ) : tAtom<TYPE>( source ) 
	{ argnr = source.argnr; delta = source.delta; }
	virtual ~tTranslate() 
	  {}
	
	virtual tFunction0<TYPE> *Clone() const
	{ return new tTranslate<TYPE>( *this ); }
	
	virtual tFunction0<TYPE> *AnalyticalDerivative( int dargnr ) const;
	virtual TYPE y( const tnmmatrix<TYPE> &x ) const
	{
	  TYPE res;
	  tnmmatrix<TYPE> xx( x );
	  xx.Ref( argnr , 1 ) =  x.Get(1,argnr) + delta;
	  res = this->f->y( xx );
	  return res; 
	}

	virtual std::ostream &Dump( std::ostream &dest , string lm = "" ) const {
	  dest << __FILE__ << "(" << (void *)this << ")" << std::endl;
	  lm += "\t";
	  this->f != NULL ? this->f->Dump( dest , lm ) : dest << lm << "NULL" << std::endl;
	  return dest;
	  
	}
	
  };


}


#endif
