#include "tstat2.h"
#include <debug.h>
#include <string>

#include "tmathtools.h"


namespace P4th
{

  template<class TYPE>
  tStat2<TYPE>::tStat2( int dim ) : 
    sumX( dim , 1 , 0.0 ) , minX( dim , 1 , 0.0 ) , maxX( dim , 1 , 0.0 ) , sumXX( dim , dim , 0.0 )  
    
  {
    N = 0.0;
  }

  template<class TYPE>
  void tStat2<TYPE>::Reset( int dim)
  {
    if ( dim < 0 )
      dim = sumX.GetRows();
    
    sumX.Resize( dim , 1 );
    sumX.tnmContainer<TYPE>::Cpy( 0.0 );
    sumXX.Resize( dim , dim );
    sumXX.tnmContainer<TYPE>::Cpy( 0.0 );

    minX.Resize( dim , 1 );
    maxX.Resize( dim , 1 );

    N = 0;
  }

  template<class TYPE>
  void tStat2<TYPE>::AddObservation( const tnmmatrix<TYPE> &X , const TYPE &weight )
  {
    /*
    _DB( std::cout << "DIM = " << Dim() << std::endl; );
    _DB( tMathTools<TYPE>::PrintColumnVector( std::cout , X , 10 , labels ) ); 
    */
#ifdef _SAFE
    if ( X.GetRows() != Dim() )
      _DBTHROW( "void tStat2<TYPE>::AddObservation( const tnmmatrix<TYPE> &X , const TYPE &weight ) : X.GetRows() != Dim()" );
#endif

    tnmmatrix<TYPE> tmp( X );
    tnmmatrix<TYPE> tmpT( 1 , Dim() );
    tnmmatrix<TYPE> tmptmp( Dim() , Dim() );

    tmp.Trp( &tmpT );
    tmptmp.Mul( &tmp , &tmpT );

    tmp.Mul( weight );
    tmptmp.Mul( weight * weight );
    
    sumX.Add( &tmp );
    sumXX.Add( &tmptmp );

    if ( N <= 0.0 )
      {
	minX.tnmContainer<TYPE>::Cpy( X );
	maxX.tnmContainer<TYPE>::Cpy( X );
      } else {
      X.MinMax( minX , maxX );
    }
    N += weight;
  }


  template<class TYPE>
  tStat2<TYPE>& tStat2<TYPE>::operator=( const tStat2 &source )
  {
    N = source.N;
    sumX.SafeCopy( source.sumX );
    minX.SafeCopy( source.minX );
    maxX.SafeCopy( source.maxX );
    sumXX.SafeCopy( source.sumXX );
    return *this;
  }

  template<class TYPE>
  tnmmatrix<TYPE> tStat2<TYPE>::Sum() const
  {
    return sumX;
  }

  template<class TYPE>
  tnmmatrix<TYPE> tStat2<TYPE>::Mean() const
  {
    if ( N < 1 )
      _DBTHROW( "tnmmatrix<TYPE> tStat2<TYPE>::Mean() const : N < 1" );
    tnmmatrix<TYPE> res( sumX );
    res.Mul( 1.0 / N );
    return res;
  }

  template<class TYPE>
  tnmmatrix<TYPE> tStat2<TYPE>::Variance() const
  {
      if ( N < 1 )
      _DBTHROW( "tnmmatrix<TYPE> tStat2<TYPE>::Variance() const : N < 2" );

    tnmmatrix<TYPE> res( sumXX );
    res.Mul( 1.0 / ( N - 0 ) );
    
    tnmmatrix<TYPE> mean( Mean() );
    tnmmatrix<TYPE> meanT( 1 , Dim() );
    mean.Trp( &meanT );

    tnmmatrix<TYPE> meanmeanT( Dim() , Dim() );
    meanmeanT.Mul( &mean , &meanT ); 
    
    res.Sub( meanmeanT );
    return res;
    
  }

  template<class TYPE>
  tnmmatrix<TYPE> tStat2<TYPE>::Correlation() const
  {
    
    tnmmatrix<TYPE> var( Variance() );
    tnmmatrix<TYPE> cor( Dim() , Dim() );
    TYPE var2;

    for ( int i = 1 ; i <= Dim() ; i++ )
      for ( int j = 1 ; j <= i ; j++ )
	{
	  var2 = var(i,i) * var(j,j);
	  if ( var2 > 0 )
	    cor( i , j ) = var(i,j) / sqrt( var2 ); 
	  else
	    cor( i , j ) = tMathTools<TYPE>::NaN();

	  if ( i != j )
	    cor( j , i ) = cor( i , j ); 
	}
    return cor;
    
  }

  template<class TYPE>
  tnmmatrix<TYPE> tStat2<TYPE>::VariationCoefficient() const
  {

    tnmmatrix<TYPE> cof( Dim() , 1 );
    tnmmatrix<TYPE> var( Variance() );
    tnmmatrix<TYPE> mean( Mean() );

    for ( int i = 1 ; i <= Dim() ; i++ )
      {
	assert( mean(i,1) != 0 );
	cof(i,1) = sqrt( var(i,i) / ( mean(i,1) * mean(i,1) ) );
      }
    return cof;
    
  }

  template<class TYPE>
  std::ostream &tStat2<TYPE>::PrintSummary( std::ostream &dest , int width , const string &lm ) const
  {
    
    dest << lm << "Mean:" << std::endl;
    if ( N > 0 )
      {
	tnmmatrix<TYPE> moment1( Dim() , 3 );
	tnmmatrix<TYPE>::Set( moment1 , this->Min() , 1 , 1 );
	tnmmatrix<TYPE>::Set( moment1 , this->Mean() , 1 , 2 );
	tnmmatrix<TYPE>::Set( moment1 , this->Max() , 1 , 3 );
	tMathTools<TYPE>::PrintMatrix( dest , moment1 , width , labels , "Min,Mean,Max"  , lm );
      } else 
      dest << "Too Few Observations!" << std::endl;
    
    dest << lm << "Varicance:" << std::endl;
    if ( N > 1 )
      {
	tnmmatrix<TYPE> var( this->Variance() );
	tMathTools<TYPE>::PrintMatrix( dest , var , width , labels , labels , lm );
      } else
      dest << "Too Few Observations!" << std::endl;
    
    dest << lm << "Correlation:" << std::endl;
    if ( N > 1 )
      {
	tnmmatrix<TYPE> corr( this->Correlation() );
	tMathTools<TYPE>::PrintMatrix( dest , corr , width , labels , labels , lm );
      } else
      dest << "Too Few Observations!" << std::endl;
    
    return dest;
  }



}

template class P4th::tStat2<double>;
template class P4th::tStat2<float>;

