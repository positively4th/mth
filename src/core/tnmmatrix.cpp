/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#include "tnmmatrix.h"
#include <iostream>
#include <iomanip>
#include <stdarg.h>
#include "debug.h"
#include <stlstringtools.h>
#include "texception.h"
#include "tmathtools.h"


#ifdef _OCTAVE
#include <octave/config.h>
#include <octave/Matrix.h>
#endif
#ifdef _SLATEC
#include <tslatectools.h>
#endif
#ifdef _GSL
#include <gsl/gsl_math.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_permutation.h>
#include <gsl/gsl_linalg.h>
#endif

namespace P4th
{

  template<class TYPE>
  tnmmatrix<TYPE>::tnmmatrix( int R , int C , TYPE e ) :
    tnmContainer<TYPE>( R , C , e )
  {
#ifdef _GSL
    tMathTools<TYPE>::InitGSLErrorHandler();
#endif

  }

  template<class TYPE>
  tnmContainer<TYPE>::tnmContainer( int R , int C , TYPE **es ) :
    tnmContainer<TYPE>( R , C , es )
  {
#ifdef _GSL
    tMathTools<TYPE>::InitGSLErrorHandler();
#endif
  }

  
  template<class TYPE>
  inline int tnmmatrix<TYPE>::IsNaN() const
  {
    for ( int i = 1 ; i <= this->ROWS ; i++ )
      for ( int j = 1 ; j <= this->COLS ; j++ )
	if ( tMathTools<TYPE>::IsNaN( this->elements[i-1][j-1] ) )
	  return this->ROWS * i + j;
    return 0;
  }

  template<class TYPE>
  inline int tnmmatrix<TYPE>::IsNumber() const
  {
    for ( int i = 1 ; i <= this->ROWS ; i++ )
      for ( int j = 1 ; j <= this->COLS ; j++ )
	if ( ! tMathTools<TYPE>::IsNumber( this->elements[i-1][j-1] ) )
	  return 0;
    return 1;
  }

  template<class TYPE>
  tnmmatrix<TYPE> *tnmmatrix<TYPE>::I()
  {
#ifdef _SAFE
    if ( this->ROWS != this->COLS )
      _DBTHROW( (string)"identity(): error this->ROWS != this->COLS" );
#endif
    for ( int i = 0 ; i < this->ROWS ; i++ )
      for ( int j = 0 ; j < this->COLS ; j++ )
	if ( i == j )
	  this->elements[i][j] = 1;
	else
	  this->elements[i][j] = 0;
    return this;
  }

  template<class TYPE>
  tnmmatrix<TYPE> *tnmmatrix<TYPE>::Abs() 
  {
    for ( int  i = 0 ; i < this->ROWS ; i++ ) 
      for ( int  j = 0 ; j < this->COLS ; j++ )
	if ( this->elements[i][j] < 0 )
	  this->elements[i][j] = - this->elements[i][j];
    return this;
  }


  /*
  template<class TYPE>
  tnmmatrix<TYPE> Trp( tnmmatrix<TYPE> *res , const tnmmatrix<TYPE> *A )
  {
#ifdef _SAFE
    if ( this->COLS != res->ROWS || this->ROWS != res->COLS )
      throw (string)"tnmmatrix<TYPE> *tnmmatrix<TYPE>::Trp( tnmmatrix<TYPE> *res ) const : this->COLS != res->ROWS || this->ROWS != res->COLS";
#endif
    for ( int i = 1 ; i <= A->ROWS ; i++ )
      for ( int j = 1 ; j <= A->COLS ; j++ )
	(*res)(j,i) = A->this->Get(i,j);
    return res;
  }
  */

  template<class TYPE>
  tnmmatrix<TYPE> *tnmmatrix<TYPE>::Add( const tnmmatrix<TYPE> *B )
  {
    for ( int  i = 0 ; i < this->ROWS ; i++ ) 
      for ( int  j = 0 ; j < this->COLS ; j++ )
	this->elements[i][j] += B->elements[i][j];
    return this;
  }

  template<class TYPE>
  tnmmatrix<TYPE> *tnmmatrix<TYPE>::Sub( const tnmmatrix<TYPE> *B )
  {
    for ( int  i = 0 ; i < this->ROWS ; i++ ) 
      for ( int  j = 0 ; j < this->COLS ; j++ )
	this->elements[i][j] -= B->elements[i][j];
    return this;
  }

  template<class TYPE>
  tnmmatrix<TYPE> *tnmmatrix<TYPE>::Add( const TYPE *e )
  {
    for ( int  i = 0 ; i < this->ROWS ; i++ ) 
      for ( int  j = 0 ; j < this->COLS ; j++ )
	this->elements[i][j] += *e;
    return this;
  }

  template<class TYPE>
  tnmmatrix<TYPE> *tnmmatrix<TYPE>::Sub( const TYPE *e )
  {
    for ( int  i = 0 ; i < this->ROWS ; i++ ) 
      for ( int  j = 0 ; j < this->COLS ; j++ )
	this->elements[i][j] -= *e;
    return this;
  }

  template<class TYPE>
  tnmmatrix<TYPE> *tnmmatrix<TYPE>::Mul( const TYPE *e )
  {
    for ( int  i = 0 ; i < this->ROWS ; i++ ) 
      for ( int  j = 0 ; j < this->COLS ; j++ )
	this->elements[i][j] *= *e;
    return this;
  }

  template<class TYPE>
  tnmmatrix<TYPE> *tnmmatrix<TYPE>::Add( const tnmmatrix<TYPE> *A , const tnmmatrix<TYPE> *B )
  {
    for ( int  i = 0 ; i < this->ROWS ; i++ ) 
      for ( int  j = 0 ; j < this->COLS ; j++ )
	this->elements[i][j] = A->elements[i][j] + B->elements[i][j];
    return this;
  }

