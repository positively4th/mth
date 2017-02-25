#include "tparameters.h"

#include "tfmatrix.h"
#include "tparameter.h"

#include <debug.h>
#include <math.h>
#include <stlstringtools.h>

#include "tconstant.h"


namespace P4th
{

  template<class TYPE> 
  tParameters<TYPE>::tParameters( const TYPE *_v , int R , int C  )
  {
    this->elements = 0L;
    this->ROWS = R;
    this->COLS = C;
    this->Alloc();
    for ( int  i = 0 ; i < this->GetRows() ; i++ ) 
      for ( int  j = 0 ; j < this->GetCols() ; j++ )
	this->Assign( this->EPA(i,j)  , new tParameter<TYPE>( _v ) );
    
  }

  template<class TYPE> 
  tParameters<TYPE>::tParameters( const tnmmatrix<TYPE> *_m ) 
  {
    this->elements = 0L;
    this->ROWS = _m->GetRows();
    this->COLS = _m->GetCols();
    this->Alloc();

    const TYPE *tmp;
    tParameter<TYPE> *tmpp;
    for ( int  i = 0 ; i < this->GetRows() ; i++ ) 
      for ( int  j = 0 ; j < this->GetCols() ; j++ )
	{
	  tmp =  _m->GetPtr( i + 1 , j + 1 );
	  tmpp = new tParameter<TYPE>( tmp );
	  this->Assign( this->EPA(i,j)  ,  tmpp );
	}
  }
  
  template<class TYPE> 
  tParameters<TYPE>::tParameters( const tParameters<TYPE> &source  ) 
  {
    this->elements = 0L;
    this->ROWS = source.GetRows();
    this->COLS = source.GetCols();
    this->Alloc();

    const tParameter<TYPE> *tmpp;
    for ( int  i = 0 ; i < this->GetRows() ; i++ ) 
      for ( int  j = 0 ; j < this->GetCols() ; j++ )
	{
	  tmpp = dynamic_cast<const tParameter<TYPE> *>( source( i + 1 , j + 1 ) );
	  if ( tmpp == 0L )
	    _DBTHROW( "tParameter element is not of type tParameter!" );
	  (*this->EPA(i,j))  = new tParameter<TYPE>( *tmpp );
	}
  }
  

  template<class TYPE> 
  bool tParameters<TYPE>::InitDerivatives( int depth , int dim  )
  { 
    _DB( std::cout << "int tParameters<TYPE>::InitDerivatives( int depth , int dim  )" << std::endl; );
    return true;
  }
  
  

  
}

template class P4th::tParameters<float>;
template class P4th::tParameters<double>;



