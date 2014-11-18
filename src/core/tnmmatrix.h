/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#ifndef _P4TH_TNMMATRIX_H_
#define _P4TH_TNMMATRIX_H_

#include <string>
#include <vector>
#include <algorithm>
#include <math.h>
#include <tnmcontainer.h>
#include <debug.h>

#ifndef MTH
#include "tmatrix.h"
#endif

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
#endif

using namespace std;

class Matrix;

namespace P4th
{
  
  template<class TYPE> 
    class tnmmatrix : public tnmContainer<TYPE>
    {
    protected:
    public:
    tnmmatrix() : tnmContainer<TYPE>()
      {}
      
    tnmmatrix( int R  , int C , const string &db = "" ) : tnmContainer<TYPE>( R , C , db )
      {}
      
    tnmmatrix( int R  , int C , const tnmmatrix &source ) : tnmContainer<TYPE>( R , C , source )
      {}

    tnmmatrix( const tnmmatrix &source ) : tnmContainer<TYPE>( source )
      {}
    tnmmatrix( int R , int C , TYPE e );
    tnmmatrix( int R , int C , TYPE **es ); 
      //    virtual ~tnmmatrix();

    tnmmatrix( const tnmContainer<TYPE> &source ) : tnmContainer<TYPE>( source )
      {}

    virtual int IsNaN() const;
    virtual int IsNumber() const;
    virtual tnmmatrix &operator=( const tnmmatrix &b ) 
      { 
	tnmContainer<TYPE>::SafeCopy( b );
	return *this;
      }
    virtual TYPE &operator()( int i , int j ) 
    { return tnmContainer<TYPE>::operator()( i , j ); }
    
    virtual tnmmatrix *I();
    virtual tnmmatrix *Abs();

    virtual tnmmatrix *Add( const tnmmatrix *B );
    virtual tnmmatrix &Add( const tnmmatrix &B )
    { return *(Add( &B)); }
    virtual tnmmatrix *Add( const tnmmatrix *A , const tnmmatrix *B );
    virtual tnmmatrix &Add( const tnmmatrix &A , const tnmmatrix &B )
    { Add( &A , &B ); return *this; }
    tnmmatrix operator+( const tnmmatrix &B ) const
      { tnmmatrix res( this->GetRows() , this->GetCols() ); 
	return res.Sub( *this , B ); 
      }  

    virtual tnmmatrix *Sub( const tnmmatrix *B );
    virtual tnmmatrix &Sub( const tnmmatrix &B )
    { return *(Sub( &B)); }
    virtual tnmmatrix *Sub( const tnmmatrix *A , const tnmmatrix *B );
    virtual tnmmatrix &Sub( const tnmmatrix &A , const tnmmatrix &B )
    { Sub( &A , &B ); return *this; }
    tnmmatrix operator-( const tnmmatrix &B ) const
      { tnmmatrix res( this->GetRows() , this->GetCols() ); 
	return res.Sub( *this , B ); 
      }  
    tnmmatrix operator-() const
      { tnmmatrix res( this->GetRows() , this->GetCols() ); 
	return res.Mul( -1.0 ); 
      }  

    tnmmatrix *Mul( const tnmmatrix *A , const tnmmatrix *B );
    tnmmatrix &Mul( const tnmmatrix &A , const tnmmatrix &B )
    { return *Mul(&A , &B ); }
    tnmmatrix operator*( const tnmmatrix &B ) const
      { tnmmatrix res( this->GetRows() , B.GetCols() ); 
	return res.Mul( *this , B ); 
      }  
    
    tnmmatrix *MulElementwise( const tnmmatrix *A , const tnmmatrix *B );
    tnmmatrix &MulElementwise( const tnmmatrix &A , const tnmmatrix &B )
    { return *MulElementwise(&A , &B ); }
    
    tnmmatrix *DivElementwise( const tnmmatrix *A , const tnmmatrix *B );
    tnmmatrix &DivElementwise( const tnmmatrix &A , const tnmmatrix &B )
    { return *DivElementwise(&A , &B ); }
    
