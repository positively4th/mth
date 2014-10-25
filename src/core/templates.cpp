/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#include <tfunction0.h>
#include <tfunctions.h>
#include <tadd.h>

#include <linkedptrlist.cpp>
template class P4th::LinkedPtrList<P4th::tAdd<float> *>;
template class P4th::LinkedPtrList<P4th::tAdd<double> *>;
#include <linkedptrlistpost.cpp>
template class P4th::LinkedPtrListPost<P4th::tAdd<float> *>;
template class P4th::LinkedPtrListPost<P4th::tAdd<double> *>;


#include <tpointerstack.cpp>
template class P4th::tPointerStack<P4th::tFunctions<float> >;
template class P4th::tPointerStack<P4th::tFunctions<double> >;
template class P4th::tPointerStack<P4th::tFunction0<float> >;
template class P4th::tPointerStack<P4th::tFunction0<double> >;
