/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#include "tbinary.h"

namespace P4th
{

  template<class TYPE> 
  tBinary<TYPE>::tBinary( tFunction0<TYPE> *l , tFunction0<TYPE> *r  ) : 
    tDerivatives0<TYPE>()
  {
      left = l;
      right = r;
  }  

  template<class TYPE> 
  tBinary<TYPE>::~tBinary() 
  {
    if ( left != 0L )
      delete left;
    if ( right != 0L )
      delete right;
  }

  
  


  
}

template class P4th::tBinary<float>;
template class P4th::tBinary<double>;


