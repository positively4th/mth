/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#include "tfmatrix.h"

#include <tconstant.h>
#include <tadd.h>
#include <tsub.h>
#include <tmul.h>
#include <tdiv.h>
#include <tpow.h>

#include <tabs.h>
#include <tsqrt.h>


#include <iostream>
#include <iomanip>
#include "debug.h"
#include <stlstringtools.h>
#include <algorithm>
#include "tmathtools.h"

#include <stdarg.h>

#ifdef _OCTAVE
#include <octave/config.h>
#include <octave/Matrix.h>
#endif

namespace P4th
{

  template<class TYPE>
  string tfMatrix<TYPE>::_FILETAG = "_TFMATRIX";
  
  template<class TYPE>
  int tfMatrix<TYPE>::Nodes() const
  {
    int res = 0;
    for ( int  i = 0 ; i < GetRows() ; i++ ) 
      for ( int  j = 0 ; j < GetCols() ; j++ )
	{
	  if ( EP(i,j) != 0L )
	    res += EP( i , j )->Nodes();
	}    
    return res;
  }

  template<class TYPE>
  void tfMatrix<TYPE>::Alloc()
  {
    //    _DB( std::cout << "Allocating [" << GetRows() << "," << GetCols() << "] matrix" << std::endl; );
    if ( elements != 0L )
      _DBTHROW( "void tfMatrix<TYPE>::Alloc() : elements != 0L" );
    
   
    elements = new tFunction0<TYPE> **[GetRows()];
    if ( elements == 0L )
      {
	_DBTHROW( (string)"Error allocating [" + STLStringTools::AsString( GetRows() ) 
		  + "," + STLStringTools::AsString( GetCols() ) + "] matrix." );
      }
    for (int i = 0 ; i < GetRows() ; i++ )
      {
	elements[i] = new tFunction0<TYPE> *[GetCols()];
	if ( elements[i] == 0L )
	  _DBTHROW( (string)"Error allocating row with " + STLStringTools::AsString( GetCols() ) + "columns." );
	
	for (int j = 0 ; j < GetCols() ; j++ )
	  elements[i][j] = 0L;
      }
  }
  
  template<class TYPE>
  void tfMatrix<TYPE>::Delete()
  {
    if ( elements != 0L )
      {
	for ( int i = 0 ; i < GetRows() ; i++ )
	  if ( elements[i] != 0L )
	    {
	      for (int j = 0 ; j < GetCols() ; j++ )
		if ( elements[i][j] != 0L )
		  delete elements[i][j];
	      delete [] elements[i];
	    }
	delete [] elements;
	elements = 0L;
      }
  }

  template<class TYPE>
  void tfMatrix<TYPE>::Resize( int R , int C )
  {
    Delete();
    ROWS = R;
    COLS = C;
    Alloc();
  }
  
  template<class TYPE>
  tfMatrix<TYPE>::tfMatrix( int R  , int C  , tFunction0<TYPE> *f0 )
  {
    elements = 0L;
    ROWS = R;
    COLS = C;
    Alloc();

    tFunction0<TYPE> *last = 0L;
    for ( int  i = 0 ; i < GetRows() ; i++ ) 
      for ( int  j = 0 ; j < GetCols() ; j++ )
	if ( f0 == 0L )
	  Assign( EPA(i,j) ,  new tConstant<TYPE>( 0 ) );
	else
	  {
	    if ( last == 0L )
	      {
		Assign( EPA(i,j) , f0 );
		last = f0;
	      } else
	      Assign( EPA(i,j) , f0->Clone() );
	  }
  }
  
  template<class TYPE>
  tfMatrix<TYPE>::tfMatrix( tFunction0<TYPE> *f0 )
  {
    elements = 0L;
    ROWS = 1;
    COLS = 1;
    Alloc();
    for ( int  i = 0 ; i < GetRows() ; i++ ) 
      for ( int  j = 0 ; j < GetCols() ; j++ )
	if ( f0 == 0L )
	  Assign( EPA(i,j) ,  new tConstant<TYPE>( 0 ) );
	else
	  Assign( EPA(i,j) , f0 );
  }
  
  template<class TYPE>
  tfMatrix<TYPE>::tfMatrix( int R  , int C  , TYPE c  )
  {
    elements = 0L;
    ROWS = R;
    COLS = C;
    Alloc();
    for ( int  i = 0 ; i < GetRows() ; i++ ) 
      for ( int  j = 0 ; j < GetCols() ; j++ )
	Assign( EPA(i,j)  , new tConstant<TYPE>( c ) );
  }
  
  template<class TYPE>
  tfMatrix<TYPE>::tfMatrix( TYPE c  )
  {
    elements = 0L;
    ROWS = 1;
    COLS = 1;
    Alloc();
    for ( int  i = 0 ; i < GetRows() ; i++ ) 
      for ( int  j = 0 ; j < GetCols() ; j++ )
	Assign( EPA(i,j) , new tConstant<TYPE>( c ) );
  }

  template<class TYPE>
  tfMatrix<TYPE>::tfMatrix( const tnmmatrix<TYPE> &m  )
  {
    elements = 0L;
    ROWS = m.GetRows();
    COLS = m.GetCols();
    Alloc();
    for ( int  i = 0 ; i < GetRows() ; i++ ) 
      for ( int  j = 0 ; j < GetCols() ; j++ )
	Assign( EPA(i,j) , new tConstant<TYPE>( m.Get( i + 1 , j + 1 ) ) );
  }  

  template<class TYPE>
  tfMatrix<TYPE>::tfMatrix( const tfMatrix<TYPE> &source )
  {
    elements = 0L;
    ROWS = source.GetRows();
    COLS = source.GetCols();
    Alloc();
    for ( int  i = 0 ; i < GetRows() ; i++ ) 
      for ( int  j = 0 ; j < GetCols() ; j++ )
	Assign( EPA(i,j) , source.EP(i,j)->Clone() );
  }
  
  template<class TYPE>
  tfMatrix0<TYPE> *tfMatrix<TYPE>::Cpy( const tfMatrix &source )
  {
    if ( GetRows() != source.GetRows() || GetCols() != source.GetCols() )
      {
	Delete();
	ROWS = source.GetRows();
	COLS = source.GetCols();
	Alloc();
      }
    for ( int  i = 0 ; i < GetRows() ; i++ ) 
      for ( int  j = 0 ; j < GetCols() ; j++ )
	Assign( EPA(i,j) , source.EP(i,j)->Clone() );
    return this;
  }

