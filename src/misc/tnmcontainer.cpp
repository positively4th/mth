/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#include "tnmcontainer.h"
#include <iostream>
#include <iomanip>
#include <stdarg.h>
#include "debug.h"
#include <stlstringtools.h>
#include "texception.h"

namespace P4th {

  template<class TYPE>
  string tnmContainer<TYPE>::_FILETAG = "_TNMCONTAINER";
  
  template<class TYPE>
  void tnmContainer<TYPE>::Alloc()
  {
    if ( elements != 0L )
      _DBTHROW( "void tnmContainer<TYPE>::Alloc() : elements != 0L" );

    elements = new TYPE*[ROWS];
    for (int i = 0 ; i < ROWS ; i++ )
      elements[i] = new TYPE[COLS];
  }
  
  template<class TYPE>
  void tnmContainer<TYPE>::Delete()
  {
    if ( elements != 0L )
      {
	for ( int i = 0 ; i < ROWS ; i++ )
	  if ( elements[i] != 0L )
	    delete [] elements[i];
	delete [] elements;
	elements = 0L;
      }
  }

  template<class TYPE>
  tnmContainer<TYPE>::tnmContainer( int R , int C , TYPE e )
  {
    elements = 0L;
    ROWS = R;
    COLS = C;
    Alloc();
    for ( int  i = 0 ; i < ROWS ; i++ ) 
      for ( int  j = 0 ; j < COLS ; j++ )
	elements[i][j] = e;
  }

  template<class TYPE>
  tnmContainer<TYPE>::tnmContainer( int R , int C , TYPE **es )
  {
    elements = 0L;
    ROWS = R;
    COLS = C;
    Alloc();
    for ( int  i = 0 ; i < ROWS ; i++ ) 
      for ( int  j = 0 ; j < COLS ; j++ )
	elements[i][j] = es[i][j];
  }

  template<class TYPE>
  tnmContainer<TYPE>::~tnmContainer()
  {
    Delete();
  }




  template<class TYPE>
  tnmContainer<TYPE> tnmContainer<TYPE>::ColumnVector( int dim , const TYPE *e , ... )
  {

    tnmContainer<TYPE> res( dim , 1 , (TYPE)0.0 );

    va_list arglist;
    va_start(arglist,e);
    
    res(1,1) = *e;
    
    TYPE *tmp;
    for ( int i = 2 ; i <= dim ; i++ )
      {
	tmp = va_arg(arglist, TYPE * );
	if ( tmp == 0L )
	  break;

	res(i,1) = *tmp;
	
      }
    va_end( arglist );
    return res;
  }

  template<class TYPE>
  tnmContainer<TYPE> tnmContainer<TYPE>::RowVector( int dim , const TYPE *e , ... )
  {
    tnmContainer<TYPE> res( 1 , dim , (TYPE)0.0 );
    
    va_list arglist;
    va_start(arglist,e);
    
    res(1,1) = *e;
    TYPE *tmp;
    
    for ( int i = 2 ; i <= dim ; i++ )
      {
	tmp = va_arg(arglist, TYPE * );
	if ( tmp == 0L )
	  break;

	res(1,i) = *tmp;
	
      }
    va_end( arglist );
    return res;
    
  }


  template<class TYPE>
  tnmContainer<TYPE> tnmContainer<TYPE>::ColumnVector( int dim , const TYPE &e , ... )
  {

    tnmContainer<TYPE> res( dim , 1 , (TYPE)0.0 );

    va_list arglist;
    va_start(arglist,e);
    
    res(1,1) = e;
    
    TYPE tmp;
    for ( int i = 2 ; i <= dim ; i++ )
      {
	tmp = va_arg(arglist, TYPE  );

	res(i,1) = tmp;
	
      }
    va_end( arglist );
    return res;
  }

  template<class TYPE>
  tnmContainer<TYPE> tnmContainer<TYPE>::RowVector( int dim , const TYPE &e , ... )
  {
    tnmContainer<TYPE> res( 1 , dim , (TYPE)0.0 );
    
    va_list arglist;
    va_start(arglist,e);
    
    res(1,1) = e;
    
    TYPE tmp;
    for ( int i = 2 ; i <= dim ; i++ )
      {
	tmp = va_arg(arglist, TYPE  );

	res(1,i) = tmp;
	
      }
    va_end( arglist );
    return res;
    
  }