  template<class TYPE>
  tnmmatrix<TYPE> *tnmmatrix<TYPE>::Sub( const tnmmatrix<TYPE> *A , const tnmmatrix<TYPE> *B )
  {
    for ( int  i = 0 ; i < this->ROWS ; i++ ) 
      for ( int  j = 0 ; j < this->COLS ; j++ )
	this->elements[i][j] = A->elements[i][j] - B->elements[i][j];
    return this;
  }

  template<class TYPE>
  tnmmatrix<TYPE> *tnmmatrix<TYPE>::Mul( const tnmmatrix<TYPE> *A , const tnmmatrix<TYPE> *B )
  {
#ifdef _SAFE
    if ( A->COLS != B->ROWS ) 
      throw (string)"tnmmatrix<TYPE> *Mul( const tnmmatrix<TYPE> *A , const tnmmatrix<TYPE> *B ) : A->COLS != B->ROWS";
    //    if ( this->ROWS != A->ROWS || this->COLS != B->COLS ) 
    //      throw (string)"tnmmatrix<TYPE> *Mul( const tnmmatrix<TYPE> *A , const tnmmatrix<TYPE> *B ) : this->ROWS != A->ROWS || this->COLS != B->COLS";
#endif      
    for ( int i = 0 ; i < A->ROWS  ; i++ )
      for ( int j = 0 ; j < B->COLS  ; j++ )
	{
	  this->elements[i][j] = 0;
	  for ( int k = 0 ; k < A->COLS ; k++ )
	    this->elements[i][j] += A->elements[i][k] * B->elements[k][j];
	}
    return this;
  }

  template<class TYPE>
  tnmmatrix<TYPE> *tnmmatrix<TYPE>::MulElementwise( const tnmmatrix<TYPE> *A , const tnmmatrix<TYPE> *B )
  {
#ifdef _SAFE
    if ( A->COLS != B->COLS ) 
      throw (string)"tnmmatrix<TYPE> *MulElementwise( const tnmmatrix<TYPE> *A , const tnmmatrix<TYPE> *B ) : A->COLS != B->COLS";
    if ( A->ROWS != B->ROWS ) 
      throw (string)"tnmmatrix<TYPE> *MulElementwise( const tnmmatrix<TYPE> *A , const tnmmatrix<TYPE> *B ) : A->ROWS != B->ROWS";
    if ( this->ROWS != A->ROWS || this->COLS != A->COLS ) 
      throw (string)"tnmmatrix<TYPE> *MulElementwise( const tnmmatrix<TYPE> *A , const tnmmatrix<TYPE> *B ) : this->COLS != A->COLS || this->ROWS != A->ROWS";
#endif      
    for ( int i = 0 ; i < this->ROWS  ; i++ )
      for ( int j = 0 ; j < this->COLS  ; j++ )
	this->elements[i][j] = A->elements[i][j] * B->elements[i][j];
    return this;
  }

  template<class TYPE>
  tnmmatrix<TYPE> *tnmmatrix<TYPE>::DivElementwise( const tnmmatrix<TYPE> *A , const tnmmatrix<TYPE> *B )
  {
#ifdef _SAFE
    if ( A->COLS != B->COLS ) 
      throw (string)"tnmmatrix<TYPE> *DivElementwise( const tnmmatrix<TYPE> *A , const tnmmatrix<TYPE> *B ) : A->COLS != B->COLS";
    if ( A->ROWS != B->ROWS ) 
      throw (string)"tnmmatrix<TYPE> *DivElementwise( const tnmmatrix<TYPE> *A , const tnmmatrix<TYPE> *B ) : A->ROWS != B->ROWS";
    if ( this->ROWS != A->ROWS || this->COLS != A->COLS ) 
      throw (string)"tnmmatrix<TYPE> *DivElementwise( const tnmmatrix<TYPE> *A , const tnmmatrix<TYPE> *B ) : this->COLS != A->COLS || this->ROWS != A->ROWS";
#endif      
    for ( int i = 0 ; i < this->ROWS  ; i++ )
      for ( int j = 0 ; j < this->COLS  ; j++ )
	this->elements[i][j] = A->elements[i][j] / B->elements[i][j];
    return this;
  }

  template<class TYPE>
  tnmmatrix<TYPE> *tnmmatrix<TYPE>::Add( const TYPE *e , const tnmmatrix<TYPE> *A )
  {
    for ( int  i = 0 ; i < this->ROWS ; i++ ) 
      for ( int  j = 0 ; j < this->COLS ; j++ )
	this->elements[i][j] = *e + A->elements[i][j];
    return this;
  }

  template<class TYPE>
  tnmmatrix<TYPE> *tnmmatrix<TYPE>::Sub( const TYPE *e , const tnmmatrix<TYPE> *A )
  {
    for ( int  i = 0 ; i < this->ROWS ; i++ ) 
      for ( int  j = 0 ; j < this->COLS ; j++ )
	this->elements[i][j] = *e - A->elements[i][j];
    return this;
  }

  template<class TYPE>
  tnmmatrix<TYPE> *tnmmatrix<TYPE>::Sub( const tnmmatrix<TYPE> *A , const TYPE *e )
  {
    for ( int  i = 0 ; i < this->ROWS ; i++ ) 
      for ( int  j = 0 ; j < this->COLS ; j++ )
	this->elements[i][j] = A->elements[i][j] - *e;
    return this;
  }

  template<class TYPE>
  tnmmatrix<TYPE> *tnmmatrix<TYPE>::Mul( const TYPE *e , const tnmmatrix<TYPE> *A )
  {
    for ( int  i = 0 ; i < this->ROWS ; i++ ) 
      for ( int  j = 0 ; j < this->COLS ; j++ )
	this->elements[i][j] = *e * A->elements[i][j];
    return this;
  }