  template<class TYPE>
  tfMatrix0<TYPE> *tfMatrix<TYPE>::Cpy( const TYPE &a )
  {
    for ( int  i = 0 ; i < GetRows() ; i++ ) 
      for ( int  j = 0 ; j < GetCols() ; j++ )
	Assign( EPA(i,j) , new tConstant<TYPE>( a ) );
    return this;
  }

  template<class TYPE>
  tfMatrix0<TYPE> *tfMatrix<TYPE>::Cpy( const tFunction0<TYPE> *f )
  {
    for ( int  i = 0 ; i < GetRows() ; i++ ) 
      for ( int  j = 0 ; j < GetCols() ; j++ )
	Assign( EPA(i,j) , f->Clone() );
    return this;
  }
  
  template<class TYPE>
  tfMatrix<TYPE>::~tfMatrix()
  {
#ifdef _SAFE
    if ( this == 0L )
      _DBTHROW( "this == 0L" );
#endif
    this->Delete();
  }

  template<class TYPE>
  bool tfMatrix<TYPE>::DimOk( int d ) const
  {
    if ( elements[0][0] == 0L )
      return true;
    
    for ( int  i = 0 ; i < GetRows() ; i++ ) 
      for ( int  j = 0 ; j < GetCols() ; j++ )
	{
	  if ( EP(i,j) != 0L )
	    if ( ! EP(i,j)->DimOk( d )  )
	      return false;
	}    
    return true;
  }

  template<class TYPE>
  tfMatrix<TYPE> *tfMatrix<TYPE>::ColumnVector( int dim , tFunction0<TYPE> *e , ... )
  {

    tfMatrix<TYPE> *res = new tfMatrix<TYPE>( dim , 1 , (tFunction0<TYPE> *)NULL );
    
    
    va_list arglist;
    va_start(arglist,e);
    
    res->Assign( &res->elements[0][0], e );
    
    tFunction0<TYPE> *tmp;
    int i;
    for ( i = 2 ; i <= dim ; i++ )
      {
	tmp = va_arg(arglist, tFunction0<TYPE> * );
	if ( tmp == 0L )
	  break;
	
	res->Assign( &res->elements[i-1][0], tmp );
	
      }
    va_end( arglist );
    if ( i <= dim )
      throw tException<tfMatrix<TYPE> >("Incorrect number of elements");
    
    return res;
  }

  template<class TYPE>
  tfMatrix<TYPE> *tfMatrix<TYPE>::RowVector( int dim , tFunction0<TYPE> *e , ... )
  {

    tfMatrix<TYPE> *res = new tfMatrix<TYPE>( 1 , dim , (tFunction0<TYPE> *)NULL );
    
    va_list arglist;
    va_start(arglist,e);
    
    res->Assign( &res->elements[0][0], e );
    
    tFunction0<TYPE> *tmp;
    int i;
    for ( i = 2 ; i <= dim ; i++ )
      {
	tmp = va_arg(arglist, tFunction0<TYPE> * );
	if ( tmp == 0L )
	  break;
	
	res->Assign( &res->elements[0][i-1], tmp );
	
      }
    va_end( arglist );
    if ( i <= dim )
      throw tException<tfMatrix<TYPE> >("Incorrect number of elements"  );
    
    return res;
  }


  template<class TYPE>
  inline tFunction0<TYPE> *tfMatrix<TYPE>::Cpy( int i , int j ) const
  {
    if ( i < 1 || i > GetRows() || j < 1 || j > GetCols()  )
      _DBTHROW( (string)"tfMatrix::Clone( i , j ) : index error" );
    if ( elements[i-1][j-1] != 0L )
      return elements[i-1][j-1]->Clone();
    _DBTHROW( (string)"Element (" 
	      + STLStringTools::AsString( i ) + "," 
	      + STLStringTools::AsString( j ) + ") not initialized." );
  }

  template<class TYPE>
  bool tfMatrix<TYPE>::InitDerivatives( bool analytical , int depth , int dim  )
  {
    for ( int  i = 0 ; i < GetRows() ; i++ ) 
      for ( int  j = 0 ; j < GetCols() ; j++ )
	{
	  //	  _DB( std::cout << "(" << i << "," << j << "): elelment = " << EP( i,j) << std::endl; );
	  if ( EP(i,j) == 0L )
	    return false;
	  if ( ! (*EPA(i,j))->InitDerivatives( analytical , depth , dim ) )
	    return false;
	}    
    return true;
  }

  /*
  template<class TYPE>
  void tfMatrix<TYPE>::AnalyticalDerivatives()
  {
    for ( int  i = 0 ; i < GetRows() ; i++ ) 
      for ( int  j = 0 ; j < GetCols() ; j++ )
	{
	  if ( EP(i,j) != 0L )
	    (*EPA(i,j))->AnalyticalDerivatives();
	}    
  }

  template<class TYPE>
  void tfMatrix<TYPE>::NumericalDerivatives()
  {
    for ( int  i = 0 ; i < GetRows() ; i++ ) 
      for ( int  j = 0 ; j < GetCols() ; j++ )
	{
	  if ( EP(i,j) != 0L )
	    (*EPA(i,j))->NumericalDerivatives();
	}    
  }
  */


  template<class TYPE>
  inline tFunction0<TYPE> *tfMatrix<TYPE>::Ref( int i , int j )
  {
#ifdef _SAFE
    if ( i < 1 || i > GetRows() || j < 1 || j > GetCols()  )
      _DBTHROW( (string)"tfMatrix::operator(): index error" );
    if ( elements[i-1][j-1] == 0L )
      _DBTHROW( "Matrix element not initialized!" );
#endif
    return elements[i-1][j-1];
  }

  template<class TYPE>
  inline const tFunction0<TYPE> *tfMatrix<TYPE>::operator()( int i , int j ) const
  {
#ifdef _SAFE
    if ( i < 1 || i > GetRows() || j < 1 || j > GetCols()  )
      _DBTHROW( (string)"tfMatrix::operator(): index error" );
    if ( elements[i-1][j-1] == 0L )
      _DBTHROW( "Matrix element not initialized!" );
#endif
    return elements[i-1][j-1];
  }

