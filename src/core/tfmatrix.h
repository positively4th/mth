/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#ifndef _P4TH_TFMATRIX_H_
#define _P4TH_TFMATRIX_H_

#include <debug.h>
#include "tfmatrix0.h"
#include <string>
#include "linkedptrlist.h"
#include "tfunction0.h"
#include "tnmmatrix.h"


#ifndef MTH
#include "tmatrixalias.h"
#endif

//class istream;
//class ostream;

using namespace std;

namespace P4th
{

  class Polymorphic;

  template<class TYPE> 
    class tfMatrix : public tfMatrix0<TYPE>
 {
 public:
   virtual int Nodes() const;
   protected:
    static string _FILETAG;
    int COLS;
    int ROWS;
    tFunction0<TYPE> ***elements;

    virtual void Alloc();
    virtual void Delete();

    static void Assign( tFunction0<TYPE> **to , tFunction0<TYPE> *from ) 
    { if ( *to != 0L || *to != from ) delete *to; *to = from; }
    virtual void Resize( const tfMatrix<TYPE> &m )
    { if ( COLS != m.COLS || ROWS != m.ROWS ) 
	Resize( m.ROWS , m.COLS ); }

    tfMatrix()
      { ROWS = 0; COLS = 0; elements = 0L; }
  public:
    tfMatrix( int R  , int C = 1 , tFunction0<TYPE> *f0 = 0L );
    tfMatrix( int R  , int C , TYPE  );
    tfMatrix( TYPE  );
    tfMatrix( const tnmmatrix<TYPE> &m  );
    tfMatrix( tFunction0<TYPE> *f );
    tfMatrix( const tfMatrix<TYPE> &source );
    virtual ~tfMatrix();

    static tfMatrix *RowVector( int rows , tFunction0<TYPE> *e , ... );
    static tfMatrix *ColumnVector( int cols , tFunction0<TYPE> *e , ... );

    virtual tFunction0<TYPE> **EPA( int i0based , int j0based )
      { return &elements[i0based][j0based]; }
    virtual const tFunction0<TYPE> *EP( int i0based , int j0based ) const
      { return elements[i0based][j0based]; }

    
    virtual void Resize( int R , int C );

    virtual tfMatrix0<TYPE> *Cpy( const TYPE &a );
    virtual tfMatrix0<TYPE> *Cpy( const tFunction0<TYPE> *f );
    virtual tfMatrix0<TYPE> *Cpy( const tfMatrix<TYPE> &source );
    virtual tFunction0<TYPE> *Cpy( int i , int j = 1 ) const;


    virtual int GetRows() const
    { return ROWS; }
    virtual int GetCols() const
    { return COLS; }
    virtual bool DimOk( int d ) const;

    virtual tfMatrix0<TYPE> *operator=( const tfMatrix &b )
      { return Cpy( b ); }
    virtual tfMatrix0<TYPE> *operator=( const tnmmatrix<TYPE> &b  );
    virtual tfMatrix0<TYPE> *operator=( const TYPE &number );
    virtual tfMatrix0<TYPE> *operator=( tFunction0<TYPE> *f );
    //    virtual void Dim( int _dim );

    virtual tfMatrix0<TYPE> *Clone() const
    { return new tfMatrix<TYPE>( *this ); }

    virtual bool InitDerivatives( bool analytical , int depth , int dim  );
    /*
    virtual void NumericalDerivatives();
    virtual void AnalyticalDerivatives();
    */

    virtual tfMatrix0<TYPE> *Simplify();
    
    virtual tFunction0<TYPE> *Ref( int i , int j = 1 );
    virtual const tFunction0<TYPE> *operator()( int i , int j = 1 ) const;
    


    virtual operator const tFunction0<TYPE> *() const
      {
	_DB( std::cout << "Casting Matrix to Scalar!" << std::endl; );
	if ( GetRows() != 1 && GetCols() != 1 )
	  _DBTHROW( "Error csting matrix/vector to scalar : ROWS != 1 && COLS != 1" );
	if ( elements[0][0] == 0L )
	  _DBTHROW( "Error csting matrix/vector to scalar : not initialized" );
	return elements[0][0];
      }

