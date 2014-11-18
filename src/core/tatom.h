/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#ifndef _P4TH_TATOM_H_
#define _P4TH_TATOM_H_

#include <tderivatives0.h>
#include <tselector.h>
#include <math.h>
#include <debug.h>

//class istream;
//class ::std::ostream;


namespace P4th
{

  template<class TYPE> 
    class tAtom : public tDerivatives0<TYPE>
  {
  public:
    virtual int Nodes() const
    { return 1 + f->Nodes(); }
  protected:
    tFunction0<TYPE> *f;
  public:
    tAtom( tFunction0<TYPE> *_f = NULL) : tDerivatives0<TYPE>()
      { 
	if (_f == NULL) {
	  f = new tSelector<TYPE>(1);
	} else {
	  f = _f; 
	}
    }

   tAtom( const tAtom<TYPE> &source ) : tDerivatives0<TYPE>( source )
    { 
      f = source.f->Clone(); 
    }
    virtual ~tAtom()  
    { 
      delete f;
    }

    virtual tFunction0<TYPE> *Simplify();

    
    virtual bool DimOk( int d ) const
    { return f->DimOk( d ); }

    
    virtual std::ostream &Dump( std::ostream &dest , string lm = "" ) const
    {
	  dest << __FILE__ << "(" << (void *)this << ")" << std::endl;
	  lm += "\t";
	  f != NULL ? f->Dump( dest , lm ) : dest << lm << "NULL" << std::endl;
	  return dest;
    }
    
  };
  
}


#endif
