#include <memory>
#include <twasher0.h>
#include <twasher.h>
#include <testimator0.h>
#include <tols.h>

#include <tptroption.cpp>
template class ::P4th::tPtrOption<::std::shared_ptr<::P4th::Fit::tWasher0<float> >, ::P4th::Fit::tWasher0<float> >;
template class ::P4th::tPtrOption<::std::shared_ptr<::P4th::Fit::tWasher0<double> >, ::P4th::Fit::tWasher0<double> >;

template class ::P4th::tPtrOption<::std::shared_ptr<::P4th::Fit::tWasher<float> >, ::P4th::Fit::tWasher<float> >;
template class ::P4th::tPtrOption<::std::shared_ptr<::P4th::Fit::tWasher<double> >, ::P4th::Fit::tWasher<double> >;

template class ::P4th::tPtrOption<::std::shared_ptr<::P4th::Fit::tEstimator0<float> >, ::P4th::Fit::tEstimator0<float> >;
template class P4th::tPtrOption<::std::shared_ptr<::P4th::Fit::tEstimator0<double> >, ::P4th::Fit::tEstimator0<double> >;

template class ::P4th::tPtrOption<::std::shared_ptr<::P4th::Fit::tOLS<float> >, ::P4th::Fit::tOLS<float> >;
template class ::P4th::tPtrOption<::std::shared_ptr<::P4th::Fit::tOLS<double> >, ::P4th::Fit::tOLS<double> >;
