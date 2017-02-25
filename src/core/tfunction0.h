/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#ifndef _P4TH_TFUNCTION0_H_
#define _P4TH_TFUNCTION0_H_


#include <iostream>
#include <vector>
#include <typeinfo>
#include <linkedptrlist.h>
#include <tnmmatrix.h>
#include <texception.h>

/*

The smallest possible epsilon is 1 / 2^_EPSILONITERMAX
*/

namespace P4th
{


  template<class TYPE>
    class tfMatrix;

  template<class TYPE>
    class tConstant;

  template<class TYPE>
    class tFunctions;


  template<class TYPE>
    class tFunction0
  {
  public:
    /*
    virtual bool IsConstant() const
      { return false; }
    */
    virtual bool IsReal() const
      { return true; }
  protected:
    const static int _EPSILONITERMAX = 100;
    static TYPE _EPSILON;
    static TYPE _EPSILONx2;
    /*
    static tFunction0<TYPE> *errorf;
    virtual const tFunction0<TYPE> *ErrorFunction() const
    { return errorf; }
    */
    void SetEpsilon( const TYPE &eps )
      { _EPSILON = eps; _EPSILONx2 = eps + eps; }
    virtual string ErrorString() const
    { return (string)"Unspecified Error"; }
  public:
    tFunction0();
    virtual ~tFunction0()
      {}

    static TYPE Epsilon()
    { return _EPSILON; }
    static TYPE Epsilonx2()
    { return _EPSILONx2; }



    virtual void OnChangex( const tnmmatrix<TYPE> &x ) const
    {}

  public:


    virtual std::ostream &Dump( std::ostream &dest , string lm = "" ) const {
      dest << (const string)lm << (const string)__FILE__ << (const string)"(" << (void *)this << (const string)")" << std::endl;
      return dest;
    }
    
  public:
    TYPE &y( TYPE *res , const tnmmatrix<TYPE> *x ) const
      { *res = this->y( *x ); return *res; }
    TYPE &y( TYPE &res , const tnmmatrix<TYPE> &x ) const
      { res = this->y( x ); return res; }


    static void EpsilonInit();

    // Pure Virtuals
    virtual tFunction0 *Clone() const = 0;
    virtual tFunction0 *Reduce( int argnr , tFunction0 *f  ) const = 0;

    virtual tFunction0 *Simplify() { /* std::cerr << (string)"Warning simplify not implemented for " << typeid( this ).name() << std::endl; */ 
      return this; 
    };
    
    virtual int Nodes() const = 0;
    virtual bool DimOk( int d ) const = 0;
    virtual TYPE y( const tnmmatrix<TYPE> &x ) const = 0;
    virtual TYPE y( TYPE x ) const {
      tnmmatrix<TYPE> _x(1,1,x);
      return this->y(_x);
    }



    virtual void ResetDerivatives() = 0;
    virtual bool InitDerivatives( TYPE dx , int depth , int dim ) = 0;
    virtual const tFunction0<TYPE> *Getdydx( TYPE dx , int argnr , int argnr2 = -1 , ...  ) const = 0;
    virtual const tFunction0<TYPE> *Getdydx( int argnr ) const
    {
      //      std::cerr << "Warning, using depreciated function!" << std::endl;
      return Getdydx( 0 , argnr , -1 );
    }
    /*
    virtual void NumericalDerivatives() = 0;
    virtual void AnalyticalDerivatives() = 0;
    */


    // Basic Arthmetics
    static tFunction0<TYPE> *Add( tFunction0 *l , tFunction0 *r );
    static tFunction0<TYPE> *Sub( tFunction0 *l , tFunction0 *r );
    static tFunction0<TYPE> *Mul( tFunction0 *l , tFunction0 *r );
    static tFunction0<TYPE> *Div( tFunction0 *l , tFunction0 *r );
    static tFunction0<TYPE> *Pow( tFunction0 *l , tFunction0 *r );
    static tFunction0<TYPE> *Negate( tFunction0 *f );
    static tFunction0<TYPE> *Integrate( int argnr , int fdim ,
					tFunction0 *f ,
					tFunction0 *lb ,
					tFunction0 *ub ,
					int maxints = -1 ,
					TYPE maxabserr = -1 ,
					TYPE maxrelerr = -1 );
    static tFunction0<TYPE> *Composite( tFunction0<TYPE> *f , tFunction0<TYPE> *g , ... );

    /*
    static tFunction0<TYPE> *AddClone( const tFunction0 *l , const tFunction0 *r )
      { return AddIt( l->Clone() , r->Clone() ); }
    static tFunction0<TYPE> *SubClone( const tFunction0 *l , const tFunction0 *r )
    { return SubIt( l->Clone() , r->Clone() ); }
    static tFunction0<TYPE> *MulClone( const tFunction0 *l , const tFunction0 *r )
    { return MulIt( l->Clone() , r->Clone() ); }
    static tFunction0<TYPE> *DivClone( const tFunction0 *l , const tFunction0 *r )
    { return DivIt( l->Clone() , r->Clone() ); }
    */

