/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#ifndef _P4TH_TNMCONTAINER_H_
#define _P4TH_TNMCONTAINER_H_

#include <vector>
#include <algorithm>
#include <string>
#include <debug.h>

//class istream;
//class ostream;

using namespace std;

namespace P4th
{

  template<class TYPE> 
    class tnmContainer 
 {
  protected:
    static string _FILETAG;
    int COLS;
    int ROWS;
    TYPE **elements;

    void Alloc();
    void Delete();

  public:
    tnmContainer()
    { 
      elements = 0L;
      ROWS = COLS = 0;
      Alloc();
    }

    tnmContainer( int R  , int C , const string &db = "" )
    { 
      elements = 0L;
      ROWS = R;
      COLS = C;
      Alloc(); 
    }

    tnmContainer( int R  , int C , const tnmContainer &source )
    { 
      elements = 0L;
      ROWS = R;
      COLS = C;
      if ( ROWS > source.ROWS || COLS > source.COLS )
	_DBTHROW( "tnmContainer( int R  , int C , const tnmContainer &source ) : ROWS > source.ROWS || COLS > source.COLS" );
      Alloc(); 
      for ( int i = 0 ; i < ROWS ; i++ )
	for ( int j = 0 ; j < COLS ; j++ )
	  elements[i][j] = source.elements[i][j];
    }

    tnmContainer( const tnmContainer &source )
    {
      //      Delete();
      elements = 0L;
      ROWS = source.ROWS;
      COLS = source.COLS;
      Alloc();
      Cpy( source );
    }
    tnmContainer( int R , int C , TYPE e );
    tnmContainer( int R , int C , TYPE **es );
    virtual ~tnmContainer();

    static tnmContainer<TYPE> RowVector( int rows , const TYPE *e , ... );
    static tnmContainer<TYPE> ColumnVector( int cols , const TYPE *e , ... );
    static tnmContainer<TYPE> RowVector( int rows , const TYPE &e , ... );
    static tnmContainer<TYPE> ColumnVector( int cols , const TYPE &e , ... );

    virtual void Resize( int R , int C )
    {
      Delete();
      ROWS = R;
      COLS = C;
      Alloc();
    }

    virtual tnmContainer<TYPE> &SafeCopy( const tnmContainer &b )
    {
      Resize( b.GetRows() , b.GetCols() );
      this->Cpy( b );
      return *this;
    }

    virtual tnmContainer &operator=( const tnmContainer &b )
      { return this->SafeCopy( b ); }
    virtual TYPE &operator()( int i , int j );

    virtual int GetRows() const
    { return ROWS; }
    virtual int GetCols() const
    { return COLS; }

    virtual TYPE Get( int i , int j ) const;
    virtual TYPE GetRepeated( int i , int j ) const
    { return Get( ((i - 1) % ROWS) + 1 , ((j - 1) % COLS) + 1 ); }
    virtual TYPE &Ref( int i , int j );
    virtual const TYPE *GetPtr( int i , int j ) const;

    virtual void Set( int i , int j , const TYPE &v );
    virtual TYPE *Opengl( TYPE *array ) const;

    virtual tnmContainer *Cpy( const TYPE &a );
    virtual tnmContainer *Cpy( const tnmContainer &B );
    virtual tnmContainer *Cpy( const tnmContainer *B )
    { return Cpy( *B ); }
    virtual tnmContainer *Cpy( const tnmContainer *B , 
			    int torow , int fromrow , int rows ,
			    int tocol , int fromcol , int cols );
    virtual tnmContainer *Trp( tnmContainer *res ) const;
    virtual tnmContainer Trp() const
    { tnmContainer<TYPE> res( GetCols() , GetRows() ); Trp( &res ); return res; }

    tnmContainer<TYPE> *SwapColumns( int c0 , int c1 );
    tnmContainer<TYPE> *SwapRows( int r0 , int r1 );
    virtual tnmContainer *SwapAll( tnmContainer *res) const;
    
    virtual tnmContainer *PivotRows( int _i , int _j ); 

    virtual int operator==( const tnmContainer &A ) const;
    virtual int operator!=( const tnmContainer &A ) const
    { return ! ((*this) == A);  }

