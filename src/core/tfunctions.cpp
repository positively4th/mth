/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#include "tfunctions.h"

#include <stdarg.h>

#include <debug.h>
#include <math.h>
#include <string>
#include <stlstringtools.h>

#include "tfunction0.h"
#include "tfmatrix.h"
#include "tadd.h"


namespace P4th
{

  template<class TYPE> 
  tFunctions<TYPE>::tFunctions( tFunction0<TYPE> *f , ... )
  {
    if ( f == 0L )
      return;
    
    this->AddFunction( f );
    va_list arglist;
    va_start(arglist,f);


    tFunction0<TYPE> *tmp;
    for ( ;; )
      {
	tmp = va_arg(arglist, tFunction0<TYPE> * );
	if ( tmp == 0L )
	  break;

	if ( dynamic_cast<tFunction0<TYPE> *>( tmp ) == 0L )
	  _DBTHROW( "Pointer not NULL but not a tFunction0 pointer!" ); 
	
	this->AddFunction( tmp );
      }
    va_end( arglist );
  }

  template<class TYPE> 
  tFunctions<TYPE>::tFunctions( const tfMatrix<TYPE> *source )
  {
    _DB( std::cout 
	 << "tFunctions<TYPE>::tFunctions( const tfMatrix<TYPE> *source )" 
	 << std::endl; );
    for ( int i = 1 ; i <= source->GetRows() ; i++ )
      for ( int j = 1 ; j <= source->GetCols() ; j++ )
	{
	  _DB( std::cout << "i = " << i << ", j = " << j << std::endl; );
	  if ( (*source)(i,j) != 0L )
	    this->AddFunction( (*source)(i,j)->Clone() );
	}
  }

  template<class TYPE> 
  tFunctions<TYPE>::tFunctions( const tFunctions<TYPE> &source ) 
  {
    //    _DB( std::cout << "tFunctions<TYPE>::tFunctions( const tFunctions<TYPE> &source )" << std::endl; );
    for ( int i = 1 ; i <= source.GetSize() ; i++ )
      {
	/*
#ifdef _SAFE
	if ( source.Get( i ) == 0L )
	  _DBTHROW( "source.Get( i ) == 0L" );
#endif	
	*/
	// tDerivatives member dydxs have 0L pointers!!
	if ( source.Get( i ) != 0L )
	  this->AddFunction( source.Get( i )->Clone() );
	else
	  this->AddFunction( (tFunction0<TYPE> *)0L );
      }
  }


  template<class TYPE> 
  tFunctions<TYPE>::~tFunctions()
  {
    //    _DB( std::cout << "tFunctions<TYPE>::~tFunctions()" << std::endl; );
    
    /*
    for ( int i = 1 ; i <= this->GetSize() ; i++ )
      {
	if ( this->Get( i ) != 0L )
	  {
	    delete this->Get(i);
	  }
      }

    this->Delete();
    /*
    /*
    
    for ( int i = this->GetSize() ; i >= 1 ; i-- )
      if ( this->Get( i ) != 0L )
	{
	  delete this->Get(i);
	}
    */
    this->Free();
  }

  template<class TYPE> 
  tFunctions<TYPE> *tFunctions<TYPE>::Clone() const
  { 
    return new tFunctions<TYPE>( *this );
  }
  

  template<class TYPE> 
  tFunction0<TYPE> *tFunctions<TYPE>::Steal( int nr )
  {
    tFunction0<TYPE> *tmp = Get( nr );
    LinkedPtrList<tFunction0<TYPE> *>::Remove( nr - 1  );
    return tmp;
  }

  
  template<class TYPE> 
  void tFunctions<TYPE>::AddFunction( tFunction0<TYPE> *f )
  { 
    LinkedPtrList<tFunction0<TYPE> *>::Add( f ); 
  }
  
  template<class TYPE> 
  void tFunctions<TYPE>::Insert( int index , tFunction0<TYPE> *f )
  { 
#ifdef _SAFE
    if ( index < 1 || index > this->GetSize() + 1 )
      _DBTHROW( "index out of bounds" );
#endif

    LinkedPtrList<tFunction0<TYPE> *>::Insert( index - 1 , f ); 

  }
  
  template<class TYPE> 
  void tFunctions<TYPE>::Replace( int index , tFunction0<TYPE> *f )
  { 
#ifdef _SAFE
    if ( index < 1 || index > this->GetSize() + 1 )
      _DBTHROW( "index out of bounds" );
#endif

    tFunction0<TYPE> *ptr = this->Get( index ); 
    if ( ptr == f )
      return;
    if ( ptr != NULL )
      delete ptr;
    LinkedPtrList<tFunction0<TYPE> *>::Replace( index - 1 , f );

  }
  
