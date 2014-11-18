/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#ifndef _P4TH_TSQUARE_H_
#define _P4TH_TSQUARE_H_

#include <tatom.h>
#include <tmul.h>
#include <tconstant.h>
#include <math.h>

//class istream;
//class ::std::ostream;


namespace P4th
{

  template<class TYPE> 
    class tSquare : public tAtom<TYPE> 
  {
  protected:
    tFunction0<TYPE> *Reduce( int argnr , tFunction0<TYPE> *s  ) const
      {  return new tSquare<TYPE>( this->f->Reduce( argnr , s ) ); }
  public:
    tSquare( tFunction0<TYPE> *f  ) : tAtom<TYPE>( f ) 
    {}
    tSquare( const tSquare<TYPE> &source  ) : tAtom<TYPE>( source ) 
    {}
    virtual ~tSquare() 
    {}
    
    virtual tFunction0<TYPE> *Clone() const
    { return new tSquare<TYPE>( *this ); }
    
    virtual tFunction0<TYPE> *AnalyticalDerivative( int argnr ) const;
    virtual TYPE y( const tnmmatrix<TYPE> &x ) const
    {
      TYPE res = this->f->y( x );
      res *= res;
      return res; 
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
