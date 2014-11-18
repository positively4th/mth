/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#include "tatom.h"

#include <debug.h>
#include <math.h>
#include <tconstant.h>



namespace P4th
{

  template<class TYPE> 
  tFunction0<TYPE> *tAtom<TYPE>::Simplify()
  {
    
    f = f->Simplify();
    
    tFunction0<TYPE> *c = dynamic_cast<tConstant<TYPE> *>( f );
    if ( c )  {
      std::cerr << "tFunction0<TYPE> *tAtom<TYPE>::Simplify(): f is constant ... improvement possible!" << std::endl;
      //      c = new tConstant<TYPE>( this->y->
    }
    return this;
  }

}

template class P4th::tAtom<float>;
template class P4th::tAtom<double>;



