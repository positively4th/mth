/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#include "tmul.h"

#include "tadd.h"

namespace P4th
{


  template<class TYPE> 
  tMul<TYPE>::tMul() : 
    tDerivatives0<TYPE>()  
  {
  }
  
  template<class TYPE> 
  tMul<TYPE>::tMul( tFunction0<TYPE> *l ) : 
    tDerivatives0<TYPE>()  
  {
#ifdef _SAFE
    if ( l == 0L )
      _DBTHROW( "l == 0L" );
#endif

    AddTerm( l );
  }

  template<class TYPE> 
  tMul<TYPE>::tMul( tFunction0<TYPE> *l , tFunction0<TYPE> *r ) : 
    tDerivatives0<TYPE>()  
  {
#ifdef _SAFE
    if ( l == 0L )
      _DBTHROW( "l == 0L" );
#endif
    AddTerm( l );
    if ( r != 0L ) 
      AddTerm( r );
  }

  /*
  template<class TYPE> 
  tMul<TYPE>::tMul( const tFunctions<TYPE> &_fs  ) :
    tFunction<TYPE>()  
  {
    for ( int i = 1 ; i <= _fs.GetSize() ; i++ )
      MulClone(  _fs.Get( i ) );
  }
  */
  
  
  
  template<class TYPE> 
  tMul<TYPE>::~tMul() 
  {
    //    _DB( std::cout << "tMul<TYPE>::~tMul()" << std::endl; );
  }
  
  template<class TYPE> 
  int tMul<TYPE>::Nodes() const
  {
    int res = 1;
    for ( int i = 1 ; i <= fs.GetSize() ; i++ )
      res += this->fs.Get( i )->Nodes(); 
    return res;
  }

  template<class TYPE> 
  tFunction0<TYPE> *tMul<TYPE>::Reduce( int argnr , tFunction0<TYPE> *s  ) const
  {
    //    _DB( std::cout << "tFunction0<TYPE> *tMul<TYPE>::Reduce( int argnr , tFunction0<TYPE> *s  ) const" << std::endl; );
#ifdef _SAFE
    if ( fs.GetSize() < 1 )
      _DBTHROW( "fs.GetSize() < 1" );
#endif
    Heap<tFunction0<TYPE> > h;
    tMul<TYPE> *res = new tMul<TYPE>;
    for ( int i = 1 ; i <= fs.GetSize() - 1 ; i++ )
      res->fs.AddFunction( this->fs.Get( i )->Reduce( argnr , s->Clone() ) ); 

    res->fs.AddFunction( this->fs.Get( fs.GetSize() )->Reduce( argnr , s ) ); 
    return res;
  }

  template<class TYPE> 
  void tMul<TYPE>::CheckFirst()
  {
    // Check if the constant is 1, then remove it
    if ( fs.GetSize() < 2 )
      return;

    tConstant<TYPE> *firstf = dynamic_cast<tConstant<TYPE> *>( fs.Get( 1 ) );
    if ( firstf == 0L )
      return;

    if ( (*firstf) == 1 )
      delete fs.Steal( 1 );


  }

  template<class TYPE> 
  void tMul<TYPE>::AddTerms( tMul<TYPE> *f )
  { 
    //    _DB( std::cout << "Muling an Mul to an Mul ..." << std::endl; );

    // Copy all pointers functions
    while ( f->fs.GetSize() > 0 )
      this->AddTerm( f->fs.Steal( 1 ) );

    delete f;
  }

  template<class TYPE> 
  void tMul<TYPE>::AddTerm( tFunction0<TYPE> *f )
  { 
#ifdef _SAFE
    if ( f == 0L )
      _DBTHROW( "f == 0L" );
#endif
    //    _DB( std::cout << "f = " << (void *)f << std::endl; );




    f = f->Simplify();


    // If adding tMul collpse to one tMul
    tMul<TYPE> *aadd = dynamic_cast<tMul<TYPE> *>( f );
    if ( aadd != 0L )
      {
	return this->AddTerms( aadd );
      }


    // If no terms, just att new term and exit
    if ( fs.GetSize() < 1 )
      {
	fs.AddFunction( f );
	return;
      }


    // If first term a constant and zero, then no change
    tConstant<TYPE> *firstf = dynamic_cast<tConstant<TYPE> *>( fs.Get( 1 ) );
    if ( firstf != 0L )
      if ( (*firstf) == 0 )
	{
	  delete f;
	  return;
      }

    // If new term is zero clear all terms and add zero term
    tConstant<TYPE> *newf = dynamic_cast<tConstant<TYPE> *>( f );
    if ( newf != 0L )
      if ( (*newf) == 0 )
	{
	  fs.Free();
	  fs.AddFunction( f );
	  return;
	}
    

    // If new function is not a constant add it and exit
    if ( newf == 0L )
      {
	fs.AddFunction( f );
	CheckFirst();
	return;
      }
  
    
    // If first function not a constant, just add the new function first and and exit     
    if ( firstf == 0L )
      {
	fs.Insert( 1 , f );
	CheckFirst();
	return;
      }
    
    // If first function and new function are constants, collapse them
    *firstf = (TYPE)(*firstf) * (TYPE)(*newf);

    // Check if the constant is zero, then remove all other terms
    if ( (*firstf) == 0 )
      {
	fs.Free();
	fs.AddFunction( f );
	CheckFirst();
	return;
      }
    
    
    delete f;

  }

  template<class TYPE> 
  tFunction0<TYPE> *tMul<TYPE>::Simplify()
  {
    if ( fs.GetSize() > 1 )
      {
	CheckFirst();
	return this;
      }
    
    if ( fs.GetSize() == 0 )
      {
	return this;
      }    

    tFunction0<TYPE> *tmp = fs.Steal( 1 );
    
    delete this;
    
    return tmp->Simplify();
    
  }

  template<class TYPE> 
  tFunction0<TYPE> *tMul<TYPE>::AnalyticalDerivative( int argnr ) const
    {
      tAdd<TYPE> *res = new tAdd<TYPE>;
      tMul<TYPE> *tmp = 0L;
      for ( int i = 1 ; i <= fs.GetSize() ; i++ )
	{
	  tmp = new tMul<TYPE>;
	  for ( int j = 1 ; j <= fs.GetSize() ; j++ )
	    {
	      if ( i == j )
		tmp->AddTerm( this->fs.Get( j )->Getdydx( argnr )->Clone() );
	      else
		tmp->AddTerm( this->fs.Get( j )->Clone() );
	    }
	  res->AddTerm( tmp );
	}
      return res;
    }


 
}

template class P4th::tMul<float>;
template class P4th::tMul<double>;


