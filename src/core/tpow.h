/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#ifndef _P4TH_TPOW_H_
#define _P4TH_TPOW_H_

#include "tbinary.h"
#include "tconstant.h"
#include "tmul.h"
#include "tadd.h"
#include <math.h>
#include <string>

//class istream;
//class ::std::ostream;


namespace P4th
{

  template<class TYPE> 
    class tPow : public tBinary<TYPE> 
  {
  protected:
    tPow( tFunction0<TYPE> *l , tFunction0<TYPE> *r ) :
      tBinary<TYPE>( l , r  )
      {}
  public:
    tPow( const tPow<TYPE> &source ) : tBinary<TYPE>( source )
    {}

    virtual ~tPow()
      {}
        

    virtual tPow<TYPE> *Clone() const
    { return new tPow( *this ); }
    virtual tFunction0<TYPE> *Reduce( int argnr , tFunction0<TYPE> *s  ) const;
    
    virtual tFunction0<TYPE> *AnalyticalDerivative( int argnr ) const;

    virtual TYPE y( const tnmmatrix<TYPE> &x ) const;

    virtual std::ostream &Dump( std::ostream &dest , string lm = "" ) const 
    {
      dest << lm << __FILE__ << "(" << (void *)this << ")" << std::endl;
      lm += "\t";
      dest << lm << "base:" << std::endl;
      this->left != NULL ? this->left->Dump( dest , lm ) : dest << lm << "NULL" << std::endl;
      dest << lm << "exponent:" << std::endl;
      this->right != NULL ? this->right->Dump( dest , lm ) : dest << lm << "NULL" << std::endl;
      return dest;
    }
    
    friend class tFunction0<TYPE>;
    
  };


}


#endif
