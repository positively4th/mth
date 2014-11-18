/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#ifndef _P4TH_TSLATECSTACK_H_
#define _P4TH_TSLATECSTACK_H_

#include <debug.h>
#include <time.h>
#include <linkedptrlist.h>


//class istream;
//class ::std::ostream;


namespace P4th
{

  template<class TYPE> 
    class tSlatecStack
    {
    protected:
      tSlatecStack *next;
      TYPE data;
    public:
      tSlatecStack( TYPE d , tSlatecStack *n = 0L ) { data = d; next = n;}
      
      static TYPE Peek( tSlatecStack *s , TYPE res = NULL )
	{
	  if ( s == NULL )
	    return res;

	  res = s->data;
	  return res;
	}

      static void Push( tSlatecStack **s , TYPE d )
	{
	  //	  _DB( std::cout << "Pushing " << d << " on stack " << (void *)(*s) << std::endl << std::flush; );
	  *s = new tSlatecStack<TYPE>( d , (*s) );
	}
	
      static TYPE Pop( tSlatecStack **s  )
	{
	  //	  _DB( std::cout << "Popping from stack " << (void *)(*s) << std::endl << std::flush; );
	  if ( (*s) == NULL )
	    return NULL;

	  TYPE res = (*s)->data;
	  
	  tSlatecStack *tmp;
	  tmp = (*s)->next;
	  delete (*s);
	  (*s) = tmp;
	  return res;
	}
	

    };



}



#endif
