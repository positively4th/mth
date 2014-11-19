
#include <memory>
#include <twasher.h>
#include <testimator0.h>

#include <tptroption.cpp>
template class P4th::tPtrOption<shared_ptr<P4th::Fit::tWasher<float> >, P4th::Fit::tWasher<float> >;
template class P4th::tPtrOption<shared_ptr<P4th::Fit::tWasher<double> >, P4th::Fit::tWasher<double> >;

template class P4th::tPtrOption<shared_ptr<P4th::Fit::tEstimator0<float> >, P4th::Fit::tEstimator0<float> >;
template class P4th::tPtrOption<shared_ptr<P4th::Fit::tEstimator0<double> >, P4th::Fit::tEstimator0<double> >;