  template<class TYPE>
  tnmContainer<TYPE> *tnmContainer<TYPE>::Cpy( const tnmContainer *B , 
					 int torow , int fromrow , int rows ,
					 int tocol , int fromcol , int cols )
  {
#ifdef _SAFE
    if ( fromrow < 1 || fromcol < 1 )
      _DBTHROW( "tnmContainer<TYPE>::Cpy( ... ) : fromrow < 1 || fromcol < 1" );
    if ( torow < 1 || tocol < 1 )
      _DBTHROW( "tnmContainer<TYPE>::Cpy( ... ) : torow < 1 || tocol < 1" );
    if ( fromrow + rows - 1 > B->ROWS || fromcol + cols - 1 > B->COLS )
      _DBTHROW( "tnmContainer<TYPE>::Cpy( ... ) : fromrow + rows - 1 > B->ROWS || fromcol + cols - 1 > B->COLS" );
    if ( torow + rows - 1 > ROWS || tocol + cols - 1 > COLS )
      _DBTHROW( "tnmContainer<TYPE>::Cpy( ... ) : torow + rows - 1 > ROWS || tocol + cols - 1 > COLS" );

#endif
    for ( int i = 0 ; i < rows ; i++ )
      for ( int j = 0 ; j < cols  ; j++ )
	(*this)( torow + i , tocol + j ) = B->Get( fromrow + i , fromcol + j );
    return this;
  }

  template<class TYPE>
  int tnmContainer<TYPE>::operator==( const tnmContainer &A ) const
  {
    for ( int i = 0 ; i < this->ROWS ; i++ ) 
      for ( int j = 0 ; j < this->COLS ; j++ ) 
	if ( this->elements[i][j] != A.elements[i][j] )
	  return 0;
    return 1;
  }

  template<class TYPE>
  inline tnmContainer<TYPE>::operator TYPE()
  {
    if ( this->ROWS != 1 && this->COLS != 1 )
      _DBTHROW( "operator TYPE() : this->ROWS != 1 && this->COLS != 1" );
    return this->elements[0][0];
  }
  
  template<class TYPE>
  inline tnmContainer<TYPE>::operator bool()
  {
    if ( this->ROWS < 1 || this->COLS < 1 )
      return false;
    for ( int i = 0 ; i < this->ROWS ; i++ )
      for ( int j = 0 ; j < this->COLS ; j++ )
	if ( this->elements[i][j] != (TYPE)0.0 )
	  return true;
    return false;
  }


  template<class TYPE>
  inline TYPE &tnmContainer<TYPE>::operator()( int i , int j )
  {
#ifdef _SAFE
    if ( i < 1 || i > ROWS || j < 1 || j > COLS  )
      _DBTHROW( (string)"tnmContainer::operator[]: index error" );
#endif
    return elements[i-1][j-1];
  }

  template<class TYPE>
  inline TYPE tnmContainer<TYPE>::Get( int i , int j ) const
  {
#ifdef _SAFE
    if ( i < 1 || i > ROWS || j < 1 || j > COLS  )
      _DBTHROW( (string)__PRETTY_FUNCTION__ + ": index error" );
#endif
    return elements[i-1][j-1];
  }

  template<class TYPE>
  inline TYPE &tnmContainer<TYPE>::Ref( int i , int j )
  {
#ifdef _SAFE
    if ( i < 1 || i > ROWS || j < 1 || j > COLS  )
      _DBTHROW( (string)"tnmContainer::operator[]: index error" );
#endif
    return elements[i-1][j-1];
  }

  template<class TYPE>
  inline const TYPE *tnmContainer<TYPE>::GetPtr( int i , int j ) const
  {
#ifdef _SAFE
    if ( i < 1 || i > ROWS || j < 1 || j > COLS  )
      _DBTHROW( (string)"tnmContainer::operator[]: index error" );
#endif
    return &elements[i-1][j-1];
  }

  template<class TYPE>
  inline void tnmContainer<TYPE>::Set( int i , int j , const TYPE &v ) 
  {
#ifdef _SAFE
    if ( i < 1 || i > ROWS || j < 1 || j > COLS  )
      _DBTHROW( (string)"Set - index error" );
#endif
    elements[i-1][j-1] = v;
  }

  template<class TYPE>
  TYPE *tnmContainer<TYPE>::Opengl( TYPE *array ) const
  {
    int k = 0;
    for ( int j = 0 ; j < COLS ; j++ )
      for ( int i = 0 ; i < ROWS ; i++ )
	{
	  array[k++] = elements[i][j];
	}
    return array; 
  }

