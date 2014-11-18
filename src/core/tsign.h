/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#ifndef _P4TH_TSIGN_H_
#define _P4TH_TSIGN_H_

#include <tatom.h>
#include <math.h>
#include <algorithm>

//class istream;
//class ::std::ostream;


namespace P4th
{

  template<class TYPE> 
    class tSign : public tAtom<TYPE> 
  {
  protected:
  public:
    tSign( tFunction0<TYPE> *_f ) : tAtom<TYPE>( _f  ) 
    {} 
    tSign( const tSign<TYPE> &source  ) : tAtom<TYPE>( source ) 
    {}
    virtual ~tSign() 
    {} 
    
    virtual tSign *Clone() const
    { return new tSign( *this  ); }
    virtual tFunction0<TYPE> *Reduce( int argnr , tFunction0<TYPE> *s  ) const
      {  return new tSign<TYPE>( this->f->Reduce( argnr , s )  ); }
    
    virtual tFunction0<TYPE> *AnalyticalDerivative( int argnr ) const;

    virtual TYPE y( const tnmmatrix<TYPE> &x ) const
    {
      TYPE res;
      res = this->f->y( x );
      if ( res >= 0 )
	return res = 1;
      return res = -1;
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
