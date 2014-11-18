/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#ifndef _P4TH_TMUL_H_
#define _P4TH_TMUL_H_

#include "tderivatives0.h"
#include "tfunctions.h"
#include "tconstant.h"

//class istream;
//class ostream;


namespace P4th
{

  template<class TYPE> 
    class tMul : public tDerivatives0<TYPE>
  {
  public:
    virtual int Nodes() const;
  protected:
    tFunctions<TYPE> fs;

    virtual tFunction0<TYPE> *Reduce( int argnr , tFunction0<TYPE> *s  ) const;
    void CheckFirst();
  public:
    tMul();
    tMul( tFunction0<TYPE> *l  );
    //    tMul( const tFunctions<TYPE> &_fs );
    tMul( tFunction0<TYPE> *l , tFunction0<TYPE> *r  );
    tMul( const tMul<TYPE> &source ) : 
      tDerivatives0<TYPE>( source ) , fs( source.fs ) 
    { 
    }

    virtual ~tMul();
        

    virtual bool DimOk( int d ) const
      {
	for ( int i = 1 ; i <= fs.GetSize() ; i++ )
	  if ( ! fs.Get( i )->DimOk( d ) ) 
	    return false;
	return true;
      }


    virtual void AddTerms( tMul<TYPE> *f );
    virtual void AddTerm( tFunction0<TYPE> *f );

    virtual tFunction0<TYPE> *Simplify();


    virtual tMul<TYPE> *Clone() const
    { return new tMul( *this ); }
    
    virtual int Elements() const
    { return fs.GetSize(); }
    
    virtual tFunction0<TYPE> *AnalyticalDerivative( int argnr ) const;
    
    virtual TYPE y( const tnmmatrix<TYPE> &x ) const
    {
      TYPE res = 1;
      TYPE tmp = 0;
      for ( int j = 1 ; j <= fs.GetSize() ; j++ )
	{
	  res *= fs.Get( j )->y( &tmp , &x );
	  if ( tmp == 0 )
	    return ( res = 0 );
	}
      return res;
    }

    virtual std::ostream &Dump( std::ostream &dest , string lm = "" ) const
    {
      dest << lm << __FILE__ << "(" << (void *)this << ")" << std::endl; 
      lm += "\t";
      for ( int j = 1 ; j <= fs.GetSize() ; j++ )
	{
	  fs.Get( j )->Dump( dest , lm  );
	  if ( j < fs.GetSize() )
	    dest << lm << "*" << std::endl;
	}
      return dest;
    }

    friend class tFunction0<TYPE>;
  };
  
  
  
}


#endif
