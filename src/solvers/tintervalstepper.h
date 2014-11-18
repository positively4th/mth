/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#pragma once

#include <tnmmatrix.h>
#include <tstepper.h>

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
      class tIntervalStepper : public tStepper<TYPE> 
    {
      MTH_CORE_TYPES(TYPE);
    public:
      bool printFraction;
    protected:
      
      map<int,TYPE, std::less<int> > intervals;
      map<int,TYPE, std::less<int> > points;
    public:
      tIntervalStepper(TYPE firstFraction);
      
      void AddIterFractionPair( int i , TYPE aFraction ) 
      { intervals[i] = aFraction; }
      virtual void Step( const tNSolver<TYPE> *aSolver , _m &lastX , _m &nextX );
      virtual void Open( const tNSolver<TYPE> *aSolver ); 
      virtual void Close( const tNSolver<TYPE> *aSolver ); 
    };
  
  }

}