  template<class TYPE>
  TYPE tnmmatrix<TYPE>::Dot( const tnmmatrix<TYPE> *v ) const
  {
#ifdef _SAFE
    if ( this->COLS != 1 )
      throw (string)"TYPE Dot( const tnmmatrix<TYPE> *u , const tnmmatrix<TYPE> *v ) : this->COLS != 1";
    if ( v->COLS != 1 )
      throw (string)"TYPE Dot( const tnmmatrix<TYPE> *u , const tnmmatrix<TYPE> *v ) : v->COLS != 1";
    if ( this->ROWS != v->ROWS )
      throw (string)"TYPE Dot( const tnmmatrix<TYPE> *u , const tnmmatrix<TYPE> *v ) : this->ROWS != 1 v->ROWS";
#endif
    TYPE res = 0;
    for ( int i = 1 ; i <= this->ROWS ; i++ ) 
      res += this->Get(i,1) * (*v).Get(i,1); 
    return res;
  }

  template<class TYPE>
  TYPE tnmmatrix<TYPE>::Dst( const tnmmatrix<TYPE> *v ) const
  {
#ifdef _SAFE
    if ( this->COLS != 1 )
      throw (string)"TYPE Dst( const tnmmatrix<TYPE> *u , const tnmmatrix<TYPE> *v ) : this->COLS != 1";
    if ( v->COLS != 1 )
      throw (string)"TYPE Dst( const tnmmatrix<TYPE> *u , const tnmmatrix<TYPE> *v ) : v->COLS != 1";
    if ( this->ROWS != v->ROWS )
      throw (string)"TYPE Dst( const tnmmatrix<TYPE> *u , const tnmmatrix<TYPE> *v ) : this->ROWS != 1 v->ROWS";
#endif
    tnmmatrix<TYPE> tmp( *this );
    tmp.Sub( *v );
    return tmp.Nrm();
  }

  template<class TYPE>
  tnmmatrix<TYPE> *tnmmatrix<TYPE>::Crs( const tnmmatrix<TYPE> *u , const tnmmatrix<TYPE> *v )
  {
    throw (string)"tnmmatrix<TYPE> *Crs( tnmmatrix<TYPE> *res , const tnmmatrix<TYPE> *u , const tnmmatrix<TYPE> *v ) : Not implemented";
    return this;
  }

  template<class TYPE>
  tnmmatrix<TYPE> *tnmmatrix<TYPE>::Crs( const tnmmatrix<TYPE> *a , const tnmmatrix<TYPE> *b , const tnmmatrix<TYPE> *c )
  {
#ifdef _SAFE
    if ( a->COLS != 1 || b->COLS != 1 || c->COLS != 1 )
      throw (string)"tnmmatrix<TYPE> *Crs( tnmmatrix<TYPE> *res , const tnmmatrix<TYPE> *a , const tnmmatrix<TYPE> *b , const tnmmatrix<TYPE> *c ) : a->COLS != 1 || b->COLS != 1 || c->COLS != 1";
    if ( a->ROWS != b->ROWS || a->ROWS != b->ROWS )
      throw (string)"tnmmatrix<TYPE> *Crs( tnmmatrix<TYPE> *res , const tnmmatrix<TYPE> *a , const tnmmatrix<TYPE> *b , const tnmmatrix<TYPE> *c ) : a->ROWS != b->ROWS || a->ROWS != b->ROWS";
#endif

    (*this)(1,1) = 
      - (*a).Get(4,1) * ( (*b).Get(2,1) * (*c).Get(3,1) - (*b).Get(3,1) * (*c).Get(2,1) )
      + (*a).Get(3,1) * ( (*b).Get(4,1) * (*c).Get(2,1) - (*b).Get(2,1) * (*c).Get(4,1) )
      + (*a).Get(2,1) * ( (*b).Get(3,1) * (*c).Get(4,1) - (*b).Get(4,1) * (*c).Get(3,1) );
    (*this)(2,1) = 
      - (*a).Get(4,1) * ( (*b).Get(3,1) * (*c).Get(1,1) - (*b).Get(1,1) * (*c).Get(3,1) )
      + (*a).Get(3,1) * ( (*b).Get(1,1) * (*c).Get(4,1) - (*b).Get(4,1) * (*c).Get(1,1) )
      + (*a).Get(1,1) * ( (*b).Get(4,1) * (*c).Get(3,1) - (*b).Get(3,1) * (*c).Get(4,1) );
    (*this)(3,1) = 
      - (*a).Get(4,1) * ( (*b).Get(1,1) * (*c).Get(2,1) - (*b).Get(2,1) * (*c).Get(1,1) )
      + (*a).Get(2,1) * ( (*b).Get(4,1) * (*c).Get(1,1) - (*b).Get(1,1) * (*c).Get(4,1) )
      + (*a).Get(1,1) * ( (*b).Get(2,1) * (*c).Get(4,1) - (*b).Get(4,1) * (*c).Get(2,1) );
    (*this)(4,1) = 
      - (*a).Get(3,1) * ( (*b).Get(2,1) * (*c).Get(1,1) - (*b).Get(1,1) * (*c).Get(2,1) )
      + (*a).Get(2,1) * ( (*b).Get(1,1) * (*c).Get(3,1) - (*b).Get(3,1) * (*c).Get(1,1) )
      + (*a).Get(1,1) * ( (*b).Get(3,1) * (*c).Get(2,1) - (*b).Get(2,1) * (*c).Get(3,1) );
    return this;
  }

  template<class TYPE>
  int tnmmatrix<TYPE>::Less( const tnmmatrix<TYPE> *B ) const
  {
    for ( int  i = 0 ; i < this->ROWS ; i++ ) 
      for ( int  j = 0 ; j < this->COLS ; j++ )
	if ( this->elements[i][j] > B->elements[i][j] )
	  return 0;
    return 1;
  }

  template<class TYPE>
  int tnmmatrix<TYPE>::StrictlyLess( const tnmmatrix<TYPE> *B ) const
  {
    for ( int  i = 0 ; i < this->ROWS ; i++ ) 
      for ( int  j = 0 ; j < this->COLS ; j++ )
	if ( this->elements[i][j] >= B->elements[i][j] )
	  return 0;
    return 1;
  }

