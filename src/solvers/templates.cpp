/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#include <memory>
#include <tnsolver.h>
#include <tnmmatrix.h>

#include <toption.cpp>
template class P4th::tOption<P4th::tNSolver<double>::_stepper>;
template class P4th::tOption<P4th::tNSolver<float>::_stepper>;
template class P4th::tOption<P4th::tnmmatrix<double> >;
template class P4th::tOption<P4th::tnmmatrix<float> >;
