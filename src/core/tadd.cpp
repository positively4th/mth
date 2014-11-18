/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#include "tadd.h"

namespace P4th
{


  template<class TYPE> 
  tAdd<TYPE>::tAdd() : 
    tDerivatives0<TYPE>()  
  {
  }
  
  template<class TYPE> 
  tAdd<TYPE>::tAdd( tFunction0<TYPE> *l ) : 
    tDerivatives0<TYPE>()  
  {
#ifdef _SAFE
    if ( l == 0L )
      _DBTHROW( "l == 0L" );
#endif
    AddTerm( l );
  }

  template<class TYPE> 
  tAdd<TYPE>::tAdd( tFunction0<TYPE> *l , tFunction0<TYPE> *r ) : 
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
  
  
  
  template<class TYPE> 
  tAdd<TYPE>::~tAdd() 
  {
    //    _DB( std::cout << "tAdd<TYPE>::~tAdd()" << std::endl; );
  }
  
  template<class TYPE> 
  int tAdd<TYPE>::Nodes() const
  {
    int res = 1;
    for ( int i = 1 ; i <= fs.GetSize() ; i++ )
      res += this->fs.Get( i )->Nodes(); 
    return res;
  }

  template<class TYPE> 
  tAdd<TYPE> *tAdd<TYPE>::Reduce( int argnr , tFunction0<TYPE> *s  ) const
  {
#ifdef _SAFE
    if ( fs.GetSize() < 1 )
      _DBTHROW( "fs.GetSize() < 1" );
#endif
    Heap<tFunction0<TYPE> > h;
    tAdd<TYPE> *res = new tAdd<TYPE>;
    for ( int i = 1 ; i <= fs.GetSize() - 1 ; i++ )
      res->fs.AddFunction( this->fs.Get( i )->Reduce( argnr , s->Clone() ) ); 
    
    res->fs.AddFunction( this->fs.Get( fs.GetSize() )->Reduce( argnr , s ) ); 


    return res;
  }

  template<class TYPE> 
  void tAdd<TYPE>::AddTerms( tAdd<TYPE> *f )
  { 
    //    _DB( std::cout << "Adding an Add to an Add ..." << std::endl; );

    // Copy all pointers functions
    while ( f->fs.GetSize() > 0 )
      this->AddTerm( f->fs.Steal( 1 ) );

    delete f;
  }

  template<class TYPE> 
  void tAdd<TYPE>::CheckFirst()
  {
    // Check if the constant is 1, then remove it
    if ( fs.GetSize() < 2 )
      return;

    tConstant<TYPE> *firstf = dynamic_cast<tConstant<TYPE> *>( fs.Get( 1 ) );
    if ( firstf == 0L )
      return;

    if ( (*firstf) == 0 )
      delete fs.Steal( 1 );

  }

  template<class TYPE> 
  void tAdd<TYPE>::AddTerm( tFunction0<TYPE> *f )
  { 
#ifdef _SAFE
    if ( f == 0L )
      _DBTHROW( "f == 0L" );
#endif
    //    _DB( std::cout << "f = " << (void *)f << std::endl; );


    f = f->Simplify();

    if ( fs.GetSize() < 1 )
      {
	fs.AddFunction( f );
	return;
      }

    // If adding tAdd collpse to one tAdd
    tAdd<TYPE> *aadd = dynamic_cast<tAdd<TYPE> *>( f );
    if ( aadd != 0L )
      {
	return this->AddTerms( aadd );
      }

    tConstant<TYPE> *newf = dynamic_cast<tConstant<TYPE> *>( f );
    if ( newf == 0L )
      {
	fs.AddFunction( f );
	CheckFirst();
	return;
      }
    
    tConstant<TYPE> *firstf = dynamic_cast<tConstant<TYPE> *>( fs.Get( 1 ) );
    if ( firstf == 0L )
      {
	fs.Insert( 1 , f );
	CheckFirst();
	return;
      }
    
    //    _DB( std::cout << "Collapsing in first term!" << std::endl; );
    *firstf = (TYPE)(*firstf) + (TYPE)(*newf);
    delete f;

    CheckFirst();
  }

  template<class TYPE> 
  tFunction0<TYPE> *tAdd<TYPE>::Simplify()
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
  tFunction0<TYPE> *tAdd<TYPE>::AnalyticalDerivative( int argnr ) const
    {
      //      std::cout << "." << std::flush;
      Heap<tAdd<TYPE> > h;
      
      h += new tAdd;
      for ( int i = 1 ; i <= fs.GetSize() ; i++ )
	h.Last()->AddTerm( fs.Get( i )->Getdydx( argnr )->Clone() );
      
      return h.Pop();
    }


 
}

template class P4th::tAdd<float>;
template class P4th::tAdd<double>;