  template<class TYPE>
  int tnmmatrix<TYPE>::Greater( const tnmmatrix<TYPE> *B ) const
  {
    for ( int  i = 0 ; i < this->ROWS ; i++ ) 
      for ( int  j = 0 ; j < this->COLS ; j++ )
	if ( this->elements[i][j] < B->elements[i][j] )
	  return 0;
    return 1;
  }

  template<class TYPE>
  int tnmmatrix<TYPE>::StrictlyGreater( const tnmmatrix<TYPE> *B ) const
  {
    for ( int  i = 0 ; i < this->ROWS ; i++ ) 
      for ( int  j = 0 ; j < this->COLS ; j++ )
	if ( this->elements[i][j] <= B->elements[i][j] )
	  return 0;
    return 1;
  }


  template<class TYPE>
  void tnmmatrix<TYPE>::MinMax(  TYPE &min , TYPE &max ) const
  {
    min = max = this->elements[0][0];
    for ( int row = 0 ; row < this->ROWS ; row++ )
      for ( int col = 0 ; col < this->COLS ; col++ )
	{
	  if ( this->elements[row][col] < min )
	    min = this->elements[row][col]; 
	  if ( this->elements[row][col] > max )
	    max = this->elements[row][col]; 
	}
   
  }

  template<class TYPE>
  void tnmmatrix<TYPE>::MinMax( tnmmatrix<TYPE> *min , tnmmatrix<TYPE> *max ) const
  {
    for ( int row = 0 ; row < this->ROWS ; row++ )
      for ( int col = 0 ; col < this->COLS ; col++ )
	{
	  if ( this->elements[row][col] < min->elements[row][col] )
	    min->elements[row][col] = this->elements[row][col]; 
	  if ( this->elements[row][col] > max->elements[row][col] )
	    max->elements[row][col] = this->elements[row][col]; 
	}
  }

  template<class TYPE>
  tnmmatrix<TYPE> &tnmmatrix<TYPE>::Zeroize( TYPE threshold )
  {
    for ( int i = 0 ; i < this->ROWS ; i++ ) 
      for ( int j = 0 ; j < this->COLS ; j++ ) 
	if ( this->elements[i][j] < threshold &&  this->elements[i][j] > -threshold )
	  this->elements[i][j] = 0;
    return *this;
  }

  template<class TYPE>
  TYPE tnmmatrix<TYPE>::RowLength( int row ) const
  {
#ifdef _SAFE
    if ( row < 1 || row > this->ROWS )
      throw (string)"TYPE tnmmatrix<TYPE>::RowLength( int row ) const : row < 1 || row > this->ROWS"; 
#endif
    row -= 1;
    TYPE res = 0;
    for ( int col = 0 ; col < this->COLS ; col++ )
      res += this->elements[row][col] * this->elements[row][col];
    return (TYPE)sqrt( res );
  }


  template<class TYPE>
  TYPE tnmmatrix<TYPE>::ColLength( int col ) const
  {
#ifdef _SAFE
    if ( col < 1 || col > this->COLS )
      throw (string)"TYPE tnmmatrix<TYPE>::ColLength( int col ) const : col < 1 || col > this->COLS"; 
#endif
    col -= 1;
    TYPE res = 0;
    for ( int row = 0 ; row < this->ROWS ; row++ )
      res += this->elements[row][col] * this->elements[row][col];
    return (TYPE)sqrt( res );
  }
  

  template<class TYPE>
  tnmmatrix<TYPE> *tnmmatrix<TYPE>::NormalizeRow( int row )
  {
#ifdef _SAFE
    if ( row < 1 || row > this->ROWS )
      throw (string)"tnmmatrix<TYPE> *tnmmatrix<TYPE>::NormalizeRow( int row ) : row < 1 || row > this->ROWS"; 
#endif
    TYPE length = 1 / RowLength( row );
    row -= 1;
    for ( int col = 0 ; col < this->COLS ; col++ )
      this->elements[row][col] *= length;
    return this;
  }
  
  template<class TYPE>
  tnmmatrix<TYPE> *tnmmatrix<TYPE>::NormalizeCol( int col )
  {
#ifdef _SAFE
    if ( col < 1 || col > this->COLS )
      throw (string)"tnmmatrix<TYPE> *tnmmatrix<TYPE>::NormalizeCol( int col ) : col < 1 || col > this->COLS"; 
#endif
    TYPE length = 1 / ColLength( col );
    col -= 1;
    for ( int row = 0 ; row < this->ROWS ; row++ )
      this->elements[row][col] *= length;
    return this;
  }




  /***********************************************************************************************/

#ifdef _OCTAVE   
  template<class TYPE>
  tnmmatrix<TYPE> *tnmmatrix<TYPE>::Cpy( const Matrix *source ) 
  {
    for ( int i = 1 ; i <= this->ROWS ; i++ )
      for ( int j = 1 ; j <= this->COLS ; j++ )
	elements[i-1][j-1] = (TYPE)(*source)( i - 1 , j - 1 ) ;
    return this;

  }


  template<class TYPE>
  tnmmatrix<TYPE>::tnmmatrix( const Matrix &source )
  {
    Alloc();
    for ( int  i = 0 ; i < this->ROWS ; i++ ) 
      for ( int  j = 0 ; j < this->COLS ; j++ )
	elements[i][j] = (TYPE)source( i , j );
  }

  template<class TYPE>
  Matrix *tnmmatrix<TYPE>::OctMat( Matrix *res ) const
  {
    for ( int i = 1 ; i <= this->ROWS ; i++ )
      for ( int j = 1 ; j <= this->COLS ; j++ )
	(*res)( i - 1  , j - 1 ) = this->elements[i-1][j-1];
    return res;
  }

