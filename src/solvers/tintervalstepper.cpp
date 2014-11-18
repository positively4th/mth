/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/

#include <tintervalstepper.h>
#include <tnsolver.h>


namespace P4th {

  namespace NSolver {
    
    template<class TYPE>
    tIntervalStepper<TYPE>::tIntervalStepper(TYPE firstFraction)
    {
      this->AddIterFractionPair(0, firstFraction);
      printFraction = false;
    }
    
    template<class TYPE>
    void tIntervalStepper<TYPE>::Step( const tNSolver<TYPE> *aSolver , _m &lastX , _m &nextX )
    {
      TYPE fraction;
      int I = aSolver->GetCurrentIteration();
      if ( I <= (points.begin())->first )
	fraction = (points.begin())->second;
      else if ( I >= (--points.end())->first )
	fraction = (--points.end())->second;
      else
	fraction = points[I];
      
      if ( printFraction )
	std::cout << I << "->" << fraction << std::endl;
      
      /*
      lastX.Mul( fraction );
      nextX.Mul( 1.0 - fraction );
      lastX.Add( nextX ); 
      */

      lastX.Mul( 1.0 - fraction );
      nextX.Mul( fraction );
      lastX.Add( nextX ); 
    }
    
    template<class TYPE>
    void tIntervalStepper<TYPE>::Open( const tNSolver<TYPE> *aSolver )
    {
      points.clear();
      
      typename map<int,TYPE>::iterator it0 = intervals.begin();
      typename map<int,TYPE>::iterator it1 = it0;
      it1++;
      if (it1 == intervals.end()) {
	points[it0->first] = it0->second;
	return;
      }
      for ( ; it1 != intervals.end() ; it1++ , it0++ ) {
	for ( int i = it0->first ; i <= it1->first ; i++ ) {
	  points[i] = it0->second + (it1->second - it0->second) * (TYPE)(i - it0->first) / (TYPE)(it1->first - it0->first);
	  //	  std::cout << i << "<->" << points[i] << std::endl;
	}
      }

    }
    
    template<class TYPE>
    void tIntervalStepper<TYPE>::Close( const tNSolver<TYPE> *aSolver )
    {
      points.clear();
    }
    
  }
  
}

template class P4th::NSolver::tIntervalStepper<float>;
template class P4th::NSolver::tIntervalStepper<double>;

