/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#ifndef _P4THPOINTERSTACK_H_
#define _P4THPOINTERSTACK_H_

#include <vector>

namespace P4th {
  
  using namespace std;

  template<class TYPE> 
    class tPointerStack 
    {
    protected:
      vector<TYPE *> funcs; /// Pointers
    public:
      tPointerStack() {};
      tPointerStack( TYPE *f ) { Push( f ); };
      virtual ~tPointerStack();
      
      /// Add (and return) the pointer f to the stack.
      TYPE *operator+=( TYPE *f )
	{ return Push( f ); }
      /// Add (and return) the pointer f to the stack.
      TYPE *Push( TYPE *f );
      
      /// Peeks at the pointer located at level i from the top
      TYPE *Last( unsigned int i = 0 ) const;
      
      /// Removes the pointer located at level i from the top
      TYPE *Pop( unsigned int i = 0 );
    };

}
  
#endif