    virtual tFunction0<TYPE> *Set( int i , int j  , tFunction0<TYPE> *f );
    virtual tFunction0<TYPE> *Steal( int i , int j   );
    

    virtual tfMatrix0<TYPE> *I();
    static tfMatrix0<TYPE> *I( int rows = 1 );

    virtual tfMatrix0<TYPE> *Trp( tfMatrix0<TYPE> *res ) const;
    virtual tfMatrix0<TYPE> *Trp() const
    { tfMatrix0<TYPE> *res = new tfMatrix<TYPE>( GetRows() , GetCols() ); return Trp( res ); }

    virtual tfMatrix0<TYPE> *Abs( tfMatrix0<TYPE> *res ) const;
    virtual tfMatrix0<TYPE> *Abs() const
    { tfMatrix0<TYPE> *res = new tfMatrix<TYPE>( GetRows() , GetCols() ); return Abs( res ); }


    virtual tfMatrix0<TYPE> *Add( tfMatrix0<TYPE> *res , const tfMatrix0<TYPE> *B );
    virtual tfMatrix0<TYPE> *Add( const tfMatrix0<TYPE> *B )
    { tfMatrix0<TYPE> *res = new tfMatrix<TYPE>( GetRows() , GetCols() ); return Add( res , B ); }

    virtual tfMatrix0<TYPE> *Sub( tfMatrix0<TYPE> *res , const tfMatrix0<TYPE> *B );
    virtual tfMatrix0<TYPE> *Sub( const tfMatrix0<TYPE> *B )
    { tfMatrix0<TYPE> *res = new tfMatrix<TYPE>( GetRows() , GetCols() ); return Sub( res , B ); }

    virtual tfMatrix0<TYPE> *Mul( tfMatrix0<TYPE> *res , const tfMatrix0<TYPE> *B );
    virtual tfMatrix0<TYPE> *Mul( const tfMatrix0<TYPE> *B )
    { tfMatrix0<TYPE> *res = new tfMatrix<TYPE>( GetRows() , GetCols() ); return Mul( res , B ); }

    virtual int ForEvery( const tFunction0<TYPE> *(*func)( const tFunction0<TYPE> * , 
							   int row , int col , Polymorphic * ) , Polymorphic *something = 0L ) const;

   
    virtual tfMatrix0<TYPE> *Cofactor( int i , int j ) const;
    virtual tfMatrix0<TYPE> *Row( int row ) const;
    virtual tfMatrix0<TYPE> *Column( int col ) const;
    virtual tfMatrix0<TYPE> *Negative() const;

    virtual tnmmatrix<TYPE> y( const tnmmatrix<TYPE> &x ) const;
    virtual tfMatrix0<TYPE> *Reduce( int argnr , tFunction0<TYPE> *s ) const;
    
    virtual std::ostream &Dump( std::ostream &dest , string lm = "" ) const; 

    static tfMatrix0<TYPE> *Add( const tfMatrix0<TYPE> *l , const tfMatrix0<TYPE> *r );
    static tfMatrix0<TYPE> *Sub( const tfMatrix0<TYPE> *l , const tfMatrix0<TYPE> *r );
    static tfMatrix0<TYPE> *Mul( const tfMatrix0<TYPE> *l , const tfMatrix0<TYPE> *r );
        
    /*
    static tfMatrix0<TYPE> *AddClone( const tfMatrix0<TYPE> *l , const tfMatrix0<TYPE> *r )
      { return AddIt( l->Clone() , r->Clone() ); }
    static tfMatrix0<TYPE> *SubClone( const tfMatrix0<TYPE> *l , const tfMatrix0<TYPE> *r )
    { return SubIt( l->Clone() , r->Clone() ); }
    static tfMatrix0<TYPE> *MulClone( const tfMatrix0<TYPE> *l , const tfMatrix0<TYPE> *r )
    { return MulIt( l->Clone() , r->Clone() ); }
    */
        