  template<class TYPE>
  tFunction0<TYPE> *tfMatrix<TYPE>::Set( int i , int j , tFunction0<TYPE> *f )
  {
    if ( i < 1 || i > GetRows() || j < 1 || j > GetCols()  )
      _DBTHROW( (string)"tfMatrix::Set( ... ): index error" );
    if ( f != 0L )
      Assign( &elements[i-1][j-1], f );
    else
      Assign( &elements[i-1][j-1], 0L );
  }

  template<class TYPE>
  tFunction0<TYPE> *tfMatrix<TYPE>::Steal( int i , int j  )
  {
    if ( i < 1 || i > GetRows() || j < 1 || j > GetCols()  )
      _DBTHROW( (string)"tfMatrix::Set( ... ): index error" );

    tFunction0<TYPE> *res = elements[i-1][j-1];
    elements[i-1][j-1] = 0L;
    return res;
  }

  template<class TYPE>
  tfMatrix0<TYPE> *tfMatrix<TYPE>::operator=( const tnmmatrix<TYPE> &b  )
  {
    if ( GetRows() != b.GetRows() || GetCols() != b.GetCols()  )
      {
	Delete();
	COLS = b.GetCols();
	ROWS = b.GetRows();
	Alloc();
      }
    
    for ( int  i = 0 ; i < GetRows() ; i++ ) 
      for ( int  j = 0 ; j < GetCols() ; j++ )
	{
	  Assign( EPA(i,j) , new tConstant<TYPE>( b.Get(i+1,j+1) ) ); 
	}    
    return this;
  }

  /*
  template<class TYPE>
  void tfMatrix<TYPE>::Dim( int _dim )
  {
    for ( int  i = 0 ; i < GetRows() ; i++ ) 
      for ( int  j = 0 ; j < GetCols() ; j++ )
	{
#ifdef _SAFE
	  if ( EP(i,j) == 0L )
	    _DBTHROW( (string)"Matrix element not initialized." );
#endif
	  (*EPA(i,j))->Dim( _dim );
	  
	}    
  }
  */

  template<class TYPE>
  tfMatrix0<TYPE> *tfMatrix<TYPE>::operator=( tFunction0<TYPE> *f )
  {
    if ( GetRows() != 1 || GetCols() != 1  )
      {
	Delete();
	COLS = 1;
	ROWS = 1;
	Alloc();
      }
    Assign( &elements[0][0], f->Clone() );
    return this;
  }

  template<class TYPE>
  tfMatrix0<TYPE> *tfMatrix<TYPE>::operator=( const TYPE &number )
  {
    if ( GetRows() != 1 || GetCols() != 1  )
      {
	Delete();
	COLS = 1;
	ROWS = 1;
	Alloc();
      }
    Assign( EPA(0,0), new tConstant<TYPE>( number ) );
    return this;
  }

  template<class TYPE>
  tfMatrix0<TYPE> *tfMatrix<TYPE>::I()
  {
    if ( GetRows() != GetCols() )
      _DBTHROW( (string)"GetRows() != GetCols()" );
    for ( int i = 0 ; i < GetRows() ; i++ )
      for ( int j = 0 ; j < GetCols() ; j++ )
	if ( i == j )
	  (*EPA(i,j)) = new tConstant<TYPE>( 1.0 );
	else
	  (*EPA(i,j)) = new tConstant<TYPE>( 0.0 );
    return this;
  }


  template<class TYPE>
  tfMatrix0<TYPE> *tfMatrix<TYPE>::I( int rows )
  {
    tfMatrix<TYPE> *res = new tfMatrix<TYPE>( rows , rows );
    for ( int i = 0 ; i < res->GetRows() ; i++ )
      for ( int j = 0 ; j < res->GetCols() ; j++ )
	if ( i == j )
	  Assign( res->EPA(i,j) , new tConstant<TYPE>( 1.0 ) );
	else
	  Assign( res->EPA(i,j) , new tConstant<TYPE>( 0.0 ) );
    return res;
  }

  template<class TYPE>
  tfMatrix0<TYPE> *tfMatrix<TYPE>::Trp( tfMatrix0<TYPE> *res ) const
  {
    if ( GetCols() != res->GetRows() || GetRows() != res->GetCols() )
      _DBTHROW( "GetCols() != res.GetRows() || GetRows() != res.GetCols()" );
    for ( int i = 1 ; i <= GetRows() ; i++ )
      for ( int j = 1 ; j <= GetCols() ; j++ )
	Assign( res->EPA(i,j) , EP(i,j)->Clone() );
    return res;
  }

  template<class TYPE>
  tfMatrix0<TYPE> *tfMatrix<TYPE>::Abs( tfMatrix0<TYPE> *res ) const
  {
    if ( GetCols() != res->GetRows() || GetRows() != res->GetCols() )
      _DBTHROW( "GetCols() != res.GetRows() || GetRows() != res.GetCols()" );
    for ( int i = 1 ; i <= GetRows() ; i++ )
      for ( int j = 1 ; j <= GetCols() ; j++ )
	Assign( res->EPA(i,j) , new tAbs<TYPE>( EP(i,j)->Clone() ) );
    return res;
  }

  template<class TYPE>
  tfMatrix0<TYPE> *tfMatrix<TYPE>::Add( tfMatrix0<TYPE> *res , const tfMatrix0<TYPE> *B )
  {
    if ( GetRows() != B->GetRows() || GetCols() != B->GetCols() )
      _DBTHROW( "Matrix dimensions differ!" );
    if ( GetRows() != res->GetRows() || GetCols() != res->GetCols() )
      _DBTHROW( "Matrix dimensions differ!" );

    for ( int  i = 0 ; i < GetRows() ; i++ ) 
      for ( int  j = 0 ; j < GetCols() ; j++ )
	Assign( res->EPA(i,j) , tFunction0<TYPE>::Add( EP(i,j)->Clone() , B->EP(i,j)->Clone() ) );
    return res;
  }

