/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#pragma once

#include <misctypes.h>
#include <coretypes.h>
#include <tnmmatrix.h>

#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif

#include <sstream>
#include <string>
#include <map>

namespace P4th
{

  template<class TYPE> 
    class tNSolver;

  namespace NSolver {
  
  template<class TYPE> 
    class tStepper 
    {
    protected:
      MTH_MISC_TYPES(TYPE);
      MTH_CORE_TYPES(TYPE);
    public:
      virtual void Step( const tNSolver<TYPE> *aSolver , _m &lastX , _m &nextX ) 
      { lastX.tnmContainer<TYPE>::Cpy( nextX ); }
      virtual void Open( const tNSolver<TYPE> *aSolver ) 
      {};
      virtual void Close( const tNSolver<TYPE> *aSolver ) 
      {};
    };
  
  }

}