    static tfMatrix0<TYPE> *MulElements( const tfMatrix0<TYPE> *l , const tFunction0<TYPE> *r );
    static tfMatrix0<TYPE> *DivElements( const tfMatrix0<TYPE> *l , const tFunction0<TYPE> *r );
    static tfMatrix0<TYPE> *PowElements( const tfMatrix0<TYPE> *l , const tFunction0<TYPE> *r );
    static tfMatrix0<TYPE> *MulElements( const tFunction0<TYPE> *r , const tfMatrix0<TYPE> *l )
      { return MulElements( l , r ); }
    static tfMatrix0<TYPE> *DivElements( const tFunction0<TYPE> *r , const tfMatrix0<TYPE> *l )
      { return DivElements( l , r ); }

    /*
    static tfMatrix0<TYPE> *MulElementsClone( const tfMatrix0<TYPE> *l , const tFunction0<TYPE> *r )
      { return MulElementsIt( l->Clone() , r->Clone() ); }
    static tfMatrix0<TYPE> *DivElementsClone( const tfMatrix0<TYPE> *l , const tFunction0<TYPE> *r )
      { return DivElementsIt( l->Clone() , r->Clone() ); }
    static tfMatrix0<TYPE> *PowElementsClone( const tfMatrix0<TYPE> *l , const tFunction0<TYPE> *r )
      { return PowElementsIt( l->Clone() , r->Clone() ); }
    static tfMatrix0<TYPE> *MulElementsClone( const tFunction0<TYPE> *l , const tfMatrix0<TYPE> *r )
      { return MulElementsIt( l->Clone() , r->Clone() ); }
    static tfMatrix0<TYPE> *DivElementsClone( const tFunction0<TYPE> *l , const tfMatrix0<TYPE> *r )
      { return DivElementsIt( l->Clone() , r-Clone() ); }
    */

    static tfMatrix0<TYPE> *Concatenate( const tfMatrix0<TYPE> *l , const tfMatrix0<TYPE> *r );
    static tfMatrix0<TYPE> *Stack( const tfMatrix0<TYPE> *t , const tfMatrix0<TYPE> *b );
    
    /*
      static tfMatrix0<TYPE> *ConcatenateClone( const tfMatrix0<TYPE> *l , const tfMatrix0<TYPE> *r )
      { ConcatenateIt( l->Clone() , r->Clone() ); }
      static tfMatrix0<TYPE> *StackClone( const tfMatrix0<TYPE> *t , const tfMatrix0<TYPE> *b );
      { StackIt( l->Clone() , r->Clone() ); }
    */
    

    static tFunction0<TYPE> *Dot( const tfMatrix0<TYPE> *a , const tfMatrix0<TYPE> *b  );

    /*
    static tfMatrix0<TYPE> *DotClone( const tfMatrix0<TYPE> *a , const tfMatrix0<TYPE> *b  )
      { return DotIt( a->Clone() , b->Clone() ); }
    */
    static tfMatrix0<TYPE> *Crs( const tfMatrix0<TYPE> *a , const tfMatrix0<TYPE> *b  );
    static tfMatrix0<TYPE> *Crs( const tfMatrix0<TYPE> *a , const tfMatrix0<TYPE> *b  , const tfMatrix0<TYPE> *c  );
    static tfMatrix0<TYPE> *D( tfMatrix0<TYPE> *a , int argnr , int dim );
    static tfMatrix0<TYPE> *Transpose( const tfMatrix0<TYPE> *t  );


    static tfMatrix0<TYPE> *ColumnDerivatives( tfMatrix0<TYPE> *a , const tnmmatrix<TYPE> &deltas ,  
					       int columnnr = 1 , int dim = - 1);

    #ifndef MTH
    friend class tMatrixAlias<TYPE>;
    #endif
    //    friend void tMatrixAlias<TYPE>::Initialize( const tfMatrix0<TYPE> * );

  };



}


#endif
