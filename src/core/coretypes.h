#pragma once

#include <string>
#include <vector>

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

  template <class TYPE>
    class tOption;
 
  template <class TYPE>
    class tPointerStack;

 
}

typedef std::vector<std::string> _strVec;
typedef _strVec::const_iterator cStrVecee;
typedef _strVec::iterator _strVecee;


#define MTH_CORE_TYPES(TYPE)			    \
  /* MTH_CORE_TYPES(TYPE): BEGIN */		    \
  /* Basic Templated Types */			    \
  typedef P4th::tFunction0<TYPE> _f;		    \
  typedef P4th::tFunctions<TYPE> _fs;		    \
  typedef P4th::tfMatrix0<TYPE> _fm0;		    \
  typedef P4th::tfMatrix<TYPE> _fm;		    \
  typedef P4th::tnmmatrix<TYPE> _m;		    \
  typedef P4th::tAdd<TYPE> _add;		    \
  typedef P4th::tMul<TYPE> _mul;		    \
  typedef P4th::tSelector<TYPE> _arg;		    \
  typedef P4th::tParameter<TYPE> _par;		    \
  typedef P4th::tConstant<TYPE> _const;		    \
  typedef P4th::tConstant<TYPE> _c;		    \
  /* Shared ptr types */			    \
  typedef shared_ptr<_f> $_f;			    \
  typedef shared_ptr<_fs> $_fs;			    \
  typedef shared_ptr<_m> $_m;			    \
  typedef shared_ptr<_fm> $fm;			    \
  typedef shared_ptr<_fm0> $fm0;		    \
  typedef shared_ptr<_add> $_add;		    \
  typedef shared_ptr<_mul> $_mul;		    \
  typedef shared_ptr<_arg> $_arg;		    \
  typedef shared_ptr<_par> $_par;		    \
  typedef shared_ptr<_const> $_const;		    \
  typedef shared_ptr<_c> $_c;		    \
  /* Unique ptr types */			    \
  typedef unique_ptr<_f> $$_f;			    \
  typedef unique_ptr<_fs> $$_fs;		    \
  typedef unique_ptr<_m> $$_m;			    \
  typedef unique_ptr<_fm> $$fm;			    \
  typedef unique_ptr<_fm0> $$fm0;		    \
  typedef unique_ptr<_add> $$_add;		    \
  typedef unique_ptr<_mul> $$_mul;		    \
  typedef unique_ptr<_arg> $$_arg;		    \
  typedef unique_ptr<_par> $$_par;		    \
  typedef unique_ptr<_const> $$_const;		    \
  typedef unique_ptr<_c> $$_c;			    \
  /* Misc */					    \
  typedef P4th::tOption<_m> _matrixOpt;		    \
  typedef P4th::tPointerStack<_f> _fStack;	    \
  /* MTH_CORE_TYPES(TYPE): EMD */
