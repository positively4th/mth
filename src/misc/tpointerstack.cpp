/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#include "tpointerstack.h"

#include <stddef.h>
#include "debug.h"

namespace P4th {

  using namespace std;

  template<class TYPE> 
  tPointerStack<TYPE>::~tPointerStack() 
  {
    for (  int i = 0 ; i < funcs.size() ; i++ )
      if ( funcs[i] != NULL )
	delete funcs[i];
  }
      

  template<class TYPE> 
  TYPE *tPointerStack<TYPE>::Push( TYPE *f )
  {
    funcs.push_back( f );
    return f;
  }
      
    
  template<class TYPE> 
  TYPE *tPointerStack<TYPE>::Last( unsigned int i ) const
  {

    if ( this->funcs.size() - i <= 0 )
      _DBTHROW( "To few pointers on stack." );
    return this->funcs[this->funcs.size() - 1 - i]; 
  }
  

  template<class TYPE> 
  TYPE *tPointerStack<TYPE>::Pop( unsigned int i )
  {
    if ( this->funcs.size() - i <= 0 )
      _DBTHROW( "To few pointers on stack." );
    TYPE *res =  funcs[funcs.size() - 1 - i];
    funcs.erase( funcs.end() - 1 - i );
    return res; 
  }

}
    