  template<class TYPE>
  tfMatrix0<TYPE> *tfMatrix<TYPE>::Sub( tfMatrix0<TYPE> *res , const tfMatrix0<TYPE> *B )
  {
    if ( GetRows() != B->GetRows() || GetCols() != B->GetCols() )
      _DBTHROW( "Matrix dimensions differ!" );
    if ( GetRows() != res->GetRows() || GetCols() != res->GetCols() )
      _DBTHROW( "Matrix dimensions differ!" );

    for ( int  i = 0 ; i < GetRows() ; i++ ) 
      for ( int  j = 0 ; j < GetCols() ; j++ )
	Assign( res->EPA(i,j) , tFunction0<TYPE>::Sub( EP(i,j)->Clone() , B->EP(i,j)->Clone() ) );
    return res;
  }

  template<class TYPE>
  tfMatrix0<TYPE> *tfMatrix<TYPE>::Mul( tfMatrix0<TYPE> *res , const tfMatrix0<TYPE> *B )
  {
    //    _DB( std::cout << "ddd" << std::endl; );
    if ( GetCols() != B->GetRows()  )
      _DBTHROW( "Matrix dimensions incorrect!" );
    if ( GetRows() != res->GetRows() || B->GetCols() != res->GetCols() )
      _DBTHROW( "Matrix dimensions incorrect!" );

    Heap<tFunction0<TYPE> > h;
    for ( int i = 0 ; i < GetRows()  ; i++ )
      for ( int j = 0 ; j < B->GetCols()  ; j++ )
	{
	  tAdd<TYPE> *adds = new tAdd<TYPE>;
	  Assign( res->EPA(i,j) , adds );
	  for ( int k = 0 ; k < GetCols() ; k++ )
	    adds->AddTerm( tFunction0<TYPE>::Mul( EP(i,k)->Clone() , B->EP(k,j)->Clone() ) ); 
	}

    return res;
  }

  template<class TYPE>
  int tfMatrix<TYPE>::ForEvery( const tFunction0<TYPE> *(*func)( const tFunction0<TYPE> * , int col , int row , Polymorphic *) , Polymorphic *something ) const
  {
    //    _DB( std::cout << "int tfMatrix<TYPE>::ForEvery( tFunction0<TYPE> *(*func)( tFunction0<TYPE> * , int col , int row , void *) , void *something ) const" << std::endl; );

    int ctr;
    const tFunction0<TYPE> *ptr;
    for ( int i = 1 ; i <= GetRows()  ; i++ )
      for ( int j = 1 ; j <= GetCols()  ; j++ )
	{
	  //	  ptr = (*func)( this->Cpy(i,j) , i , j , something );
	  ptr = (*func)( (*this)( i , j ) , i , j , something );
	  if ( ptr != 0L )
	    delete ptr;
	  ctr++;
	}
    //    _DB( std::cout << "int tfMatrix<TYPE>::ForEvery( tFunction0<TYPE> *(*func)( tFunction0<TYPE> * , int col , int row , void *) , void *something ) const ---- Done!" << std::endl; );

    return ctr;
  }




  template<class TYPE>
  tfMatrix0<TYPE> *tfMatrix<TYPE>::Cofactor( int i , int j ) const
  {
    tfMatrix<TYPE> *res = new tfMatrix<TYPE>( GetRows() - 1  , GetCols() - 1  );
    int ii = 0;
    int jj = 0;
    for ( int  j = 0 ; j < GetCols() ; j++ )
      {
	for ( int  i = 0 ; i < GetRows() ; i++ ) 
	  if ( ii != i || jj != j )
	    tfMatrix<TYPE>::Assign( &res->elements[ii++][j++] , 
				    EP(i,j)->Clone() );
      }
    return res;
  }
  
  
  template<class TYPE>
  tfMatrix0<TYPE> *tfMatrix<TYPE>::Column( int col ) const
  {
    if ( col < 1 || col > GetCols() )
      _DBTHROW( "Column does not exist." );

    col--;
    tfMatrix<TYPE> *res = new tfMatrix<TYPE>( GetRows()  , 1  );
    for ( int  i = 0 ; i < GetRows() ; i++ ) 
      tfMatrix<TYPE>::Assign( &res->elements[i][0] , 
			      elements[i][col]->Clone() );
    return res;

  }
  
  template<class TYPE>
  tfMatrix0<TYPE> *tfMatrix<TYPE>::Row( int row ) const
  {
    if ( row < 1 || row > GetRows() )
      _DBTHROW( "Row does not exist." );

    row--;
    tfMatrix<TYPE> *res = new tfMatrix<TYPE>( 1  , GetCols()  );
    for ( int  j = 0 ; j < GetCols() ; j++ ) 
      tfMatrix<TYPE>::Assign( &res->elements[0][j] , 
			      elements[row][j]->Clone() );
    return res;

  }

  template<class TYPE>
  tfMatrix0<TYPE> *tfMatrix<TYPE>::Negative() const
  {
    tfMatrix<TYPE> *res = new tfMatrix<TYPE>( GetRows()  , GetCols()  );
    for ( int  i = 0 ; i < GetRows() ; i++ ) 
      for ( int  j = 0 ; j < GetCols() ; j++ ) 
	{
	  if ( EP(i,j) != 0L )
	    {
	      Assign( res->EPA(i,j) , 
		      tFunction0<TYPE>::Negate( EP(i,j)->Clone() ) );
	    } else
	    _DBTHROW( (string)"Matrix element not initialized." ); 
	}
    return res;
  }

  template<class TYPE>
  tnmmatrix<TYPE> tfMatrix<TYPE>::y( const tnmmatrix<TYPE> &x ) const
  {
    //    _DB( std::cout << "tnmmatrix<TYPE> &tfMatrix<TYPE>::y( tnmmatrix<TYPE> &y , const tnmmatrix<TYPE> &x ) const" << std::endl );
    TYPE tmp;

    tnmmatrix<TYPE> y( GetRows() , GetCols() ); 

    for ( int  i = 0 ; i < GetRows() ; i++ ) 
      for ( int  j = 0 ; j < GetCols() ; j++ ) 
	{
#ifdef _SAFE
	  //	  _DB( std::cout << "Evaluating function (" << i << "," << j << ") ("  << EP(i,j) << ")" << std::endl; );
	  //	  _DB( if ( EP( i , j ) != 0L ) EP( i , j )->Dump( std::cout , "f: \t" ); );
	  if ( EP(i,j) == 0L )
	    _DBTHROW( (string)"Matrix element not initialized." ); 
#endif
	  y.Set( i + 1 , j + 1 , EP(i,j)->y( x ) );
	}
    return y;

  }
  
