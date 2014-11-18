/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#ifndef _P4TH_TFMATRIX0_H_
#define _P4TH_TFMATRIX0_H_

#include <debug.h>
#include <string>
#include "linkedptrlist.h"
#include "tfunction0.h"
#include "tnmmatrix.h"

//class istream;
//class ostream;


using namespace std;

namespace P4th
{

  class Polymorphic;

  template<class TYPE> 
    class tfMatrix0
 {
 public:
   virtual int Nodes() const = 0;
  protected:
 public:
   virtual tFunction0<TYPE> **EPA( int i0based , int j0based ) = 0;
   virtual const tFunction0<TYPE> *EP( int i0based , int j0based ) const = 0;
   virtual ~tfMatrix0() 
     {};

   virtual int GetRows() const = 0;
   virtual int GetCols() const = 0;
   virtual bool DimOk( int d ) const = 0;
   
   virtual tFunction0<TYPE> *Ref( int i , int j = 1 ) = 0;
   virtual const tFunction0<TYPE> *operator()( int i , int j = 1 ) const = 0;

   virtual tFunction0<TYPE> *Cpy( int i , int j = 1 ) const = 0;
   virtual tfMatrix0<TYPE> *Clone() const = 0;
   
   virtual tFunction0<TYPE> *Set( int i , int j  , tFunction0<TYPE> *f ) = 0;
   virtual tFunction0<TYPE> *Steal( int i , int j ) = 0;
   
   virtual int ForEvery( const tFunction0<TYPE> *(*func)( const tFunction0<TYPE> * , int row , int col , Polymorphic * ) , Polymorphic *something = 0L ) const = 0;
   
   virtual tfMatrix0<TYPE> *Simplify() = 0;
   virtual tnmmatrix<TYPE> y( const tnmmatrix<TYPE> &x ) const = 0;

   virtual tnmmatrix<TYPE> &y( tnmmatrix<TYPE> *y , const tnmmatrix<TYPE> *x ) const
   { y->tnmContainer<TYPE>::Cpy( this->y( *x ) ); return *y; }
   virtual tnmmatrix<TYPE> &y( tnmmatrix<TYPE> &y , const tnmmatrix<TYPE> &x ) const
   { y.tnmContainer<TYPE>::Cpy( this->y( x ) ); return y; }

   virtual tfMatrix0<TYPE> *Reduce( int argnr , tFunction0<TYPE> *s ) const = 0;


   static tfMatrix0<TYPE> *ColumnDerivatives( tfMatrix0<TYPE> *a , int columnnr = 1 , int dim = - 1);
   
   virtual std::ostream &Dump( std::ostream &dest , string lm = "" ) const = 0; 

   virtual bool InitDerivatives( bool analytical , int depth , int dim  ) = 0;
   /*
   virtual void NumericalDerivatives() = 0;
   virtual void AnalyticalDerivatives() = 0;
   */


  };



   template<class TYPE> 
     class tmHeap : public LinkedPtrList<tfMatrix0<TYPE> *> 
  {
  protected:
  public:
    ~tmHeap() 
    {
      //      _DB( std::cout << "tHeap<TYPE>::~tHeap()" << std::endl );
      for ( int i = 0 ; i < this->GetSize() ; i++ )
	if ( this->Get( i ) != 0L )
	  delete this->Get( i );
      //      _DB( std::cout << "tHeap<TYPE>::~tHeap() done" << std::endl );
    }
    
    tfMatrix0<TYPE> *operator+=( tfMatrix0<TYPE> *m )
    {
      this->Add( m );
      return m;
    }


    tfMatrix0<TYPE> *Last( int i = 0 ) const
      {
	if ( this->GetSize() - i <= 0 )
	  _DBTHROW( "To few matrices on heap." );
	return this->Get( this->GetSize() - 1 - i ); 
      }

    tfMatrix0<TYPE> *Pop()
      {
	if ( this->GetSize() <= 0 )
	  _DBTHROW( "To few matrices on heap." );
	tfMatrix0<TYPE> *res =  this->Get( this->GetSize() - 1 );
	this->Remove( this->GetSize() - 1 );
	return res; 
      }
  };
}



#endif