    virtual tnmmatrix *Add( const TYPE *e );
    virtual tnmmatrix &Add( const TYPE e )
    { return *Add( &e); }
    virtual tnmmatrix *Sub( const TYPE *e );
    virtual tnmmatrix &Sub( const TYPE e )
    { return *Sub( &e); }
    virtual tnmmatrix *Mul( const TYPE *e );
    virtual tnmmatrix &Mul( const TYPE e )
    { return *Mul( &e); }
    tnmmatrix operator+( const TYPE &e ) const
      { tnmmatrix res( *this ); 
	return res.Add( e ); 
      }  
    tnmmatrix operator-( const TYPE &e ) const
      { tnmmatrix res( *this ); 
	return res.Sub( e ); 
      }  
    tnmmatrix operator*( const TYPE &e ) const
      { tnmmatrix res( *this ); 
	return res.Mul( e ); 
      }  
    virtual tnmmatrix *Add( const TYPE *e , const tnmmatrix *A );
    virtual tnmmatrix &Add( const TYPE e , const tnmmatrix &A )
    { return *Add( &e , &A ); }
    virtual tnmmatrix *Sub( const TYPE *e , const tnmmatrix *A );
    virtual tnmmatrix *Sub( const tnmmatrix *A , const TYPE *e );
    virtual tnmmatrix *Mul( const TYPE *e , const tnmmatrix *A );
    virtual tnmmatrix &Mul( const TYPE e , const tnmmatrix &A )
    { return *Mul( &e , &A ); }

    TYPE Dst( const tnmmatrix *v ) const;
    TYPE Dst( const tnmmatrix &v ) const
    { return Dst( &v ); }

    TYPE Nrm() const
    { return (TYPE)sqrt( this->Dot( this ) ); }

    TYPE Dot( const tnmmatrix *v ) const;
    TYPE Dot( const tnmmatrix &v ) const
    { return Dot( &v ); }

    tnmmatrix *Crs( const tnmmatrix *u , const tnmmatrix *v );
    tnmmatrix &Crs( const tnmmatrix &u , const tnmmatrix &v )
    { return *(Crs( &u , &v )); }
    
    tnmmatrix *Crs( const tnmmatrix *a , const tnmmatrix *b , const tnmmatrix *c );
    tnmmatrix &Crs( const tnmmatrix &a , const tnmmatrix &b , const tnmmatrix &c )
    { return *(Crs( &a , &b , &c )); }
    
    virtual int Less( const tnmmatrix *B ) const;
    virtual int StrictlyLess( const tnmmatrix *B ) const;
    virtual int Greater( const tnmmatrix *B  ) const;
    virtual int StrictlyGreater( const tnmmatrix *B ) const;

    virtual int Less( const tnmmatrix &B ) const
    { return Less( &B ); }
    virtual int StrictlyLess( const tnmmatrix &B ) const
    { return StrictlyLess( &B ); }
    virtual int Greater( const tnmmatrix &B ) const
    { return Greater( &B ); }
    virtual int StrictlyGreater( const tnmmatrix &B ) const
    { return StrictlyGreater( &B ); }
    
    virtual void MinMax( TYPE &min , TYPE &max ) const; 

    virtual void MinMax( tnmmatrix *min , tnmmatrix *max ) const; 
    virtual void MinMax( tnmmatrix &min , tnmmatrix &max ) const
    { MinMax( &min , &max ); }
    
    virtual int operator==( const tnmmatrix &A ) const
    { return tnmContainer<TYPE>::operator==( A ); }
    virtual int operator!=( const tnmmatrix &A ) const
    { return ! ((*this) == A);  }

    virtual operator TYPE() 
    { return tnmContainer<TYPE>::operator TYPE(); }
    virtual operator bool()
    { return tnmContainer<TYPE>::operator bool(); }

    virtual tnmmatrix &Zeroize( TYPE threshold = 0.0000000001 );

    virtual TYPE RowLength( int row = 1 ) const;
    virtual TYPE ColLength( int col = 1 ) const;
    virtual TYPE RowLength( const tnmmatrix *mat , int row = 1 ) const
    { return mat->RowLength( row ); }
    virtual TYPE ColLength( const tnmmatrix *mat , int col = 1 ) const
    { return mat->ColLength( col ); }
    virtual TYPE RowLength( const tnmmatrix &mat , int row = 1 ) const
    { return mat.RowLength( row ); }
    virtual TYPE ColLength( const tnmmatrix &mat , int col = 1 ) const
    { return mat.ColLength( col ); }
 
    virtual tnmmatrix *NormalizeRow( int row = 1 );
    virtual tnmmatrix *NormalizeCol( int col = 1 );

#ifdef _OCTAVE
    virtual tnmmatrix *Inv( TYPE *rcond = 0L )
    virtual TYPE Det() const
    { return OctDet(); }

