/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#include <solvertypes.h>

#include <memory>
#include <tnsolver.h>
#include <tnmmatrix.h>
#include <tptroption.h>


#include <tptroption.cpp>
#include <toption.cpp>
namespace P4th {
  
  template class tPtrOption<::std::shared_ptr<tNSolver<float> >, tNSolver<float> >;
  template class tPtrOption<::std::shared_ptr<tNSolver<double> >, tNSolver<double> >;
  
  template class tPtrOption<shared_ptr<NSolver::tStepper<double> >, NSolver::tStepper<double>>; 
  template class tPtrOption<shared_ptr<NSolver::tStepper<float> >, NSolver::tStepper<float>>; 

  template class tOption<tnmmatrix<double> >;
  template class tOption<tnmmatrix<float> >;

 }