  template<class TYPE>
  tfMatrix0<TYPE> *tfMatrix<TYPE>::Reduce( int argnr , tFunction0<TYPE> *s ) const
  {
    //    _DB( std::cout << "tfMatrix0<TYPE> *tfMatrix<TYPE>::Reduce( int argnr , tFunction0<TYPE> *s ) const" << std::endl; );
    tfMatrix<TYPE> *res = new tfMatrix<TYPE>( GetRows()  , GetCols()  );
    if ( GetRows() == 0 || GetCols() == 0 )
      return res;
    for ( int  i = 0 ; i < GetRows() ; i++ ) 
      for ( int  j = 0 ; j < GetCols() ; j++ ) 
	{
	  //	  _DB( std::cout << "(i,j)=" << i  << "," << j << ")" << std::endl; ); 
	  if ( i != 1 || j != 1 )
	    if ( EP(i,j) != 0L )
	      {
		Assign( res->EPA(i,j) , EP(i,j)->Reduce( argnr , s->Clone() ) );
	      } else
	      _DBTHROW( (string)"Matrix element not initialized." ); 
	}
    if ( EP(0,0) != 0L )
      Assign( res->EPA(0,0) ,  EP(0,0)->Reduce( argnr , s ) );
    else
      _DBTHROW( (string)"Matrix element not initialized." );
    return res;
  }

  template<class TYPE>
  std::ostream &tfMatrix<TYPE>::Dump( std::ostream &dest , string lm ) const
  {
    for ( int  j = 0 ; j < GetCols() ; j++ )
      for ( int  i = 0 ; i < GetRows() ; i++ ) 
	{
	  dest << lm << "(" << i << "," << j << ")" << std::endl;
	  if ( EP(i,j) != 0L )
	    EP(i,j)->Dump( dest , lm + "\t" );
	  else
	    dest << lm << "\t" << (void *)0L << std::endl;
	}

    return dest;
    
  } 


  template<class TYPE>
  tfMatrix0<TYPE> *tfMatrix<TYPE>::Add( const tfMatrix0<TYPE> *l , const tfMatrix0<TYPE> *r )
  {
    if ( r->GetRows() != l->GetRows() || r->GetCols() != l->GetCols() )
      _DBTHROW( "Matrix dimensions differ!" );

    tfMatrix0<TYPE> *res = new tfMatrix<TYPE>( l->GetRows() , l->GetCols() );
    for ( int  i = 0 ; i < l->GetRows() ; i++ ) 
      for ( int  j = 0 ; j < l->GetCols() ; j++ )
	tfMatrix<TYPE>::Assign( res->EPA(i,j) , 
				tFunction0<TYPE>::Add( l->EP(i,j)->Clone() , r->EP(i,j)->Clone() ) );
    return res;
  }

  template<class TYPE>
  tfMatrix0<TYPE> *tfMatrix<TYPE>::Sub( const tfMatrix0<TYPE> *l , const tfMatrix0<TYPE> *r )
  {
    if ( r->GetRows() != l->GetRows() || r->GetCols() != l->GetCols() )
      _DBTHROW( "Matrix dimensions differ!" );

    tfMatrix0<TYPE> *res = new tfMatrix<TYPE>( l->GetRows() , l->GetCols() );
    for ( int  i = 0 ; i < l->GetRows() ; i++ ) 
      for ( int  j = 0 ; j < l->GetCols() ; j++ )
	tfMatrix<TYPE>::Assign( res->EPA(i,j) , 
				tFunction0<TYPE>::Sub( l->EP(i,j)->Clone() , r->EP(i,j)->Clone() ) );
    return res;
  }

  template<class TYPE>
  tfMatrix0<TYPE> *tfMatrix<TYPE>::Mul( const tfMatrix0<TYPE> *l , const tfMatrix0<TYPE> *r )
  {

    if ( l->GetCols() != r->GetRows()  )
      _DBTHROW( "Matrix dimensions incorrect!" );


    tfMatrix0<TYPE> *res = new tfMatrix<TYPE>( l->GetRows() , r->GetCols() );

    Heap<tFunction0<TYPE> > h;
    for ( int i = 0 ; i < l->GetRows()  ; i++ )
      for ( int j = 0 ; j < r->GetCols()  ; j++ )
	{
	  tAdd<TYPE> *adds = new tAdd<TYPE>;
	  tfMatrix<TYPE>::Assign( res->EPA(i,j) , adds );
	  for ( int k = 0 ; k < l->GetCols() ; k++ )
	    adds->AddTerm( tFunction0<TYPE>::Mul( l->EP(i,k)->Clone() , r->EP(k,j)->Clone() ) ); 
	}
    return res;
  }

  template<class TYPE>
  tfMatrix0<TYPE> *tfMatrix<TYPE>::DivElements( const tfMatrix0<TYPE> *l , const tFunction0<TYPE> *r )
  {

    tfMatrix0<TYPE> *res = new tfMatrix( l->GetRows() , l->GetCols() );
    for ( int  i = 0 ; i < l->GetRows() ; i++ ) 
      for ( int  j = 0 ; j < l->GetCols() ; j++ )
	tfMatrix<TYPE>::Assign( res->EPA(i,j) , 
				tFunction0<TYPE>::Div( l->EP(i,j)->Clone() , r->Clone() ) );
    return res;
  }
  
  template<class TYPE>
  tfMatrix0<TYPE> *tfMatrix<TYPE>::MulElements( const tfMatrix0<TYPE> *l , const tFunction0<TYPE> *r )
  {
    Heap<tFunction0<TYPE> > h;
    //    _DB( std::cout << "l = " << (void *)l << std::endl; );
    //    _DB( l->Dump( std::cout ); ); 
    tfMatrix0<TYPE> *res = new tfMatrix( l->GetRows() , l->GetCols() );
    for ( int  i = 0 ; i < l->GetRows() ; i++ ) 
      for ( int  j = 0 ; j < l->GetCols() ; j++ )
	tfMatrix<TYPE>::Assign( res->EPA(i,j) , 
				tFunction0<TYPE>::Mul( l->EP(i,j)->Clone() , r->Clone() ) );
    return res;
  }

