/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#ifndef _P4TH_TADD_H_
#define _P4TH_TADD_H_

#include <tderivatives0.h>
#include <tfunctions.h>
#include <tconstant.h>

//class istream;
//class ostream;


namespace P4th
{

  template<class TYPE> 
    class tAdd : public tDerivatives0<TYPE>
  {
  public:
    virtual int Nodes() const;
  protected:
    tFunctions<TYPE> fs;

    tAdd( tFunction0<TYPE> *l , tFunction0<TYPE> *r  );
    void CheckFirst();
    tAdd( tFunction0<TYPE> *l  );
  public:
    tAdd();
    //    tAdd( tFunctions<TYPE> &_fs  );
    tAdd( const tAdd<TYPE> &source ) : 
      tDerivatives0<TYPE>( source ) , fs( source.fs ) 
    { 
    }

    virtual ~tAdd();
        

    virtual bool DimOk( int d ) const
      {
	for ( int i = 1 ; i <= fs.GetSize() ; i++ )
	  if ( ! fs.Get( i )->DimOk( d ) ) 
	    return false;
	return true;
      }


    virtual void AddTerms( tAdd<TYPE> *f );
    virtual void AddTerm( tFunction0<TYPE> *f );


    virtual tFunction0<TYPE> *Simplify();

    virtual tAdd<TYPE> *Clone() const
    { return new tAdd( *this ); }
    
    virtual int Elements() const
    { return fs.GetSize(); }
    
    virtual tFunction0<TYPE> *AnalyticalDerivative( int argnr ) const;
    
    virtual TYPE y( const tnmmatrix<TYPE> &x ) const
    {
      TYPE res = 0;
      for ( int j = 1 ; j <= fs.GetSize() ; j++ )
	res += fs.Get( j )->y( x );
      return res;
    }
    virtual tAdd<TYPE> *Reduce( int argnr , tFunction0<TYPE> *s  ) const;

    virtual std::ostream &Dump( std::ostream &dest , string lm = "" ) const
    {
      dest << lm << __FILE__ << "(" << (void *)this << ")" << std::endl; 
      lm += "\t";
      dest << lm << fs.GetSize() << " terms:" << std::endl;
      for ( int j = 1 ; j <= fs.GetSize() ; j++ )
	{
	  dest << lm << "Term " << j << std::endl;
	  fs.Get( j )->Dump( dest , lm  );
	  if ( j < fs.GetSize() )
	    dest << lm << "+" << std::endl;
	}
      return dest;
    }
    
    friend class tFunction0<TYPE>;
  };
  
  
  
}


#endif