  template<class TYPE>
  tnmmatrix<TYPE> *tnmmatrix<TYPE>::OctInv( TYPE *rcond )
  {
#ifdef _SAFE
    if ( this->ROWS != this->COLS )
      _DBTHROW( (string)"tnmmatrix<TYPE> *tnmmatrix<TYPE>::Inv() : this->ROWS != this->COLS" );
#endif    

    Matrix tmp( this->ROWS , this->COLS );
    OctMat( &tmp );
    int info;
    double rc = 10;
    Matrix tmp2( this->ROWS , this->COLS );

    if ( rcond )
      {
	tmp2 = tmp.inverse( info , rc , (int)0 , (int)1 );
	*rcond = (TYPE)rc;
      } else 
      tmp2 = tmp.inverse();
    Cpy( &tmp2 );
    return this;
  }


  template<class TYPE>
  TYPE tnmmatrix<TYPE>::OctDet() const
  {
#ifdef _SAFE
    if ( this->ROWS != this->COLS )
      _DBTHROW( (string)"TYPE tnmmatrix<TYPE>::OctDet() const : this->ROWS != this->COLS" );
#endif    

    Matrix tmp( this->ROWS , this->COLS );
    OctMat( &tmp );
    DET det = tmp.determinant(); 
    return (TYPE)det.value();
  }
#endif

#ifdef _SLATEC
  template<class TYPE>
  doublereal *tnmmatrix<TYPE>::AsColumnColumnArray() const
  {
    doublereal *array = new double[this->COLS*this->ROWS];
    int ctr = 0;
    for ( int  i = 0 ; i < this->ROWS ; i++ ) 
      for ( int  j = 0 ; j < this->COLS ; j++ )
	array[ctr++] = (doublereal)this->elements[i][j];
    return  array;
  }


  template<class TYPE>
  doublereal *tnmmatrix<TYPE>::AsRowRowArray() const
  {
    doublereal *array = new double[this->COLS*this->ROWS];
    int ctr = 0;
    for ( int  j = 0 ; j < this->COLS ; j++ )
      for ( int  i = 0 ; i < this->ROWS ; i++ ) 
	array[ctr++] = (doublereal)this->elements[i][j];
    return array;
  }

  template<class TYPE>
  void tnmmatrix<TYPE>::FromColumnColumnArray( doublereal *array )
  {
#ifdef _SAFE
    if ( array == 0L )
      _DBTHROW( "array == 0L" );
#endif
    int ctr = 0;
    for ( int  i = 0 ; i < this->ROWS ; i++ ) 
      for ( int  j = 0 ; j < this->COLS ; j++ )
	this->elements[i][j] = (TYPE)array[ctr++];
    delete[] array;
  }


  template<class TYPE>
  void tnmmatrix<TYPE>::FromRowRowArray( doublereal *array  )
  {
#ifdef _SAFE
    if ( array == 0L )
      _DBTHROW( "array == 0L" );
#endif
    int ctr = 0;
    for ( int  j = 0 ; j < this->COLS ; j++ )
      for ( int  i = 0 ; i < this->ROWS ; i++ ) 
	this->elements[i][j] = (TYPE)array[ctr++];
    delete[] array;
  }


  template<class TYPE>
  tnmmatrix<TYPE> *tnmmatrix<TYPE>::SlatecInv( TYPE *rcond )
  {
#ifdef _SAFE
    if ( this->ROWS != this->COLS )
      _DBTHROW( (string)"tnmmatrix<TYPE> *tnmmatrix<TYPE>::Inv() : this->ROWS != this->COLS" );
#endif    

    doublereal  *array = AsRowRowArray();
    //    doublereal  *array = AsColumnColumnArray();
    double _rcond;
    tSlatecTools<double>::Inverse( array , (integer)this->ROWS , (integer)this->COLS , (double *)NULL , &_rcond );
    if ( rcond != 0L )
      (*rcond) = (TYPE)_rcond;
    FromRowRowArray( array );
    //    FromColumnColumnArray( array );
    return this;
  }

  template<class TYPE>
  TYPE tnmmatrix<TYPE>::SlatecDet() const
  {
#ifdef _SAFE
    if ( this->ROWS != this->COLS )
      _DBTHROW( (string)"TYPE tnmmatrix<TYPE>::SlatecDet() const : this->ROWS != this->COLS" );
#endif    
    double res = 0;
    doublereal  *array = AsColumnColumnArray();
    res = tSlatecTools<double>::Determinant( array , (integer)this->ROWS , (integer)this->COLS , NULL  );

    delete[] array;
    return (TYPE)res;

  }
#endif


#ifdef _GSL

  template<class TYPE>
  gsl_matrix *tnmmatrix<TYPE>::GSLMatrix() const
  {
    gsl_matrix *m = gsl_matrix_alloc ( this->ROWS , this->COLS );
    for ( int r = 1 ; r <= this->ROWS ; r++ )
      for ( int c = 1 ; c <= this->COLS ; c++ )
	gsl_matrix_set( m , r - 1 , c - 1 , this->Get( r , c ) );
    
    
    return m;
    
  }

  template<class TYPE>
 void tnmmatrix<TYPE>::FreeGSLMatrix( gsl_matrix **m )
 {
   gsl_matrix_free (*m );
   *m = NULL;
 }

  template<class TYPE>
  tnmmatrix<TYPE> *tnmmatrix<TYPE>::Cpy( const gsl_matrix *m )
  {
    for ( int r = 1 ; r <= this->ROWS ; r++ )
      for ( int c = 1 ; c <= this->COLS ; c++ )
	this->Ref( r , c ) =  gsl_matrix_get( m , r - 1 , c - 1 );
    return this;
  }

  template<class TYPE>
  tnmmatrix<TYPE> *tnmmatrix<TYPE>::GSLInv( TYPE *rcond )
  {
    assert( this->ROWS == this->COLS );
    TYPE det;
    int signum;
    gsl_matrix *A = GSLMatrix();
    gsl_matrix *invA = GSLMatrix();

    gsl_permutation *p = gsl_permutation_alloc ( this->ROWS );
    
    gsl_linalg_LU_decomp (A, p, &signum);
    gsl_linalg_LU_invert( A , p , invA );

    if ( rcond ) {
      det = gsl_linalg_LU_det( A , signum );
      *rcond = det;
    }
    Cpy( invA );
    
    FreeGSLMatrix( &A );
    FreeGSLMatrix( &invA );
    gsl_permutation_free( p );

    return this;
  }