    tnmmatrix *Cpy( const Matrix *source );
    tnmmatrix &Cpy( const Matrix &source )
    { this->Cpy( &source ); return *this; }

    tnmmatrix( const Matrix &source );
    virtual Matrix *OctMat( Matrix *res ) const;
    virtual Matrix &OctMat( Matrix &res ) const
    { return *OctMat( &res ); }

    virtual tnmmatrix *OctInv( TYPE *rcond = 0L ); // invert this matrix
    virtual TYPE OctDet() const;
    { return OctInv( rcond );  }

    /*
    TYPE Cnd() const;
    int Rnk() const;
    */
#endif
#ifdef _SLATEC
    virtual tnmmatrix *Inv( TYPE *rcond = 0L )
    { return SlatecInv( rcond );  }
    virtual TYPE Det() const
    { return SlatecDet(); }
    doublereal *AsRowRowArray() const;
    doublereal *AsColumnColumnArray() const;
    void FromRowRowArray( doublereal *array );
    void FromColumnColumnArray( doublereal *array );
    virtual tnmmatrix *SlatecInv( TYPE *rcond = 0L ); // invert this matrix
    virtual TYPE SlatecDet() const;
#endif
#ifdef _GSL
    virtual tnmmatrix *Inv( TYPE *rcond = 0L )
    { return GSLInv( rcond );  }
    virtual TYPE Det() const
    { return GSLDet(); }
    virtual gsl_matrix *GSLMatrix() const;
    static void FreeGSLMatrix( gsl_matrix **m );
    virtual tnmmatrix *Cpy( const gsl_matrix *m );
    virtual tnmmatrix *GSLInv( TYPE *rcond = 0L ); // invert this matrix
    virtual TYPE GSLDet() const;
#endif

#if defined _GSL || defined _SLATEC || defined _OCTAVE
    virtual tnmmatrix *Inv( tnmmatrix *res , TYPE *rcond = 0L ) const
    { 
      res->tnmContainer<TYPE>::Cpy( this ); 
      res->Inv( rcond  ); 
      return res;
    }
    virtual tnmmatrix &Inv( tnmmatrix &res , TYPE *rcond = 0L ) const
    { return *(Inv( &res , rcond )); }
#endif

    virtual bool Definite( int k0 , int k1, int sign ) const;
    virtual bool PositiveDefinite( int k0 = -1 , int k1 = - 1 ) const
      { return Definite( k0 , k1 , 1 ); }
    virtual bool NegativeDefinite( int k0 = - 1, int k1 = - 1) const
      { return Definite( k0 , k1 , -1 ); }

    virtual bool Definite( tnmmatrix *B , int _switch  );
    static void DefiniteHlp( int c0 , int c1 , void *_A  );
    virtual bool PositiveDefinite( tnmmatrix<TYPE> *B  )
      { return Definite( B , 0 ); }
    virtual bool NegativeDefinite( tnmmatrix<TYPE> *B  )
      { return Definite( B , 1 ); }

    int FullColumnRank( void (*OnSwap)( int , int , void *) = 0L , void *dta = 0L );
    
 };
  
#ifndef MTH
  template<class TYPE,int SROWS,int SCOLS>
    tnmmatrix<TYPE> *Cpy( tnmmatrix<TYPE> *dest , const tmatrix<TYPE,SROWS,SCOLS> *source );

  template<class TYPE,int DROWS,int DCOLS>
    tmatrix<TYPE,DROWS,DCOLS> *Cpy( tmatrix<TYPE,DROWS,DCOLS> *dest , const tnmmatrix<TYPE> *source );
#endif
  
  template<class TYPE>
    tnmmatrix<TYPE> operator+( const TYPE &e , const tnmmatrix<TYPE> &A  )
    { tnmmatrix<TYPE> res( A ); 
	return res.Add( e ); 
      }  
  template<class TYPE>
    tnmmatrix<TYPE> operator-( const TYPE &e , const tnmmatrix<TYPE> &A  )
    { tnmmatrix<TYPE> res( A.GetRows() , A.GetCols() ); 
      res.Sub( e , A ); 
    }  
  template<class TYPE>
    tnmmatrix<TYPE> operator*( const TYPE &e , const tnmmatrix<TYPE> &A  )
    { tnmmatrix<TYPE> res( A ); 
	return res.Mul( e ); 
      }  

}



#endif