  template<class TYPE>
  tfMatrix0<TYPE> *tfMatrix<TYPE>::PowElements( const tfMatrix0<TYPE> *l , const tFunction0<TYPE> *r )
  {

    tfMatrix0<TYPE> *res = new tfMatrix( l->GetRows() , l->GetCols() );
    for ( int  i = 0 ; i < l->GetRows() ; i++ ) 
      for ( int  j = 0 ; j < l->GetCols() ; j++ )
	tfMatrix<TYPE>::Assign( res->EPA(i,j) , 
				tFunction0<TYPE>::Pow( l->EP(i,j)->Clone() , r->Clone() ) );
    return res;
  }

  template<class TYPE>
  tfMatrix0<TYPE> *tfMatrix<TYPE>::Concatenate( const tfMatrix0<TYPE> *l , const tfMatrix0<TYPE> *r )
  {
    if ( r->GetRows() != l->GetRows() )
      _DBTHROW( "Matrices' rows differ!" );

    tfMatrix<TYPE> *res = new tfMatrix( l->GetRows() , l->GetCols() + r->GetCols() );
    for ( int  i = 0 ; i < l->GetRows() ; i++ ) 
      {
	for ( int  j = 0 ; j < l->GetCols() ; j++ )
	  tfMatrix<TYPE>::Assign( res->EPA(i,j) , 
				  l->EP(i,j)->Clone() );
	for ( int  j = 0 ; j < r->GetCols() ; j++ )
	  tfMatrix<TYPE>::Assign( &res->elements[i][j+l->GetCols()] , 
				  r->EP(i,j)->Clone() );
      }
    return res;
  }

  template<class TYPE>
  tfMatrix0<TYPE> *tfMatrix<TYPE>::Stack( const tfMatrix0<TYPE> *t , const tfMatrix0<TYPE> *b )
  {
    if ( t->GetCols() != t->GetCols() )
      _DBTHROW( "Matrices' cols differ!" );

    tfMatrix<TYPE> *res = new tfMatrix( t->GetRows() + b->GetRows() , t->GetCols()  );
    for ( int  j = 0 ; j < t->GetCols() ; j++ )
      {
	for ( int  i = 0 ; i < t->GetRows() ; i++ ) 
	  tfMatrix<TYPE>::Assign( res->EPA(i,j) , 
				  t->EP(i,j)->Clone() );
	for ( int  i = 0 ; i < b->GetRows() ; i++ )
	  tfMatrix<TYPE>::Assign( &res->elements[i+t->GetCols()][j] , 
				  b->EP(i,j)->Clone() );
      }
    return res;
  }

  template<class TYPE>
  tfMatrix0<TYPE> *tfMatrix<TYPE>::Transpose( const tfMatrix0<TYPE> *t  )
  {
    tfMatrix<TYPE> *res = new tfMatrix( t->GetCols() , t->GetRows()  );
    for ( int  i = 0 ; i < t->GetRows() ; i++ ) 
      for ( int  j = 0 ; j < t->GetCols() ; j++ )
	tfMatrix<TYPE>::Assign( &res->elements[j][i] , 
				t->EP(i,j)->Clone() );
    return res;
  }

  template<class TYPE>
  tfMatrix0<TYPE> *tfMatrix<TYPE>::Simplify()
  {
    for ( int  i = 0 ; i < this->GetRows() ; i++ ) 
      for ( int  j = 0 ; j < this->GetCols() ; j++ )
	this->elements[i][j] = this->elements[i][j]->Simplify();
    return this;
  }

  template<class TYPE>
  tFunction0<TYPE> *tfMatrix<TYPE>::Dot( const tfMatrix0<TYPE> *a , const tfMatrix0<TYPE> *b )
  {
#ifdef _SAFE
    if ( a->GetCols() != 1 || b->GetCols() != 1)
      _DBTHROW( "TYPE Dot( const tfMatrix0<TYPE> *a , const tfMatrix0<TYPE> *b ) : a->GetCols() != 1 || b->GetCols() != 1" );
    if ( a->GetRows() != b->GetRows() )
      _DBTHROW( "TYPE Dot( const tfMatrix0<TYPE> *u , const tfMatrix0<TYPE> *v ) : GetRows() != 1 v->GetRows()" );
#endif
    
    tAdd<TYPE> *tmp = new tAdd<TYPE>;
    for ( int i = 1 ; i <= a->GetRows() ; i++ ) 
      {
	tmp->AddTerm( tFunction0<TYPE>::Mul( (*a)(i,1)->Clone() , (*b)(i,1)->Clone() ) );  
      }
	
    return tmp;
  }

  template<class TYPE>
  tfMatrix0<TYPE> *tfMatrix<TYPE>::Crs( const tfMatrix0<TYPE> *a , const tfMatrix0<TYPE> *b  )
  {
#ifdef _SAFE
    if ( a->GetCols() != 1 || b->GetCols() != 1 )
      _DBTHROW( "tfMatrix0<TYPE> *Crs( tfMatrix0<TYPE> *res , const tfMatrix0<TYPE> *a , const tfMatrix0<TYPE> *b  ) : a->GetCols() != 1 || b->GetCols() != 1" );
    if ( a->GetRows() != 3 || b->GetRows() != 3 )
      _DBTHROW( "tfMatrix0<TYPE> *Crs( tfMatrix0<TYPE> *res , const tfMatrix0<TYPE> *a , const tfMatrix0<TYPE> *b , const tfMatrix0<TYPE> *c ) : a->GetRows() != 3 || b->GetRows() != 3" );
#endif

    tfMatrix<TYPE> *res = new  tfMatrix<TYPE>( 3 , 1 );

    Heap<tFunction0<TYPE> > h;

    
    Assign( res->EPA(0,0) ,
	    tFunction0<TYPE>::Sub( 
				    tFunction0<TYPE>::Mul( (*a)(2,1)->Clone() , (*b)(3,1)->Clone() ) , 
				    tFunction0<TYPE>::Mul( (*a)(3,1)->Clone() , (*b)(2,1)->Clone() ) ) 
	    );
    
    Assign( res->EPA(1,0) ,
	    tFunction0<TYPE>::Sub( 
				 tFunction0<TYPE>::Mul( (*a)(3,1)->Clone() , (*b)(1,1)->Clone() ) , 
				 tFunction0<TYPE>::Mul( (*a)(1,1)->Clone() , (*b)(3,1)->Clone() ) ) 
	    );

    (*res)(2,1)->Dump( std::cout , "Crs:\t " );
    Assign( res->EPA(2,0) ,
	    tFunction0<TYPE>::Sub( 
				 h += tFunction0<TYPE>::Mul( (*a)(1,1)->Clone() , (*b)(2,1)->Clone() ) , 
				 h += tFunction0<TYPE>::Mul( (*a)(2,1)->Clone() , (*b)(1,1)->Clone() ) ) 
	    );
	    
    return res;
  }