  template<class TYPE>
  TYPE tnmmatrix<TYPE>::GSLDet() const
  {
    TYPE det;

    assert( this->ROWS == this->COLS );
    int signum;
    gsl_matrix *A = GSLMatrix();

    gsl_permutation *p = gsl_permutation_alloc ( this->ROWS );
    
    gsl_linalg_LU_decomp (A, p, &signum);
    det = gsl_linalg_LU_det( A , signum );
    
    FreeGSLMatrix( &A );
    gsl_permutation_free( p );
    return det;
  }
#endif

  template<class TYPE>
  bool tnmmatrix<TYPE>::Definite( int k0 , int k1 , int sign  ) const
  {
    
    if ( k0 < 0 )
      k0 = 1;
    if ( k1 < 0 )
      k1 = this->ROWS;
#ifdef _SAFE
    if ( this->ROWS != this->COLS )
      _DBTHROW( "TYPE tnmmatrix<TYPE>::Definite() const : this->ROWS != this->COLS" );
    if ( k0 < 1 )
      _DBTHROW( "TYPE tnmmatrix<TYPE>::Definite() const : k0 < 1" );
    if ( k1 > this->ROWS )
      _DBTHROW( "TYPE tnmmatrix<TYPE>::Definite() const : k1 > this->ROWS" );
    if ( k0 > k1   )
      _DBTHROW( "TYPE tnmmatrix<TYPE>::Definite() const : k0 > k1" );
#endif    
    if ( k0 == 1 )
      {
	if ( ((TYPE)sign )  * this->Get(1,1) <= 0 )
	  return false;
	k0 = 2;
      }
    tnmmatrix<TYPE> A( this->COLS , this->ROWS );
    this->Trp( &A );
    A.Add( this );
    A.Mul( (TYPE)0.5 );

    for ( int k = k0 ; k <= k1 ; k++ )
      {
	tnmmatrix<TYPE> Ak( k , k , A );
	Ak.Print( std::cout , (string)"minor " + STLStringTools::AsString( k ) + ": \t" , 50 );

	if (  (TYPE)pow( sign , k ) * (TYPE)Ak.Det() <= 0 )
	  return false;
      }
    
    return true;
  }

  template<class TYPE>
  bool tnmmatrix<TYPE>::Definite( tnmmatrix<TYPE> *B , int _switch  )
  {
#ifdef _SAFE
    if ( this->ROWS != this->COLS )
      _DBTHROW( "bool tnmmatrix<TYPE>::PositiveDefinite( const tnmmatrix<TYPE> *B  ) const : this->ROWS != this->COLS" );
    if ( this->COLS != B->GetCols() )
      _DBTHROW( "bool tnmmatrix<TYPE>::PositiveDefinite( const tnmmatrix<TYPE> *B  ) const : this->COLS != B->GetCols()" );
#endif
    int k = B->GetRows();
    int n = this->GetRows();

    tnmmatrix<TYPE> C( n + k , n + k , (TYPE)0 );

    tnmmatrix<TYPE> BT( B->GetCols() , B->GetRows() );
    if ( B->FullColumnRank( &DefiniteHlp , (void *)this ) < B->GetRows() ) 
      {
	return false;
      }

    B->Trp( &BT );

    C.tnmContainer<TYPE>::Cpy( B , 
	   1 , 1 , k ,
	   1 + k , 1 , n );

    C.tnmContainer<TYPE>::Cpy( &BT , 
	   1 + k , 1 , n ,
	   1 , 1 , k );
 
    C.tnmContainer<TYPE>::Cpy( this , 
	   1 + k , 1 , n ,
	   1 + k , 1 , n );
    

    for (  int r = k + 1 ; r <= n ; r++ )
      {
	tnmmatrix<TYPE> Cl( k + r  , k + r  , C );
	if (  (TYPE)pow( -1 , _switch * ( r ) + ( 1 - _switch ) * ( k ) ) * Cl.Det() <= 0 )
	  return false;
      }
    return true;
  }

  template<class TYPE>
  void tnmmatrix<TYPE>::DefiniteHlp( int c0 , int c1 , void *_A  )
  {
#ifdef _SAFE
    if ( _A == 0L )
      _DBTHROW( "No matrix supplied!" );
#endif
    tnmmatrix<TYPE> *A = (tnmmatrix<TYPE> *)_A;
    A->tnmContainer<TYPE>::SwapRows( c0 , c1 );
    A->tnmContainer<TYPE>::SwapColumns( c0 , c1 );
  }
  template<class TYPE>
  int tnmmatrix<TYPE>::FullColumnRank( void (*OnSwap)( int col0 , int col1 , void *) , void *dta )
  {
    int i = 1;
    int j = this->COLS;
    TYPE det;
    while ( i < j && i < this->ROWS + 1 )
      {
	do {

	  tnmmatrix<TYPE> Am( i  , i  , *this  );

	  if ( ( det =  (TYPE)Am.Det() ) == (TYPE)0 )
	    {
	      this->SwapColumns( i , j );
	      if ( OnSwap != 0L )
		{	
		  (*OnSwap)( i , j , dta );
		}
	      j--;
	      if ( j <= i )
		return i - 1;
	    }
	} while( det == 0 );
	i++;
      };
    return i - 1;
  }  


  template class P4th::tnmmatrix<float>;
  template class P4th::tnmmatrix<double>;

}




  /*
    This causes bug in:
    gcc version 3.3.5-20050130 (Gentoo Linux 3.3.5.20050130-r1, ssp-3.3.5.20050130-1, pie-8.7.7.1)

    template P4th::tnmmatrix<double> ColumnVector( const double &x1 );
    template P4th::tnmmatrix<double> ColumnVector( const double & , const double & );
  */

