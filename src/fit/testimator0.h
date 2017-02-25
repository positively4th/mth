#pragma once

#include <misctypes.h>
#include <coretypes.h>
#include <fittypes.h>

#include <memory>
#include <map>
#include <string>
//class istream;
//class ::std::ostream;

using namespace std;

namespace P4th {
  
  template<class TYPE>
    class tFit;
  
  namespace Fit {
    
    template<class TYPE> 
      class tEstimator0 
      {
      public:
	MTH_MISC_TYPES(TYPE);
	MTH_CORE_TYPES(TYPE);
	MTH_FIT_TYPES(TYPE);
	typedef std::map<std::string,_mVec> _miscMap;
	typedef typename _miscMap::iterator _miscMapee;
      protected:
	tFit<TYPE> *owner;
	_miscMap misc;
      public:
	tEstimator0(tFit<TYPE> *owner);

	//tEstimator must define these....
	virtual $_m GetB() = 0;
	virtual $_fs GetPredictors() = 0; 
	//Other valuable output can be read from the misc map
	virtual const _miscMap &GetMisc()
	{ return misc; }
	virtual const _mVec &GetMiscItem(const string aKey);

	/* tFit API */
	//tFit parameters
	int GetP() const;
	int GetK() const;
	int GetN() const;
	//Washed data
	$_m GetX();
	$_m GetY();
      };

  }

}

