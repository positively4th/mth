/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#ifndef _P4TH_TBINARY_H_
#define _P4TH_TBINARY_H_

#include "tderivatives0.h"
#include "tfunctions.h"

//class istream;
//class ostream;


namespace P4th
{

  template<class TYPE> 
    class tBinary : public tDerivatives0<TYPE>
  {
  public:
    virtual int Nodes() const
    { return 1 + this->right->Nodes() + left->Nodes(); }
  protected:
    tFunction0<TYPE> *left;
    tFunction0<TYPE> *right;
  public:
    tBinary( tFunction0<TYPE> *l , tFunction0<TYPE> *r  ); 
    tBinary( const tBinary &source ) : 
      tDerivatives0<TYPE>( source ) 
      { left = source.left->Clone(); right = source.right->Clone(); }
      
      virtual ~tBinary();
      
      
      virtual bool DimOk( int d ) const
      { if ( left->DimOk( d ) && right->DimOk( d ) ) return true; else return false; }
      
      
      virtual std::ostream &Dump( std::ostream &dest , string lm = "" ) const 
      {
	dest << lm << __FILE__ << ")" << (void *)this << ")" << std::endl; 
	lm += "\t";
	tFunction0<TYPE>::Dump( dest , lm ); 
	left->Dump( dest , lm + "\t" ); 
	dest << lm << "<Binary Operator>" << std::endl;
	right->Dump( dest , lm + "\t" ); 
	return dest;
      }
      
      friend class tFunction0<TYPE>;
  };
  
  


}


#endif