#ifndef MTH

namespace P4th
{

  template<class TYPE,int SROWS,int SCOLS>
    tnmmatrix<TYPE> *Cpy( tnmmatrix<TYPE> *dest , const tmatrix<TYPE,SROWS,SCOLS> *source )
  {
    if ( dest->GetRows() != SROWS || dest->GetCols() != SCOLS )
      dest->Resize( SROWS , SCOLS );
    for ( int i = 1 ; i <= SROWS ; i++ )
      for ( int j = 1 ; j <= SCOLS ; j++ )
	(*dest)(i,j) = source->Get(i,j);
    return dest;
  }

  template<class TYPE,int DROWS,int DCOLS>
  tmatrix<TYPE,DROWS,DCOLS> *Cpy( tmatrix<TYPE,DROWS,DCOLS> *dest , const tnmmatrix<TYPE> *source )
  {
    if ( source->GetRows() != DROWS || source->GetCols() != DCOLS )
      _DBTHROW( (string)"tmatrix<TYPE,DROWS,DCOLS> *Cpy( tmatrix<TYPE,DROWS,DCOLS> *dest , const tnmmatrix<TYPE> *source ) : source->GetRows() != DROWS || source->GetCols() != DCOLS" );
    for ( int i = 1 ; i <= DROWS ; i++ )
      for ( int j = 1 ; j <= DCOLS ; j++ )
	(*dest)(i,j) = source->Get(i,j);
    return dest;
  }

template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,0,0> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,1,0> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,2,0> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,3,0> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,4,0> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,5,0> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,6,0> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,7,0> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,8,0> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,9,0> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,10,0> *source );

template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,0,1> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,1,1> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,2,1> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,3,1> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,4,1> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,5,1> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,6,1> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,7,1> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,8,1> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,9,1> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,10,1> *source );

template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,0,2> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,1,2> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,2,2> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,3,2> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,4,2> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,5,2> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,6,2> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,7,2> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,8,2> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,9,2> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,10,2> *source );

template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,0,3> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,1,3> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,2,3> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,3,3> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,4,3> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,5,3> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,6,3> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,7,3> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,8,3> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,9,3> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,10,3> *source );

template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,0,4> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,1,4> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,2,4> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,3,4> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,4,4> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,5,4> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,6,4> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,7,4> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,8,4> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,9,4> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,10,4> *source );

template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,0,5> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,1,5> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,2,5> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,3,5> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,4,5> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,5,5> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,6,5> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,7,5> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,8,5> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,9,5> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,10,5> *source );

template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,0,6> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,1,6> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,2,6> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,3,6> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,4,6> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,5,6> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,6,6> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,7,6> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,8,6> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,9,6> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,10,6> *source );

template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,0,7> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,1,7> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,2,7> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,3,7> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,4,7> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,5,7> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,6,7> *source );
  template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,7,7> *source );
  template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,8,7> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,9,7> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,10,7> *source );

template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,0,8> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,1,8> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,2,8> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,3,8> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,4,8> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,5,8> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,6,8> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,7,8> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,8,8> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,9,8> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,10,8> *source );

template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,0,9> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,1,9> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,2,9> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,3,9> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,4,9> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,5,9> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,6,9> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,7,9> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,8,9> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,9,9> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,10,9> *source );