  template<class TYPE>
  tnmContainer<TYPE> *tnmContainer<TYPE>::Cpy( const TYPE &a )
  {
    for ( int  i = 0 ; i < ROWS ; i++ ) 
      for ( int  j = 0 ; j < COLS ; j++ )
	elements[i][j] = a;
    return this;
  }

  template<class TYPE>
  tnmContainer<TYPE> *tnmContainer<TYPE>::Cpy( const tnmContainer<TYPE> &B )
  {
#ifdef _SAFE
    if ( ROWS != B.ROWS || COLS != B.COLS )
      _DBTHROW((string)"tnmContainer<TYPE> *tnmContainer<TYPE>::Cpy( const tnmContainer &B  ) : ROWS != B.ROWS || COLS != B.COLS" );
#endif 
    for ( int  i = 0 ; i < ROWS ; i++ ) 
      for ( int  j = 0 ; j < COLS ; j++ )
	elements[i][j] = B.elements[i][j];
    return this;
  }

  template<class TYPE>
  tnmContainer<TYPE> *tnmContainer<TYPE>::Trp( tnmContainer<TYPE> *res ) const
  {
#ifdef _SAFE
    if ( COLS != res->ROWS || ROWS != res->COLS )
      _DBTHROW( (string)"tnmContainer<TYPE> *tnmContainer<TYPE>::Trp( tnmContainer<TYPE> *res ) const : COLS != res->ROWS || ROWS != res->COLS" );
#endif
    for ( int i = 1 ; i <= ROWS ; i++ )
      for ( int j = 1 ; j <= COLS ; j++ )
	(*res)( j , i ) = this->Get( i , j );
    return res;
  }


  template<class TYPE>
  tnmContainer<TYPE> *tnmContainer<TYPE>::SwapAll( tnmContainer *res) const
  {
    //    _DB( std::cout << "tnmContainer<TYPE> *tnmContainer<TYPE>::SwapAll( tnmContainer *res) const" << std::endl; );
#ifdef _SAFE
    if ( COLS != res->COLS || ROWS != res->ROWS )
      _DBTHROW( "tnmContainer *tnmContainer<TYPE>::SwapAll( tnmContainer *res) const : COLS != res->COLS || ROWS != res->ROWS" );
#endif
    for ( int i = 0; i < ROWS ; i ++ )
      for ( int j = 0; j < COLS ; j ++ )
	res->elements[ROWS-i-1][COLS-j-1] = elements[i][j];
    return res;
  };

  template<class TYPE>
  tnmContainer<TYPE> *tnmContainer<TYPE>::SwapColumns( int c0 , int c1 )
  {
    //    _DB( std::cout << "tnmContainer<TYPE> *tnmContainer<TYPE>::SwapColumns( int c0 , int c1 )" << std::endl; );
#ifdef _SAFE
    if ( c0 < 1  || c0 > COLS  )
      _DBTHROW( "tnmContainer<TYPE> *tnmContainer<TYPE>::SwapColumns( int c0 , int c1 ) : c0 < 1  || c0 > COLS" );
    if ( c1 < 1  || c1 > COLS  )
      _DBTHROW( "tnmContainer<TYPE> *tnmContainer<TYPE>::SwapColumns( int c0 , int c1 ) : c1 < 1  || c1 > COLS" );
#endif
    TYPE tmp;
    c0--;
    c1--;
    for ( int i = 0; i < ROWS ; i ++ )
      { 
	tmp = elements[i][c0];
	elements[i][c0] = elements[i][c1];
	elements[i][c1] = tmp;
      }
    return this;
  }

  template<class TYPE>
  tnmContainer<TYPE> *tnmContainer<TYPE>::SwapRows( int r0 , int r1 )
  {
    //    _DB( std::cout << "tnmContainer<TYPE> *tnmContainer<TYPE>::SwapRows( int r0 , int r1 )" << std::endl; );
#ifdef _SAFE
    if ( r0 < 1  || r0 > ROWS  )
      _DBTHROW( "tnmContainer<TYPE> *tnmContainer<TYPE>::SwapRows( int r0 , int r1 ) : r0 < 1  || r0 > ROWS" );
    if ( r1 < 1  || r1 > ROWS  )
      _DBTHROW( "tnmContainer<TYPE> *tnmContainer<TYPE>::SwapRows( int r0 , int r1 ) : r1 < 1  || r1 > ROWS" );
#endif
    TYPE tmp;
    r0--;
    r1--;
    for ( int i = 0; i < COLS ; i ++ )
      { 
	tmp = elements[r0][i];
	elements[r0][i] = elements[r1][i];
	elements[r1][i] = tmp;
      }
    return this;
  }