  template<class TYPE>
  tfMatrix0<TYPE> *tfMatrix<TYPE>::Crs( const tfMatrix0<TYPE> *a , const tfMatrix0<TYPE> *b , const tfMatrix0<TYPE> *c )
  {
#ifdef _SAFE
    if ( a->GetCols() != 1 || b->GetCols() != 1 || c->GetCols() != 1 )
      _DBTHROW( "tfMatrix0<TYPE> *Crs( tfMatrix0<TYPE> *res , const tfMatrix0<TYPE> *a , const tfMatrix0<TYPE> *b , const tfMatrix0<TYPE> *c ) : a->GetCols() != 1 || b->GetCols() != 1 || c->GetCols() != 1" );
    if ( a->GetRows() != 4 || b->GetRows() != 4 || b->GetRows() != 4 )
      _DBTHROW( "tfMatrix0<TYPE> *Crs( tfMatrix0<TYPE> *res , const tfMatrix0<TYPE> *a , const tfMatrix0<TYPE> *b , const tfMatrix0<TYPE> *c ) : a->GetRows() != b->GetRows() || a->GetRows() != b->GetRows()" );
#endif
    
    tfMatrix<TYPE> *res = new  tfMatrix<TYPE>( 4 , 1 );

    //    Heap<tFunction0<TYPE> > h;

    /*
    h += tFunction0<TYPE>::Mul( (*a)(1,1) , (*b)(1,1) , (*c)(1,1) , (*a)(2,1) , (*b)(2,1) , (*c)(2,1) , 0L );
    h += tFunction0<TYPE>::Mul( (*a)(1,1) , (*b)(1,1) , (*c)(1,1) , (*a)(2,1) , (*b)(2,1) , (*c)(2,1) , 0L );
    h += tFunction0<TYPE>::Add( (*a)(1,1) , (*b)(1,1) , (*c)(1,1) , (*a)(2,1) , (*b)(2,1) , (*c)(2,1) , 0L );
    h += tFunction0<TYPE>::Mul( (*a)(1,1) , (*b)(1,1) , (*c)(1,1) , (*a)(2,1) , (*b)(2,1) , (*c)(2,1) , 0L );
    */

 
    //    (*this)(1,1) = 
    //      + (*a).Get(2,1) * ( (*b).Get(3,1) * (*c).Get(4,1) - (*b).Get(4,1) * (*c).Get(3,1) )
    //      + (*a).Get(3,1) * ( (*b).Get(4,1) * (*c).Get(2,1) - (*b).Get(2,1) * (*c).Get(4,1) )
    //      - (*a).Get(4,1) * ( (*b).Get(2,1) * (*c).Get(3,1) - (*b).Get(3,1) * (*c).Get(2,1) );
    Assign( res->EPA(0,0) , 
	    tFunction0<TYPE>::Add( 
				    tFunction0<TYPE>::Mul( 
							    (*a)(2,1)->Clone() ,
							    tFunction0<TYPE>::Sub( 
										    tFunction0<TYPE>::Mul( (*b)(3,1)->Clone() ,
													     (*c)(4,1)->Clone() ) ,
										    tFunction0<TYPE>::Mul( (*b)(4,1)->Clone() , 
													     (*c)(3,1)->Clone() ) ) 
							    ) , 
				    tFunction0<TYPE>::Mul( 
							    (*a)(3,1)->Clone() ,
							    tFunction0<TYPE>::Sub( 
										    tFunction0<TYPE>::Mul( (*b)(4,1)->Clone() , 
													     (*c)(2,1)->Clone() ) ,
										    tFunction0<TYPE>::Mul( (*b)(2,1)->Clone() , 
													     (*c)(4,1)->Clone() ) ) 
						      ) , 
				    tFunction0<TYPE>::Mul( 
							    new tConstant<TYPE>( -1 ) , 
							    (*a)(4,1)->Clone() , 
							  tFunction0<TYPE>::Sub( 
										  tFunction0<TYPE>::Mul( (*b)(2,1)->Clone() , 
													   (*c)(3,1)->Clone() ) ,
										  tFunction0<TYPE>::Mul( (*b)(3,1)->Clone() , 
													   (*c)(2,1)->Clone() ) ) , 
							  0L ) ,
				    0L )
	    );
	    
    
    
    
    //    (*this)(2,1) = 
    //      + (*a).Get(1,1) * ( (*b).Get(4,1) * (*c).Get(3,1) - (*b).Get(3,1) * (*c).Get(4,1) );
    //      + (*a).Get(3,1) * ( (*b).Get(1,1) * (*c).Get(4,1) - (*b).Get(4,1) * (*c).Get(1,1) )
    //      - (*a).Get(4,1) * ( (*b).Get(3,1) * (*c).Get(1,1) - (*b).Get(1,1) * (*c).Get(3,1) )
    Assign( res->EPA(1,0) , 
	    tFunction0<TYPE>::Add( 
				    tFunction0<TYPE>::Mul( 
							    (*a)(1,1)->Clone() ,
							    tFunction0<TYPE>::Sub( 
										    tFunction0<TYPE>::Mul( (*b)(4,1)->Clone() , 
													     (*c)(3,1)->Clone() ) ,
										    tFunction0<TYPE>::Mul( (*b)(3,1)->Clone() , 
													     (*c)(4,1)->Clone() ) ) 
							    ) , 
				    tFunction0<TYPE>::Mul( 
							    (*a)(3,1)->Clone() ,
							    tFunction0<TYPE>::Sub( 
										    tFunction0<TYPE>::Mul( (*b)(1,1)->Clone() , 
													     (*c)(4,1)->Clone() ) ,
										    tFunction0<TYPE>::Mul( (*b)(4,1)->Clone() , 
													     (*c)(1,1)->Clone() ) ) 
							    ) , 
				    tFunction0<TYPE>::Mul( 
							    new tConstant<TYPE>( -1 ) , 
							    (*a)(4,1)->Clone() , 
							    tFunction0<TYPE>::Sub( 
										    tFunction0<TYPE>::Mul( (*b)(3,1)->Clone() , 
													     (*c)(1,1)->Clone() ) ,
										    tFunction0<TYPE>::Mul( (*b)(1,1)->Clone() , 
													     (*c)(3,1)->Clone() ) ) , 
							 0L ) )
	    );
    
    
    //    (*this)(3,1) = 
    //      - (*a).Get(4,1) * ( (*b).Get(1,1) * (*c).Get(2,1) - (*b).Get(2,1) * (*c).Get(1,1) )
    //      + (*a).Get(2,1) * ( (*b).Get(4,1) * (*c).Get(1,1) - (*b).Get(1,1) * (*c).Get(4,1) )
    //      + (*a).Get(1,1) * ( (*b).Get(2,1) * (*c).Get(4,1) - (*b).Get(4,1) * (*c).Get(2,1) );
    Assign( res->EPA(2,0) , 
	    tFunction0<TYPE>::Add( 
				 tFunction0<TYPE>::Mul( 
							 new tConstant<TYPE>( -1 ) , 
							 (*a)(4,1)->Clone() ,
							 tFunction0<TYPE>::Sub( 
										 tFunction0<TYPE>::Mul( (*b)(1,1)->Clone() , 
													  (*c)(2,1)->Clone() ) ,
										 tFunction0<TYPE>::Mul( (*b)(2,1)->Clone() , 
													  (*c)(1,1)->Clone() ) ) , 
							 0L ) , 
				 tFunction0<TYPE>::Mul( 
							 (*a)(2,1)->Clone() ,
							 tFunction0<TYPE>::Sub( 
										 tFunction0<TYPE>::Mul( (*b)(4,1)->Clone() , 
													  (*c)(1,1)->Clone() ) ,
										 tFunction0<TYPE>::Mul( (*b)(1,1)->Clone() , 
													  (*c)(4,1)->Clone() ) ) 
							 ) , 
				 tFunction0<TYPE>::Mul( 
							 (*a)(1,1)->Clone() , 
							 tFunction0<TYPE>::Sub( 
										 tFunction0<TYPE>::Mul( (*b)(2,1)->Clone() , 
													  (*c)(4,1)->Clone() ) ,
										 tFunction0<TYPE>::Mul( (*b)(4,1)->Clone() , 
													  (*c)(2,1)->Clone() ) ) 
							 ) )
	    );
    //    (*this)(4,1) = 
    //      - (*a).Get(3,1) * ( (*b).Get(2,1) * (*c).Get(1,1) - (*b).Get(1,1) * (*c).Get(2,1) )
    //      + (*a).Get(2,1) * ( (*b).Get(1,1) * (*c).Get(3,1) - (*b).Get(3,1) * (*c).Get(1,1) )
    //      + (*a).Get(1,1) * ( (*b).Get(3,1) * (*c).Get(2,1) - (*b).Get(2,1) * (*c).Get(3,1) );
    Assign( res->EPA(3,0) , 
	    tFunction0<TYPE>::Add( 
				    tFunction0<TYPE>::Mul( 
							    new tConstant<TYPE>(  -1 ) , 
							    (*a)(3,1)->Clone() ,
							    tFunction0<TYPE>::Sub( 
										    tFunction0<TYPE>::Mul( (*b)(2,1)->Clone() , 
													     (*c)(1,1)->Clone() ) ,
										    tFunction0<TYPE>::Mul( (*b)(1,1)->Clone() , 
													     (*c)(2,1)->Clone() ) ) ,
							  0L ) , 
				    tFunction0<TYPE>::Mul( 
							  (*a)(2,1)->Clone() ,
							  tFunction0<TYPE>::Sub( 
										  tFunction0<TYPE>::Mul( (*b)(1,1)->Clone() , 
													   (*c)(3,1)->Clone() ) ,
										  tFunction0<TYPE>::Mul( (*b)(3,1)->Clone() , 
													   (*c)(1,1)->Clone() ) ) 
							  ) , 
				    tFunction0<TYPE>::Mul( 
							  (*a)(1,1)->Clone() , 
							  tFunction0<TYPE>::Sub( 
										  tFunction0<TYPE>::Mul( (*b)(3,1)->Clone() , 
													   (*c)(2,1)->Clone() ) ,
										  tFunction0<TYPE>::Mul( (*b)(2,1)->Clone() , 
													   (*c)(3,1)->Clone() ) ) 
							  ) )
	    );
    return res;
  }