template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,0,10> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,1,10> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,2,10> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,3,10> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,4,10> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,5,10> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,6,10> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,7,10> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,8,10> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,9,10> *source );
template tnmmatrix<double> *Cpy( tnmmatrix<double> *dest , const tmatrix<double,10,10> *source );



  template tmatrix<double,0,0> *Cpy( tmatrix<double,0,0> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,0,1> *Cpy( tmatrix<double,0,1> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,0,2> *Cpy( tmatrix<double,0,2> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,0,3> *Cpy( tmatrix<double,0,3> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,0,4> *Cpy( tmatrix<double,0,4> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,0,5> *Cpy( tmatrix<double,0,5> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,0,6> *Cpy( tmatrix<double,0,6> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,0,7> *Cpy( tmatrix<double,0,7> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,0,8> *Cpy( tmatrix<double,0,8> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,0,9> *Cpy( tmatrix<double,0,9> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,0,10> *Cpy( tmatrix<double,0,10> *dest , const tnmmatrix<double> *source );
  
  template tmatrix<double,1,0> *Cpy( tmatrix<double,1,0> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,1,1> *Cpy( tmatrix<double,1,1> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,1,2> *Cpy( tmatrix<double,1,2> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,1,3> *Cpy( tmatrix<double,1,3> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,1,4> *Cpy( tmatrix<double,1,4> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,1,5> *Cpy( tmatrix<double,1,5> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,1,6> *Cpy( tmatrix<double,1,6> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,1,7> *Cpy( tmatrix<double,1,7> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,1,8> *Cpy( tmatrix<double,1,8> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,1,9> *Cpy( tmatrix<double,1,9> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,1,10> *Cpy( tmatrix<double,1,10> *dest , const tnmmatrix<double> *source );
  
  template tmatrix<double,2,0> *Cpy( tmatrix<double,2,0> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,2,1> *Cpy( tmatrix<double,2,1> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,2,2> *Cpy( tmatrix<double,2,2> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,2,3> *Cpy( tmatrix<double,2,3> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,2,4> *Cpy( tmatrix<double,2,4> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,2,5> *Cpy( tmatrix<double,2,5> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,2,6> *Cpy( tmatrix<double,2,6> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,2,7> *Cpy( tmatrix<double,2,7> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,2,8> *Cpy( tmatrix<double,2,8> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,2,9> *Cpy( tmatrix<double,2,9> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,2,10> *Cpy( tmatrix<double,2,10> *dest , const tnmmatrix<double> *source );
  
  template tmatrix<double,3,0> *Cpy( tmatrix<double,3,0> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,3,1> *Cpy( tmatrix<double,3,1> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,3,2> *Cpy( tmatrix<double,3,2> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,3,3> *Cpy( tmatrix<double,3,3> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,3,4> *Cpy( tmatrix<double,3,4> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,3,5> *Cpy( tmatrix<double,3,5> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,3,6> *Cpy( tmatrix<double,3,6> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,3,7> *Cpy( tmatrix<double,3,7> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,3,8> *Cpy( tmatrix<double,3,8> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,3,9> *Cpy( tmatrix<double,3,9> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,3,10> *Cpy( tmatrix<double,3,10> *dest , const tnmmatrix<double> *source );

  template tmatrix<double,4,0> *Cpy( tmatrix<double,4,0> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,4,1> *Cpy( tmatrix<double,4,1> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,4,2> *Cpy( tmatrix<double,4,2> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,4,3> *Cpy( tmatrix<double,4,3> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,4,4> *Cpy( tmatrix<double,4,4> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,4,5> *Cpy( tmatrix<double,4,5> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,4,6> *Cpy( tmatrix<double,4,6> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,4,7> *Cpy( tmatrix<double,4,7> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,4,8> *Cpy( tmatrix<double,4,8> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,4,9> *Cpy( tmatrix<double,4,9> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,4,10> *Cpy( tmatrix<double,4,10> *dest , const tnmmatrix<double> *source );
  
  template tmatrix<double,5,0> *Cpy( tmatrix<double,5,0> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,5,1> *Cpy( tmatrix<double,5,1> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,5,2> *Cpy( tmatrix<double,5,2> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,5,3> *Cpy( tmatrix<double,5,3> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,5,4> *Cpy( tmatrix<double,5,4> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,5,5> *Cpy( tmatrix<double,5,5> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,5,6> *Cpy( tmatrix<double,5,6> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,5,7> *Cpy( tmatrix<double,5,7> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,5,8> *Cpy( tmatrix<double,5,8> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,5,9> *Cpy( tmatrix<double,5,9> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,5,10> *Cpy( tmatrix<double,5,10> *dest , const tnmmatrix<double> *source );
  
  template tmatrix<double,6,0> *Cpy( tmatrix<double,6,0> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,6,1> *Cpy( tmatrix<double,6,1> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,6,2> *Cpy( tmatrix<double,6,2> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,6,3> *Cpy( tmatrix<double,6,3> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,6,4> *Cpy( tmatrix<double,6,4> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,6,5> *Cpy( tmatrix<double,6,5> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,6,6> *Cpy( tmatrix<double,6,6> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,6,7> *Cpy( tmatrix<double,6,7> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,6,8> *Cpy( tmatrix<double,6,8> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,6,9> *Cpy( tmatrix<double,6,9> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,6,10> *Cpy( tmatrix<double,6,10> *dest , const tnmmatrix<double> *source );
  
  template tmatrix<double,7,0> *Cpy( tmatrix<double,7,0> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,7,1> *Cpy( tmatrix<double,7,1> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,7,2> *Cpy( tmatrix<double,7,2> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,7,3> *Cpy( tmatrix<double,7,3> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,7,4> *Cpy( tmatrix<double,7,4> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,7,5> *Cpy( tmatrix<double,7,5> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,7,6> *Cpy( tmatrix<double,7,6> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,7,7> *Cpy( tmatrix<double,7,7> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,7,8> *Cpy( tmatrix<double,7,8> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,7,9> *Cpy( tmatrix<double,7,9> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,7,10> *Cpy( tmatrix<double,7,10> *dest , const tnmmatrix<double> *source );
  
  template tmatrix<double,8,0> *Cpy( tmatrix<double,8,0> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,8,1> *Cpy( tmatrix<double,8,1> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,8,2> *Cpy( tmatrix<double,8,2> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,8,3> *Cpy( tmatrix<double,8,3> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,8,4> *Cpy( tmatrix<double,8,4> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,8,5> *Cpy( tmatrix<double,8,5> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,8,6> *Cpy( tmatrix<double,8,6> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,8,7> *Cpy( tmatrix<double,8,7> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,8,8> *Cpy( tmatrix<double,8,8> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,8,9> *Cpy( tmatrix<double,8,9> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,8,10> *Cpy( tmatrix<double,8,10> *dest , const tnmmatrix<double> *source );
  
  template tmatrix<double,9,0> *Cpy( tmatrix<double,9,0> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,9,1> *Cpy( tmatrix<double,9,1> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,9,2> *Cpy( tmatrix<double,9,2> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,9,3> *Cpy( tmatrix<double,9,3> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,9,4> *Cpy( tmatrix<double,9,4> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,9,5> *Cpy( tmatrix<double,9,5> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,9,6> *Cpy( tmatrix<double,9,6> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,9,7> *Cpy( tmatrix<double,9,7> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,9,8> *Cpy( tmatrix<double,9,8> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,9,9> *Cpy( tmatrix<double,9,9> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,9,10> *Cpy( tmatrix<double,9,10> *dest , const tnmmatrix<double> *source );
  
  template tmatrix<double,10,0> *Cpy( tmatrix<double,10,0> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,10,1> *Cpy( tmatrix<double,10,1> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,10,2> *Cpy( tmatrix<double,10,2> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,10,3> *Cpy( tmatrix<double,10,3> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,10,4> *Cpy( tmatrix<double,10,4> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,10,5> *Cpy( tmatrix<double,10,5> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,10,6> *Cpy( tmatrix<double,10,6> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,10,7> *Cpy( tmatrix<double,10,7> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,10,8> *Cpy( tmatrix<double,10,8> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,10,9> *Cpy( tmatrix<double,10,9> *dest , const tnmmatrix<double> *source );
  template tmatrix<double,10,10> *Cpy( tmatrix<double,10,10> *dest , const tnmmatrix<double> *source );
  
}

#endif