  template<class TYPE>
  tnmContainer<TYPE> *tnmContainer<TYPE>::PivotRows( int _i , int _j  )
  {
    _i--;
    _j--;
    TYPE aij = elements[_i][_j];
    if ( aij == 0 )
      throw (string)"tnmContainer<TTYPE,ROWS,COLS> *tnmContainer<TYPE>::PivotRow( int _i , int _j  ) : aij == 0";
    
    if ( aij != 1.0 )
      for ( int j = 0 ; j < COLS ; j++ )
	elements[_i][j] /= aij;

    TYPE mul;
    for ( int i = 0 ; i < ROWS ; i++ )
      {
	if ( _i != i )
	  {
	    mul = elements[i][_j];
	    for ( int j = 0 ; j < COLS ; j++ )
	      elements[i][j] -= mul * elements[_i][j];
	  }
      }
    return this;
  }

  template<class TYPE>
  tnmContainer<TYPE> tnmContainer<TYPE>::RowVector() const
  {
#ifdef _SAFE
    if ( ROWS != 1 )
      throw (string)"tnmContainer<TYPE> tnmContainer<TYPE>::RowVector() const : ROWS != 1"; 
    if ( COLS < 1 )
      throw (string)"tnmContainer<TYPE> tnmContainer<TYPE>::RowVector() const : COLS < 1"; 
#endif
    tnmContainer<TYPE> res(1,COLS);
    for ( int i = 1 ; i <= COLS ; i++ )
      res(1,i) = elements[0][i-1];
    return res;
  }

  template<class TYPE>
  tnmContainer<TYPE> tnmContainer<TYPE>::ColVector() const
  {
#ifdef _SAFE
    if ( COLS != 1 )
      throw (string)"tvector<TYPE,ROWS> tnmContainer<TYPE>::ColVector() const : COLS != 1"; 
    if ( ROWS < 1 )
      throw (string)"tvector<TYPE,ROWS> tnmContainer<TYPE>::ColVector() const : ROWS < 1"; 
#endif
    tnmContainer<TYPE> res(ROWS,1);
    for ( int i = 1 ; i <= ROWS ; i++ )
      res(i,1) = elements[i-1][0];
    return res;
  }
  

  template<class TYPE>
  std::istream & tnmContainer<TYPE>::Read( std::istream &source )
  {
    //  _DBOUT( "istream & tnmContainer<TYPE>::Read( istream &source )" << std::endl );
    string tag;
    tag = "";
    int rows,cols;
    while ( tag != _FILETAG )
      operator>>( source , tag );
    /*
    source >> tag;
    rows = StringTools<string>::String2Int( tag );
    */
    source >> rows;
    if ( rows != ROWS )
      _DBTHROW( (string)"operator>>: rows error" );
    /*
    source >> tag;
    cols = StringTools<string>::String2Int( tag );
    */
    source >> cols;
    if ( cols != COLS )
      _DBTHROW( (string)"operator>>: size error" );
    for ( int i = 0 ; i < ROWS ; i++ )
      for ( int j = 0 ; j < COLS ; j++ )
	{
	  //  source >> elements[i][j];
	  source >> elements[i][j];
	}
    return source;
  }

  template<class TYPE>
  std::ostream & tnmContainer<TYPE>::Write( std::ostream &dest , string lm ) const
  {
    //  _DBOUT( "ostream & tnmContainer<TYPE,COLS,ROWS>::Write( ostream &dest , string lm ) const" << "\n" );
    dest << lm << _FILETAG << "\n";
    dest << lm << "\t" << ROWS << "\n";
    dest << lm << "\t" << COLS << "\n";
    for ( int i = 0 ; i < ROWS ; i++ )
      {
	dest << lm;
	for ( int j = 0 ; j < COLS ; j++ )
	  {
	    //      dest << "\t" << elements[i][j];
	    dest << "\t";
	    dest << elements[i][j];
	  }
	dest << "\n";
      }
    return dest;
  }

  template<class TYPE>
  std::ostream &tnmContainer<TYPE>::Print( std::ostream &dest , string lm , int width ) const
  {
    int cs = COLS;
    if ( cs == 0 )
      cs = 1;
    width = ( width - 4 ) / cs; 

    dest.setf( std::ios::left );
    for ( int i = 1 ; i <= ROWS ; i++ )
      {
	dest << lm;
	for ( int j = 1 ; j <= COLS ; j++ )
	  {
	    dest << std::setw(width) << (*this).Get(i,j); 
	  }
	dest << std::endl;
      }
    return dest;
  }

