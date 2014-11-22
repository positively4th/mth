/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#pragma once

#include <vector>
#include <polymorphic.h>
#include <linkedptrlist.h>
#include <linkedlist.h>
#include <tnmmatrix.h>
#include <string>

//class istream;
//class ::std::ostream;


namespace P4th
{
  template<class TYPE> 
    class tFunction0; 

  template<class TYPE> 
    class tfMatrix; 
  

  template<class TYPE> 
    class tFunctions : public LinkedPtrList<tFunction0<TYPE> *> 
  {
  protected:
  public:
    tFunctions( tFunction0<TYPE> *f = 0L , ... );
    tFunctions( const tfMatrix<TYPE> *source );
    tFunctions( const tFunctions<TYPE> &source );
    virtual ~tFunctions();

    int GetDimension()
    { return this->GetSize(); }

    tFunction0<TYPE> *Get( int i ) const
    {
      return LinkedPtrList<tFunction0<TYPE> *>::Get( i - 1 );
    }

    virtual bool DimOk( int d ) const
      {
	for ( int i = 1 ; i <= this->GetSize() ; i++ ) 
	  if ( ! this->Get( i )->DimOk( d  ) ) 
	    return false;
	return true;
      }


    virtual tFunction0<TYPE> *Steal( int nr );

    virtual void AddFunction( tFunction0<TYPE> *f );
    virtual void Insert( int index , tFunction0<TYPE> *f );
    virtual void Replace( int index , tFunction0<TYPE> *f );
      
    virtual tFunctions *Clone() const;

    virtual void OnChangex( const tnmmatrix<TYPE> &x ) const
    {
      for ( int i = 0 ; i < this->GetSize() ; i++ )
	this->Get( i )->OnChangex( x );
    }
   
    virtual void ResetDerivatives()
    {
      for ( int i = 1 ; i <= this->GetSize() ; i++ )
	this->Get( i )->ResetDerivatives();
    }
    virtual bool InitDerivatives( bool analytical , int depth , int argnr );
    
    virtual tnmmatrix<TYPE> &y( tnmmatrix<TYPE> &res , const tnmmatrix<TYPE> &x ) const;
    virtual tnmmatrix<TYPE> y( const tnmmatrix<TYPE> &x ) const 
    { tnmmatrix<TYPE> res(this->GetSize(),1); this->y(res, x); return res;  };
    virtual LinkedList<TYPE> &y( LinkedList<TYPE> &res , const tnmmatrix<TYPE> &x ) const;
    virtual std::ostream &Dump( std::ostream &dest , string lm = "" ) const;

    virtual tFunctions<TYPE> *Reduce( int argnr , tFunction0<TYPE> *s  ) const;
    virtual int ForEvery( const tFunction0<TYPE> *(*func)( const tFunction0<TYPE> * , 
							   int index, Polymorphic * ) , Polymorphic *something = 0L ) const;
    
    
  };


   template<class TYPE> 
     class tfsHeap : public LinkedPtrList<tFunctions<TYPE> *> 
  {
  protected:
  public:
    ~tfsHeap() 
    {
      _DB( std::cout << "tHeap<TYPE>::~tHeap()" << std::endl );
      for ( int i = 0 ; i < this->GetSize() ; i++ )
	if ( this->Get( i ) != 0L )
	  delete this->Get( i );
      _DB( std::cout << "tHeap<TYPE>::~tHeap() done" << std::endl );
    }
    
    tFunctions<TYPE> *operator+=( tFunctions<TYPE> *m )
    {
      this->AddFunction( m );
      return m;
    }


    tFunctions<TYPE> *Last( int i = 0 ) const
      {
	if ( this->GetSize() - i <= 0 )
	  _DBTHROW( "To few matrices on heap." );
	return this->Get( this->GetSize() - 1 - i ); 
      }

    tFunctions<TYPE> *Pop()
      {
	if ( this->GetSize() <= 0 )
	  _DBTHROW( "To few matrices on heap." );
	tFunctions<TYPE> *res =  this->Get( this->GetSize() - 1 );
	this->Remove( this->GetSize() - 1 );
	return res; 
      }
  };

}


