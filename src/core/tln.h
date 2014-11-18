/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#ifndef _P4TH_TLN_H_
#define _P4TH_TLN_H_

#include <tatom.h>
#include <tmathtools.h>
#include <treciprocal.h>
#include <math.h>

//class istream;
//class ::std::ostream;


namespace P4th
{

  template<class TYPE> 
    class tLn : public tAtom<TYPE> 
    {
    protected:
  public:
    tLn( tFunction0<TYPE> *_f = NULL) : tAtom<TYPE>( _f ) 
    {} 
    tLn( const tLn<TYPE> &source  ) : tAtom<TYPE>( source ) 
    {}
    virtual ~tLn() 
    {} 
    
    virtual tLn *Clone() const
    { return new tLn( *this  ); }
    virtual tFunction0<TYPE> *Reduce( int argnr , tFunction0<TYPE> *s  ) const
      {  return new tLn<TYPE>( this->f->Reduce( argnr , s )  ); }
    
    virtual tFunction0<TYPE> *AnalyticalDerivative( int argnr ) const;
    
    virtual TYPE y( const tnmmatrix<TYPE> &x ) const
    {
      TYPE tmp = this->f->y( x );
      if ( tmp <= 0 )
	return tMathTools<TYPE>::NaN(); 
      return log( tmp );
    }

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