  template<class TYPE> 
  LinkedList<TYPE> &tFunctions<TYPE>::y( LinkedList<TYPE> &res , const tnmmatrix<TYPE> &x ) const
  {
    res.Delete();
    TYPE tmp;
    for ( int i = 1 ; i <= this->GetSize() ; i++ )
      {
	tmp = this->Get(i)->y( x );
	res.Add( tmp );
      }
    return res;
  }
  
  template<class TYPE> 
  tnmmatrix<TYPE> &tFunctions<TYPE>::y( tnmmatrix<TYPE> &res , const tnmmatrix<TYPE> &x ) const
  {
#ifdef _SAFE
    if ( res.GetCols() < 1 || res.GetRows() != this->GetSize() )
      _DBTHROW( "tnmmatrix<TYPE> &tFunctions<TYPE>::y( tnmmatrix<TYPE> &res , const tnmmatrix<TYPE> &x ) const : res.GetCols() < 1 || res.GetRows() != this->GetSize()" );
#endif

    for ( int i = 1 ; i <= this->GetSize() ; i++ )
      this->Get(i)->y( &res.Ref( i , 1 ) , &x );
    return res;
  }
  

  template<class TYPE> 
  bool tFunctions<TYPE>::InitDerivatives( bool analytical , int depth , int dim )
  {
    if ( depth <= 0 )
      return dim;
    int res;
    for ( int i = 1 ; i <= this->GetSize() ; i++ )
      {
	if ( ! LinkedPtrList<tFunction0<TYPE> *>::Get( i - 1 )->InitDerivatives( analytical , depth , dim ) )
	  return false;
      }
    return true;
  }
  
  template<class TYPE> 
  std::ostream &tFunctions<TYPE>::Dump( std::ostream &dest , string lm ) const
  { 
    dest << lm << __FILE__ << "(" << (void *)this << ")" << std::endl;
    lm += "\t";
    for ( int i = 1 ; i <= this->GetSize() ; i++ )
      {
	dest << lm << "Function " << i << ":" << std::endl;
	this->Get(i)->Dump( dest , lm );
      }
    return dest;
    
  }

  template<class TYPE> 
  tFunctions<TYPE> *tFunctions<TYPE>::Reduce( int argnr , tFunction0<TYPE> *s  ) const
  {
    /*
    _DB( std::cout << "tFunctions<TYPE> *tFunctions<TYPE>::Reduce( int argnr , tFunction0<TYPE> *s  ) const" << std::endl; );
    _DB( std::cout << "argnr = " << argnr << std::endl; );
    _DB( s->Dump( std::cout , "s:\t" ); );
    */
    tFunctions<TYPE> *res = new tFunctions<TYPE>;
    if ( this->GetSize() < 1 )
      {
	delete s;
	return res;
      }
    //    _DB( s->Dump( std::cout , "s: " ); );
    for ( int i = 1 ; i <= this->GetSize() - 1 ; i++ )
      {
	//	_DB( this->Get( i )->Dump( std::cout , STLStringTools::Int2String( i ) + ":" ); );
	res->AddFunction( this->Get( i )->Reduce( argnr , s->Clone() ) );
      }
    //    _DB( this->Get( this->GetSize() )->Dump( std::cout , 
    //				   STLStringTools::Int2String( this->GetSize() ) + ":" ); );
    res->AddFunction( this->Get( this->GetSize()  )->Reduce( argnr , s ) );
    return res;
  }
  
  template<class TYPE> 
  int tFunctions<TYPE>::ForEvery( const tFunction0<TYPE> *(*func)( const tFunction0<TYPE> * , 
						 int index, Polymorphic * ) , Polymorphic *something ) const 
  {
    int ctr = 0;
    const tFunction0<TYPE> *ptr;
    for ( int i = 1 ; i <= this->GetSize() ; i++ ) {
      (*func)( this->Get( i ) , i , something );
      ctr++;
    }
    return ctr;
  }
  
  template class P4th::tFunctions<float>;
  template class P4th::tFunctions<double>;


  
}

#include <linkedptrlist.cpp>
#include <linkedptrlistpost.cpp>
template class P4th::LinkedPtrList<P4th::tFunctions<float> *>;
template class P4th::LinkedPtrListPost<P4th::tFunctions<float> *>;
template class P4th::LinkedPtrList<P4th::tFunctions<double> *>;
template class P4th::LinkedPtrListPost<P4th::tFunctions<double> *>;