    static tFunction0<TYPE> *Mul( tFunctions<TYPE> *fs );
    static tFunction0<TYPE> *Add( tFunctions<TYPE> *fs );
    static tFunction0<TYPE> *Composite( tFunction0<TYPE> *f , tFunctions<TYPE> *gs );

    static tFunction0<TYPE> *Add( tFunction0 *f ,tFunction0 *g , tFunction0 *h , ... );
    static tFunction0<TYPE> *Mul( tFunction0 *f ,tFunction0 *g , tFunction0 *h , ... );
    static tFunction0<TYPE> *Square( tFunction0 *f );


    // Misc
    virtual tnmmatrix<TYPE> &dydx( const tnmmatrix<TYPE> &dx ,
				   tnmmatrix<TYPE> &gradient , const tnmmatrix<TYPE> &x ) const;
    virtual tnmmatrix<TYPE> dydx( const tnmmatrix<TYPE> &dx , const tnmmatrix<TYPE> &x ) const
    { tnmmatrix<TYPE> gradient(x.GetRows(),1); return this->dydx( dx , gradient , x ); }

    virtual tnmmatrix<TYPE> &d2ydx2( const tnmmatrix<TYPE> &dx1 , const tnmmatrix<TYPE> &dx2 ,
				     tnmmatrix<TYPE> &hessian , const tnmmatrix<TYPE> &x ,
				     bool symmetric = false ) const;
    virtual tnmmatrix<TYPE> d2ydx2( const tnmmatrix<TYPE> &dx1 , const tnmmatrix<TYPE> &dx2 ,
				    const tnmmatrix<TYPE> &x ,
				    bool symmetric = false) const
    {
      tnmmatrix<TYPE> hessian(x.GetRows(),x.GetRows());
      return this->d2ydx2( dx1 , dx2 , hessian , x , symmetric );
    }


    virtual tnmmatrix<TYPE> &Ndydx( const tnmmatrix<TYPE> &dx , tnmmatrix<TYPE> &gradient ,
				    const tnmmatrix<TYPE> &x );
    virtual tnmmatrix<TYPE> Ndydx( const tnmmatrix<TYPE> &dx , const tnmmatrix<TYPE> &x )
    { tnmmatrix<TYPE> gradient(x.GetRows(),1); return this->Ndydx( dx , gradient , x ); }

    virtual tnmmatrix<TYPE> &Nd2ydx2( const tnmmatrix<TYPE> &dx2 ,
				      tnmmatrix<TYPE> &hessian ,
				      const tnmmatrix<TYPE> &x );
    virtual tnmmatrix<TYPE> Nd2ydx2( const tnmmatrix<TYPE> &dx2 ,
				     const tnmmatrix<TYPE> &x )
    { tnmmatrix<TYPE> hessian(x.GetRows(),x.GetRows()); return this->Nd2ydx2( dx2 , hessian , x ); }

    virtual int CheckGradient( const tnmmatrix<TYPE> &dx , const tnmmatrix<TYPE> &x );
    virtual int CheckHessian( const tnmmatrix<TYPE> &dx2 ,
			      const tnmmatrix<TYPE> &x );

    virtual tfMatrix<TYPE> *Gradient(  TYPE dx , int dim ) const;
    virtual tfMatrix<TYPE> *Hessian(  TYPE dx1 , TYPE dx2 , int dim ) const;
  };


   template<class TYPE>
    class Heap
  {
  protected:
    LinkedPtrList<TYPE *> funcs;
  public:
    ~Heap()
    {
      for ( int i = 0 ; i < funcs.GetSize() ; i++ )
	if ( funcs.Get( i ) != 0L )
	  delete funcs.Get( i );
    }

    TYPE *operator+=( TYPE *f )
      {
	funcs.Add( f );
	return f;
      }

    TYPE *operator-=( TYPE *f )
      {
	delete funcs.Get(this->funcs.GetSize() - 1 );
	this->funcs.Remove( this->funcs.GetSize() - 1 );
	funcs.Add( f );
	return f;
      }


    TYPE *Last( int i = 0 ) const
      {
	if ( this->funcs.GetSize() - i <= 0 )
	  _DBTHROW( "To few matrices on heap." );
	return this->funcs.Get( this->funcs.GetSize() - 1 - i );
      }

    TYPE *Pop( int i = 0 )
      {
	if ( this->funcs.GetSize() - i <= 0 )
	  _DBTHROW( "To few matrices on heap." );
	TYPE *res =  this->funcs.Get( this->funcs.GetSize() - 1 - i );
	this->funcs.Remove( this->funcs.GetSize() - 1 - i );
	return res;
      }



  };


}


#endif