  template<class TYPE>
  std::ostream &tnmContainer<TYPE>::PrintTrp( std::ostream &dest , string lm , int width ) const
  {
    tnmContainer<TYPE> trp( COLS , ROWS );
    this->Trp( &trp  );
    return trp.Print( dest , lm , width );
  }

  template<class TYPE>
  std::ostream &tnmContainer<TYPE>::AsXML( std::ostream &dest , string lm  ) const
  {
    dest << lm << "<tnmContainer>" << std::endl;
    dest << lm << "\t<file>" << __FILE__ << "</file>" << std::endl;
    dest << lm << "\t<function>" << __PRETTY_FUNCTION__ << "</function>" << std::endl;
    dest << lm << "\t<line>" << __LINE__ << "</line>" << std::endl;
    dest << lm << "\t<rows>" << GetRows() << "</rows>" << std::endl;
    dest << lm << "\t<columns>" << GetCols() << "</columns>" << std::endl;
    dest << lm << "\t<rows>" << std::endl;

    for ( int i = 1 ; i <= GetRows() ; i++ )
      {
	dest << lm << "\t\t<row>";
	for ( int j = 1 ; j <= GetCols() ; j++ )
	  dest << "<cell>" << this->Get(i,j) << "</cell>";
	dest << "</row>" << std::endl;
      }
    dest << lm << "\t</rows>" << std::endl;
    
    dest << lm << "</tnmContainer>" << std::endl;
    
    return dest;
  }

  template<class TYPE>
  tnmContainer<TYPE> *tnmContainer<TYPE>::Get( tnmContainer<TYPE> *dest , const tnmContainer<TYPE> *source , int rownr , int colnr )
  {
#ifdef _SAFE
    if ( colnr < 1 || colnr + dest->COLS - 1 > source->COLS ) 
      _DBTHROW( (string)"tnmContainer<TTYPE,DROWS,DCOLS> *Get( tnmContainer<TTYPE,DROWS,DCOLS> *dest , const  tnmContainer<TTYPE,SROWS,SCOLS> *source , int rownr , int colnr ) : colnr < 1 || colnr + DCOLS - 1 > SCOLS" );
    if ( rownr < 1 || rownr + dest->ROWS - 1 > source->ROWS ) 
      _DBTHROW( (string)"tnmContainer<TTYPE,DROWS,DCOLS> *Get( tnmContainer<TTYPE,DROWS,DCOLS> *dest , const  tnmContainer<TTYPE,SROWS,SCOLS> *source , int rownr , int colnr ) : rownr < 1 || rownr + DROWS - 1 > SROWS" );
#endif
    for ( int i = 1 ; i <= dest->ROWS ; i++ )
      for ( int j = 1 ; j <= dest->COLS ; j++ )
	(*dest)( i , j ) = source->Get( i + rownr - 1 , j + colnr - 1 ) ; 
    return dest;
  }

  template<class TYPE>
  tnmContainer<TYPE> *tnmContainer<TYPE>::Set( tnmContainer<TYPE> *dest , const  tnmContainer<TYPE> *source , int rownr , int colnr )
  {
#ifdef _SAFE
    if ( colnr < 1 || colnr + source->COLS - 1 > dest->COLS ) 
      _DBTHROW( (string)"tnmContainer<TTYPE,DROWS,DCOLS> *Set( tnmContainer<TTYPE,DROWS,DCOLS> *dest , const  tnmContainer<TTYPE,SROWS,SCOLS> *source , int rownr , int colnr ) : colnr < 1 || colnr + SCOLS - 1 > DCOLS" );
    if ( rownr < 1 || rownr + source->ROWS - 1 > dest->ROWS ) 
      _DBTHROW( (string)"tnmContainer<TTYPE,DROWS,DCOLS> *Set( tnmContainer<TTYPE,DROWS,DCOLS> *dest , const  tnmContainer<TTYPE,SROWS,SCOLS> *source , int rownr , int colnr ) : rownr < 1 || rownr + SROWS - 1 > DROWS" );
#endif
    for ( int i = 1 ; i <= source->ROWS ; i++ )
      for ( int j = 1 ; j <= source->COLS ; j++ )
	{
	  (*dest)( i + rownr - 1 , j + colnr - 1 ) = source->Get( i , j ); 
	}
    return dest;
  }

template class P4th::tnmContainer<int>;
template class P4th::tnmContainer<unsigned int>;
template class P4th::tnmContainer<float>;
template class P4th::tnmContainer<double>;


}
