#pragma once

namespace P4th {

  template<class SPTR, class T>
    class tPtrOption;   

  template<class TYPE>
    class tNSolver;
  
  namespace NSolver {
    
    template<class TYPE>
      class tStepper;
    
    template<class TYPE>
      class tIntervalStepper;
    
  }
}

#define MTH_SOLVER_TYPES(TYPE)						\
  /* Begin MTH_SOLVER_TYPES */						\
  typedef P4th::tNSolver<TYPE> _nsolver;				\
  typedef P4th::NSolver::tStepper<TYPE> _stepper;			\
  typedef P4th::NSolver::tIntervalStepper<TYPE> _intervalStepper;	\
  typedef shared_ptr<_stepper>  $stepper;				\
  typedef shared_ptr<_intervalStepper>  $intervalStepper;		\
  typedef P4th::tPtrOption<$stepper, _stepper> _stepperOpt;		\
  /* End MTH_SOLVER_TYPES */				



