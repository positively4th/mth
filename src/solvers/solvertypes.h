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
  /* Types */								\
  typedef ::P4th::tNSolver<TYPE> _nsolver;				\
  typedef ::P4th::NSolver::tStepper<TYPE> _stepper;			\
  typedef ::P4th::NSolver::tIntervalStepper<TYPE> _intervalStepper;	\
  /* Smart Pointers */							\
  typedef ::shared_ptr<_nsolver> $_nsolver;				\
  typedef ::unique_ptr<_nsolver> $$_nsolver;				\
  typedef ::shared_ptr<_stepper> $_stepper;				\
  typedef ::shared_ptr<_intervalStepper>  $intervalStepper;		\
  /* Options */								\
  typedef ::P4th::tPtrOption<$_stepper, _stepper> $_stepperOpt;		\
  typedef ::P4th::tPtrOption<$_nsolver, _nsolver> $_nsolverOpt;		\
  /* End MTH_SOLVER_TYPES */				