  template<class TYPE>
  tfMatrix0<TYPE> *tfMatrix<TYPE>::D( tfMatrix0<TYPE> *a , int argnr  , int dim )
  {
    
#ifdef _SAFE
    if ( ! a->DimOk( argnr ) )
      _DBTHROW( "Function dimension less then argument nr." );
#endif
    
    tfMatrix<TYPE> *res = new  tfMatrix<TYPE>( a->GetRows() , a->GetCols() );
    
    for ( int  i = 1 ; i <= a->GetRows() ; i++ ) 
      for ( int  j = 1 ; j <= a->GetCols() ; j++ )
	{	
	  tfMatrix<TYPE>::Assign( &res->elements[i-1][j-1] , 
				  a->Ref( i , j )->Getdydx( argnr )->Clone() );

	}
    return res;
  }


  template<class TYPE>
  tfMatrix0<TYPE> *tfMatrix<TYPE>::ColumnDerivatives( tfMatrix0<TYPE> *a , const tnmmatrix<TYPE> &deltas , 
						      int columnnr , int dim )
  {
    if ( dim < 0 )
      dim = a->GetRows();
#ifdef _SAFE
    if ( ! a->DimOk( columnnr ) )
      _DBTHROW( "Function dimension less then argument nr." );
#endif
    
    tfMatrix<TYPE> *res = new  tfMatrix<TYPE>( a->GetRows() , dim );
    
    for ( int  r = 1 ; r <= a->GetRows() ; r++ ) 
      for ( int c = 1 ; c <= dim ; c++ ) {	
	tfMatrix<TYPE>::Assign( &res->elements[r-1][c-1] , 
				a->Ref( r , columnnr )->Getdydx( deltas.GetRepeated( r , c ) , 
								 c , 0L )->Clone() );
	
      }
    return res;
  }

}

template class P4th::tfMatrix<double>;
template class P4th::tfMatrix<float>;