    virtual operator TYPE();
    virtual operator bool();

    virtual tnmContainer<TYPE> ColVector() const;
    virtual tnmContainer<TYPE> RowVector() const;

    static tnmContainer *Get( tnmContainer *dest , const tnmContainer *source , int rownr , int colnr );
    static tnmContainer &Get( tnmContainer &dest , const tnmContainer &source , int rownr , int colnr )
    { return *Get( &dest , &source , rownr , colnr ); }


    static tnmContainer *_Get( tnmContainer *dest , const tnmContainer *source , int rownr , int colnr )
    { Get( dest , source , rownr , colnr ); }
    static tnmContainer &_Get( tnmContainer &dest , const tnmContainer &source , int rownr , int colnr )
    { return *Get( &dest , &source , rownr , colnr ); }
  
    static tnmContainer *Set( tnmContainer *dest , const tnmContainer *source , int rownr , int colnr );
    static tnmContainer &Set( tnmContainer &dest , const tnmContainer &source , int rownr , int colnr )
    { return *Set( &dest , &source , rownr , colnr ); }
    static tnmContainer *_Set( tnmContainer *dest , const tnmContainer *source , int rownr , int colnr )
    { return Set( dest , source , rownr , colnr ); }
    static tnmContainer &_Set( tnmContainer &dest , const tnmContainer &source , int rownr , int colnr )
    { return *Set( &dest , &source , rownr , colnr ); }

    /*
    static tnmContainer ColumnVector( const TYPE &x1 )
    { return tnmContainer(1,1,x1); }

    static tnmContainer<TYPE> ColumnVector( const TYPE &x1 , const TYPE &x2 )
    { tnmContainer x(2,1); x(1,1) = x1; x(2,1) = x2; return x; }

    tnmContainer ColumnVector( const TYPE &x1 , const TYPE &x2 , const TYPE &x3 )
    { tnmContainer x(3,1); x(1,1) = x1; x(2,1) = x2;  x(3,1) = x3; return x; }
    
    static tnmContainer ColumnVector( const TYPE &x1 , const TYPE &x2 , const TYPE &x3 , const TYPE &x4 )
    { tnmContainer x(4,1); x(1,1) = x1; x(2,1) = x2;  x(3,1) = x3; x(4,1) = x4; return x; }
    */

    virtual std::istream &Read( std::istream &source );
    virtual std::ostream &Write( std::ostream &dest , string lm = "" ) const;
    virtual std::ostream &Print( std::ostream &dest , string lm = "" , int width = 80 ) const;
    virtual std::ostream &PrintTrp( std::ostream &dest , string lm = "" , int width = 80 ) const;
    virtual std::ostream &AsXML( std::ostream &dest , string lm = "" ) const;
    
 };

  /*
    This causes bug in:
    gcc version 3.3.5-20050130 (Gentoo Linux 3.3.5.20050130-r1, ssp-3.3.5.20050130-1, pie-8.7.7.1)
    
    template<class TYPE>
    tnmContainer<TYPE> ColumnVector( const TYPE &x1 )
    { return tnmContainer<TYPE>(1,1,x1); }
    
    template<class TYPE>
    tnmContainer<TYPE> ColumnVector( const TYPE &x1 , const TYPE &x2 )
    { tnmContainer<TYPE> x(2,1); x(1,1) = x1; x(2,1) = x2; return x; }
    
    template<class TYPE>
    tnmContainer<TYPE> ColumnVector( const TYPE &x1 , const TYPE &x2 , const TYPE &x3 )
    { tnmContainer<TYPE> x(3,1); x(1,1) = x1; x(2,1) = x2;  x(3,1) = x3; return x; }
    
    template<class TYPE>
    tnmContainer<TYPE> ColumnVector( const TYPE &x1 , const TYPE &x2 , const TYPE &x3 , const TYPE &x4 )
    { tnmContainer<TYPE> x(4,1); x(1,1) = x1; x(2,1) = x2;  x(3,1) = x3; x(4,1) = x4; return x; }
    
  */

}



#endif
