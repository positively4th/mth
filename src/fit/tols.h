#ifndef _P4TH_TOLS_H_
#define _P4TH_TOLS_H_

#include <misctypes.h>
#include <coretypes.h>
#include <tfit.h>
#include <twasher.h>

//class istream;
//class ::std::ostream;


namespace P4th
{

  template<class TYPE> 
    class tOLS : public tFit<TYPE> 
  {
    MTH_MISC_TYPES(TYPE)
    MTH_CORE_TYPES(TYPE)
  public:
    typedef tWasher<TYPE> _washer;
    typedef tOption<shared_ptr<_washer> > _washerOpt;

    TYPE cond;
    void ResetOptions();
  protected:
  public:
    tOLS( int _M , int _K );
    
    virtual void AddObservation( const tnmmatrix<TYPE> &Y , const tnmmatrix<TYPE> &X );
    virtual $_m EstimateB();
    virtual $_fs CreatePredictors(); 
    virtual tnmmatrix<TYPE> &yhat( tnmmatrix<TYPE> &res , const  tnmmatrix<TYPE> &xrow , const  tnmmatrix<TYPE> &B );
    
    
  };


}


#endif
