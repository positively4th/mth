#ifndef _TSTAT2_H_
#define _TSTAT2_H_

#include <math.h>
#include <ostream>
#include <string>
#include "tnmmatrix.h"

// Ugly fix, gcc3.4 preprocsessor replaces max in std::max
#ifdef max
#undef max
#endif
#ifdef min
#undef min
#endif


using namespace std;

namespace P4th
{

template <class TYPE>
class tStat2  
{
 protected:
  TYPE N;
  tnmmatrix<TYPE> sumX;
  tnmmatrix<TYPE> minX;
  tnmmatrix<TYPE> maxX;
  tnmmatrix<TYPE> sumXX;
  int Dim() const
  { return sumX.GetRows(); }
  string labels; // Comma separated list of labels
 public:
  tStat2( int dim = 0 );
  tStat2( const tStat2 &source )
    { *this = source; }

  void Reset( int dim = -1 );
  void SetLabels( const string &labs )
  { labels = labs; }
  string GetLabels() const
  { return labels; }

  void AddObservation( const tnmmatrix<TYPE> &X , const TYPE &weight = 1 );

  TYPE GetN() const
  { return N;}

  tStat2 &operator=( const tStat2 &source );
  tnmmatrix<TYPE> Sum() const;
  tnmmatrix<TYPE> Mean() const;
  tnmmatrix<TYPE> Min() const
  { return minX; }
  tnmmatrix<TYPE> Max() const
  { return maxX; }
  tnmmatrix<TYPE> Variance() const;
  tnmmatrix<TYPE> Correlation() const;
  tnmmatrix<TYPE> VariationCoefficient() const;

  std::ostream &PrintSummary( std::ostream &dest , int width , const string &lm = "" ) const;   
 };
 
}
#endif
