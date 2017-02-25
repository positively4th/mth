#pragma once

#include <string>
#include <vector>
#include <memory>

namespace P4th {
  
  template<class TYPE>
    class tFunction0;				
  
  template<class TYPE>
    class tFunctions;				
  
  template<class TYPE>
    class tnmmatrix;				
  
  template<class TYPE>
    class tfMatrix0;				
  
  template<class TYPE>
    class tfMatrix;				
  
  template<class TYPE>
    class tNsolver;				
  
  template<class TYPE>
    class tAdd;				
  
  template<class TYPE>
    class tMul;				
  
  template<class TYPE>
    class tSelector;				
  
  template<class TYPE>
    class tConstant;				
  
  template<class TYPE>
    class tParameter;				
  
  template<class TYPE>
    class tParameters;				
  
  template <class TYPE>
    class tOption;
 
  template <class TYPE>
    class tPointerStack;
  
  
}

typedef std::vector<std::string> _strVec;
typedef _strVec::const_iterator _cstrVecee;
typedef _strVec::iterator _strVecee;

#define MTH_CORE_TYPES(TYPE)			    \
  /* MTH_CORE_TYPES(TYPE): BEGIN */		    \
  /* Basic Templated Types */			    \
  typedef ::P4th::tFunction0<TYPE> _f;		    \
  typedef ::P4th::tFunctions<TYPE> _fs;		    \
  typedef ::P4th::tfMatrix0<TYPE> _fm0;		    \
  typedef ::P4th::tfMatrix<TYPE> _fm;		    \
  typedef ::P4th::tnmmatrix<TYPE> _m;		    \
  typedef ::P4th::tAdd<TYPE> _add;		    \
  typedef ::P4th::tMul<TYPE> _mul;		    \
  typedef ::P4th::tSelector<TYPE> _arg;		    \
  typedef ::P4th::tParameter<TYPE> _par;	    \
  typedef ::P4th::tParameters<TYPE> _pars;	    \
  typedef ::P4th::tConstant<TYPE> _const;	    \
  typedef ::P4th::tConstant<TYPE> _c;		    \
  /* Shared ptr types */			    \
  typedef ::std::shared_ptr<_f> $_f;			    \
  typedef ::std::shared_ptr<_fs> $_fs;			    \
  typedef ::std::shared_ptr<_m> $_m;			    \
  typedef ::std::shared_ptr<_fm> $fm;			    \
  typedef ::std::shared_ptr<_fm0> $fm0;			    \
  typedef ::std::shared_ptr<_add> $_add;		    \
  typedef ::std::shared_ptr<_mul> $_mul;		    \
  typedef ::std::shared_ptr<_arg> $_arg;		    \
  typedef ::std::shared_ptr<_par> $_par;		    \
  typedef ::std::shared_ptr<_pars> $_pars;		    \
  typedef ::std::shared_ptr<_const> $_const;		    \
  typedef ::std::shared_ptr<_c> $_c;			    \
  /* Unique ptr types */			    \
  typedef ::std::unique_ptr<_f> $$_f;		    \
  typedef ::std::unique_ptr<_fs> $$_fs;			    \
  typedef ::std::unique_ptr<_m> $$_m;			    \
  typedef ::std::unique_ptr<_fm> $$fm;			    \
  typedef ::std::unique_ptr<_fm0> $$fm0;		    \
  typedef ::std::unique_ptr<_add> $$_add;		    \
  typedef ::std::unique_ptr<_mul> $$_mul;		    \
  typedef ::std::unique_ptr<_arg> $$_arg;		    \
  typedef ::std::unique_ptr<_par> $$_par;		    \
  typedef ::std::unique_ptr<_pars> $$_pars;		    \
  typedef ::std::unique_ptr<_const> $$_const;		    \
  typedef ::std::unique_ptr<_c> $$_c;			    \
  /* Vectors */						    \
  typedef ::std::vector<_m> _mVec;			    \
  typedef typename _mVec::const_iterator _cmVecee;		    \
  typedef typename _mVec::iterator _mVecee;			    \
  typedef std::shared_ptr<_mVec> $_mVec;			    \
  typedef std::unique_ptr<_mVec> $$_mVec;			    \
  /* Misc */						    \
  typedef ::P4th::tOption<_m> _matrixOpt;		    \
  typedef ::P4th::tPointerStack<_f> _fStack;		    \
  /* MTH_CORE_TYPES(TYPE): EMD */
