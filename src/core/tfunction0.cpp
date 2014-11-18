/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#include "tfunction0.h"

#include <debug.h>
#include <math.h>
#include <string>
#include <stlstringtools.h>
#include <tpointerstack.h>

#include <stdarg.h>

//#include "tderivative.h"
#include "tfmatrix.h"
#include "tmathtools.h"
#include "tadd.h"
#include "tsub.h"
#include "tmul.h"
#include "tdiv.h"
#include "tpow.h"
#include "tintegral.h"
#include "tconstant.h"
#include <tnegative.h>
#include <treciprocal.h>
#include <tsquare.h>


using namespace std;


namespace P4th
{


  //  template<class TYPE> 
  //  const static int _EPSILONITERMAX = 100;

  template<class TYPE> 
  TYPE tFunction0<TYPE>::_EPSILON = (TYPE)0.0; 

  template<class TYPE> 
  TYPE tFunction0<TYPE>::_EPSILONx2 = 2 * pow( 10 , -10 ); 

  /*  template<class TYPE> 
  tFunction0<TYPE> *tFunction0<TYPE>::errorf = 0L;
  */

  template<class TYPE> 
  tFunction0<TYPE>::tFunction0()
  { 

    if ( _EPSILON == (TYPE)0.0 ) 
      EpsilonInit();
  }


  template<class TYPE> 
  void tFunction0<TYPE>::EpsilonInit()
  {
    TYPE x1 = 1;
    TYPE x0 = 0;
    TYPE x;

    unsigned int ctr = 0;
    while ( x1 != x0 && ctr < _EPSILONITERMAX  )
      {
	//	_DB( std::cout << "(x0,x1) = (" << x0 << "," << x1 << ")" << std::endl; );
	x = (x0 + x1) / 2; 
	if ( (TYPE)1.0 + x != (TYPE)1.0 )
	  x1 = x; 
	else
	  x0 = x;

	ctr ++;
      }
    _EPSILON = x1;
    _DB( std::cout << "New EPSILON is " << _EPSILON << std::endl; );
    
    _EPSILONx2 = _EPSILON + _EPSILON; 
    _DB( std::cout << "New EPSILONx2 is " << _EPSILONx2 << std::endl; );
 }

  template<class TYPE> 
  tnmmatrix<TYPE> &tFunction0<TYPE>::dydx( const tnmmatrix<TYPE> &dx , tnmmatrix<TYPE> &gradient , 
					   const tnmmatrix<TYPE> &x ) const
  {
    //    _DB( std::cout << "dydx" );

    //    _DB( dx.PrintTrp( std::cout , "dx': \t" ) );
    TYPE dy;
#ifdef _SAFE
    if ( ! this->DimOk( gradient.GetRows() ) )
	 _DBTHROW( "Gradient rows do not match the dimension of the functions domain." );
#endif

    for ( int i = 1 ; i <= gradient.GetRows() ; i++ )
      {
	//	std::cout << "i=" << i << std::endl; 
	gradient(i,1) = Getdydx( dx.GetRepeated( i , 1 ) , i , -1 )->y( &dy , &x );
      }
    //    _DB( std::cout << "...done" << std::endl; );
    
    return gradient;

  }
  
  template<class TYPE> 
  tnmmatrix<TYPE> &tFunction0<TYPE>::d2ydx2( const tnmmatrix<TYPE> &dx1 , const tnmmatrix <TYPE> &dx2 , 
					     tnmmatrix<TYPE> &hessian , 
					     const tnmmatrix<TYPE> &x , bool symmetric ) const
  {
    hessian.tnmContainer<TYPE>::Cpy( tMathTools<TYPE>::Inf() );
    const tFunction0<TYPE> *tmp;
    for ( int i = 1 ; i <= x.GetRows() ; i++ )
      {
	for ( int j = 1 ; j <= x.GetRows() ; j++ )
	  {
	    if ( j > i && symmetric )
	      {
		break;
	      }
	    // Ensure derivative with d = dx
	    tmp = Getdydx( dx1.GetRepeated( i , 1 ) , i   );
	    // Use that firstorde derivative to get derivative with d = dx2
	    hessian(i,j) = tmp->Getdydx( dx2.GetRepeated( i , j ) , j , -1 )->y( x );
  // Wrong!!!
	    //	    hessian(i,j) = Getdydx( dx2.GetRepeated( i , j ) , i , j , -1 )->y( x );
	    //	    _DB( std::cout << "Stored (i,j)=(" << i << "," << j << ")" << std::endl; );
	    if ( symmetric && i != j )
	      {
		hessian(j,i) = hessian(i,j);
	      }
	  }
      }
    return hessian;
  }

  template<class TYPE> 
  tFunction0<TYPE> *tFunction0<TYPE>::Add( tFunction0<TYPE> *l , tFunction0<TYPE> *r )
  {
#ifdef _SAFE
    if ( l == 0L )
      _DBTHROW( "l == 0L" );
    if ( r == 0L )
      _DBTHROW( "r == 0L" );
#endif

    return tFunction0<TYPE>::Add( l , r , 0L );
  }

  template<class TYPE> 
  tFunction0<TYPE> *tFunction0<TYPE>::Sub( tFunction0<TYPE> *l , tFunction0<TYPE> *r )
  {
#ifdef _SAFE
    if ( l == 0L )
      _DBTHROW( "l == 0L" );
    if ( r == 0L )
      _DBTHROW( "r == 0L" );
#endif

    l = l->Simplify();
    r = r->Simplify();
    const tConstant<TYPE> *lconst = dynamic_cast<const tConstant<TYPE> *>( l );
    const tConstant<TYPE> *rconst = dynamic_cast<const tConstant<TYPE> *>( r );
    if ( lconst != 0L && rconst != 0L )
      {
	tConstant<TYPE> *res = new tConstant<TYPE>( (TYPE)(*lconst) - (TYPE)(*rconst) );
	delete r;
	delete l;
	return res;
      }
    if ( lconst != 0L )
      if ( (TYPE)(*lconst) == (TYPE)0 )
	{
	  delete l;
	  return tFunction0<TYPE>::Negate( r );
	}
    
    if ( rconst != 0L )
      if ( (TYPE)(*rconst)  == (TYPE)0 )
	{
	  delete r;
	  return l;
	}

    return new tSub<TYPE>( l , r  );
  }
  
  template<class TYPE> 
  tFunction0<TYPE> *tFunction0<TYPE>::Mul( tFunction0<TYPE> *l , tFunction0<TYPE> *r )
  {
#ifdef _SAFE
    if ( l == 0L )
      _DBTHROW( "l == 0L" );
    if ( r == 0L )
      _DBTHROW( "r == 0L" );
#endif

    return tFunction0<TYPE>::Mul( l , r , 0L );
  }
  
  template<class TYPE> 
  tFunction0<TYPE> *tFunction0<TYPE>::Div( tFunction0<TYPE> *l , tFunction0<TYPE> *r )
  {
#ifdef _SAFE
    if ( l == 0L )
      _DBTHROW( "l == 0L" );
    if ( r == 0L )
      _DBTHROW( "r == 0L" );
#endif

    l = l->Simplify();
    r = r->Simplify();

    const tConstant<TYPE> *lconst = dynamic_cast<const tConstant<TYPE> *>( l );
    const tConstant<TYPE> *rconst = dynamic_cast<const tConstant<TYPE> *>( r );
    if ( lconst != 0L && rconst != 0L )
      {
	TYPE tmp = (TYPE)(*lconst) / (TYPE)(*rconst);
	delete l;
	delete r;
	return new tConstant<TYPE>( tmp );
      }
    if ( lconst != 0L )
      if ( (TYPE)(*lconst) == (TYPE)0 )
	{
	  delete l;
	  delete r;
	  return new tConstant<TYPE>( 0.0 );
	}
    if ( rconst != 0L )
      if ( (TYPE)(*rconst) == (TYPE)0 )
	{
	  delete l;
	  delete r;
	  return new tConstant<TYPE>( tMathTools<TYPE>::Inf() );
	}
    if ( lconst != 0L )
      if ( (TYPE)(*lconst) == (TYPE)1 )
	{
	  delete l;
	  return new tReciprocal<TYPE>( r );
	}
    if ( rconst != 0L )
      if ( (TYPE)(*rconst)  == (TYPE)1 )
	{
	  delete r;
	  return l->Clone();
	}
    return new tDiv<TYPE>( l , r  );
  }
  
  template<class TYPE> 
  tFunction0<TYPE> *tFunction0<TYPE>::Pow( tFunction0<TYPE> *l , tFunction0<TYPE> *r )
  {
#ifdef _SAFE
    if ( l == 0L )
      _DBTHROW( "l == 0L" );
    if ( r == 0L )
      _DBTHROW( "r == 0L" );
#endif

    l = l->Simplify();
    r = r->Simplify();

    return new tPow<TYPE>( l , r  );
  }
  
  template<class TYPE> 
  tFunction0<TYPE> *tFunction0<TYPE>::Negate( tFunction0<TYPE> *f )
  {
#ifdef _SAFE
    if ( f == 0L )
      _DBTHROW( "f == 0L" );
#endif

    //    f = f->Simplify();

    tConstant<TYPE> *fconst = dynamic_cast<tConstant<TYPE> *>( f );

    if ( fconst == NULL )
      {
	return new tNegative<TYPE>( f );
      }
    /*
    _DB( std::cout << "* * * Simplifying negation since f is a constant. " 
	 << "__PRETTY__FUNCTION__" 
	 << at " << __LINE__" << std::endl; );
    */
    tPointerStack<tFunction0<TYPE> > fstack;

    fstack += f;
    return new tConstant<TYPE>( -(TYPE)(*fconst)  );

  }

  template<class TYPE> 
  tFunction0<TYPE> *tFunction0<TYPE>::Square( tFunction0<TYPE> *f )
  {
#ifdef _SAFE
    if ( f == 0L )
      _DBTHROW( "f == 0L" );
#endif

    tConstant<TYPE> *fconst = dynamic_cast<tConstant<TYPE> *>( f );

    if ( fconst == NULL ) {
      return new tSquare<TYPE>(f);
    }

    tPointerStack<tFunction0<TYPE> > fstack;

    fstack += f;
    return new tConstant<TYPE>( (TYPE)(*fconst) * (TYPE)(*fconst)  );

  }

  template<class TYPE> 
  tFunction0<TYPE> *tFunction0<TYPE>::Integrate( int argnr , int fdim , 
						 tFunction0<TYPE> *f , 
						 tFunction0<TYPE> *lb , 
						 tFunction0<TYPE> *ub ,
						 int maxints  , 
						 TYPE maxabserr , 
						 TYPE maxrelerr  )
  {
#ifdef _SAFE
    if ( f == 0L )
      _DBTHROW( "f == 0L" );
    if ( lb == 0L )
      _DBTHROW( "lb == 0L" );
    if ( ub == 0L )
      _DBTHROW( "ub == 0L" );
#endif

    //    f = f->Simplify();

    tConstant<TYPE> *lbconst = dynamic_cast<tConstant<TYPE> *>( lb );
    tConstant<TYPE> *ubconst = dynamic_cast<tConstant<TYPE> *>( ub );

    
    if ( ubconst != NULL && lbconst != NULL )
      {
	if ( (TYPE)(*lbconst) == (TYPE)(*ubconst) )
	  {
	    _DB( std::cout << "Simplifying integral, lb == ub" << std::endl; ); 
	    delete lb;
	    delete ub;
	    delete f;
	    return new tConstant<TYPE>( 0.0 );
	  }
      }
    
    tConstant<TYPE> *fconst = dynamic_cast<tConstant<TYPE> *>( f );

    tPointerStack<tFunction0<TYPE> > fstack;

    if ( fconst != NULL )
      {
	_DB( std::cout << "Simplifying integral, f is a constant" << std::endl; ); 
	fstack += tFunction0<TYPE>::Sub( ub , lb );
	return tFunction0<TYPE>::Mul( f , fstack.Pop() );
      }

    //    tFoG<TYPE> *tfog = new tFoG

    tPointerStack<tFunctions<TYPE> > fsstack( new tFunctions<TYPE> );
    int j = 1;
    for ( int i = 1 ; i <= fdim ; i++ )
      if ( i != argnr ) 
	fsstack.Last()->AddFunction( new tSelector<TYPE>( i ) );
      else
	fsstack.Last()->AddFunction( new tIntegralSelector<TYPE>() );


    return new tIntegral<TYPE>( tFunction0<TYPE>::Composite( f , fsstack.Pop() ) , 
				lb , ub , 
				maxints , 
				maxabserr , 
				maxrelerr );
  }
  
  template<class TYPE> 
  tFunction0<TYPE> *tFunction0<TYPE>::Composite( tFunction0<TYPE> *f , tFunction0<TYPE> *g , ... )
  {
    

    tFunctions<TYPE> *gs = new tFunctions<TYPE>();
    

    if ( g != NULL )
      {
	gs->AddFunction( g );

	va_list arglist;
	va_start(arglist,g);
	
	
	tFunction0<TYPE> *tmp;
	for ( ;; )
	  {
	    tmp = va_arg(arglist, tFunction0<TYPE> * );
	    if ( tmp == 0L )
	      break;
	    
	    gs->AddFunction( tmp );
	  }
	va_end( arglist );
      }
    return Composite( f , gs );
  }


  template<class TYPE> 
  tFunction0<TYPE> *tFunction0<TYPE>::Composite( tFunction0<TYPE> *f , tFunctions<TYPE> *gs )
  {
    
    tPointerStack<tFunctions<TYPE> > fsstack( gs );

    
    if( fsstack.Last()->GetSize() < 1 )
      _DBTHROW( "fsstack.Last()->GetSize()" );
    
    tConstant<TYPE> *fconst = dynamic_cast<tConstant<TYPE> *>( f );
    if ( fconst != NULL )
      {
	return f;
      }
    
    return new tFoG<TYPE>( f , fsstack.Pop() );
    
  }




  template<class TYPE> 
  tFunction0<TYPE> *tFunction0<TYPE>::Add( tFunction0<TYPE> *f1 , tFunction0<TYPE> *f2 , tFunction0<TYPE> *f3 , ... )
  {

    tFunctions<TYPE> *tmpfs = new tFunctions<TYPE>();

    if ( f1 != 0L )
      tmpfs->AddFunction( f1 );

    if ( f2 != 0L )
      tmpfs->AddFunction( f2 );

    if ( f3 != 0L )
      {
	tmpfs->AddFunction( f3 );
	
	va_list arglist;
	va_start(arglist,f3);
	
	
	tFunction0<TYPE> *tmp;
	for ( ;; )
	  {
	    tmp = va_arg(arglist, tFunction0<TYPE> * );
	    if ( tmp == 0L )
	      break;
	    
	    tmpfs->AddFunction( tmp );
	  }
	va_end( arglist );
	
	
	if( tmpfs->GetSize() < 1 )
	  _DBTHROW( "tmpfs.GetSize() < 1" );
      }
    return Add( tmpfs );
  }

  template<class TYPE> 
  tFunction0<TYPE> *tFunction0<TYPE>::Add( tFunctions<TYPE> *fs  )
  {
    /*
    _DB( std::cout << "tFunction0<TYPE> *tFunction0<TYPE>::Add( tFunctions<TYPE> *fs  )" << std::endl; );
    _DB( std::cout << "fs->GetSize() = " << fs->GetSize() << std::endl; );
    */

    tPointerStack<tFunctions<TYPE> > fsstack;
    fsstack += fs;

    if ( fs->GetSize() < 1 )
      return 0L;

    if ( fs->GetSize() == 1 )
      {
	_DB( std::cout << "\n\n* * * adding 1 function, reutrning single f\n\n" << std::endl; );
	return fs->Steal( 1 );
      }

    //    tFunction0<TYPE> *res = fs->Steal( 1 );
    tAdd<TYPE> *res = new tAdd<TYPE>();


    while ( fs->GetSize() > 0 )
      {
#ifdef _SAFE
	if ( fs->Get( 1 ) == 0L )
	  _DBTHROW( "fs->Get( 1 ) == 0L" );
#endif	
	res->AddTerm( fs->Steal( 1 ) );
      }

    
    return res;
  }


  template<class TYPE> 
  tFunction0<TYPE> *tFunction0<TYPE>::Mul( tFunction0<TYPE> *f1 , tFunction0<TYPE> *f2 , tFunction0<TYPE> *f3 , ... )
  {

    tFunctions<TYPE> *tmpfs = new tFunctions<TYPE>();

    if ( f1 != 0L )
      tmpfs->AddFunction( f1 );

    if ( f2 != 0L )
      tmpfs->AddFunction( f2 );

    if ( f3 != 0L )
    {
      tmpfs->AddFunction( f3 );
      
      
      va_list arglist;
      va_start(arglist,f3);
      
      
      tFunction0<TYPE> *tmp;
      for ( ;; )
	{
	  tmp = va_arg(arglist, tFunction0<TYPE> * );
	  if ( tmp == 0L )
	    break;
	  
	  tmpfs->AddFunction( tmp );
	}
      va_end( arglist );
    }
    if( tmpfs->GetSize() < 1 )
      _DBTHROW( "tmpfs.GetSize() < 1" );

    return Mul( tmpfs );
  }

  template<class TYPE> 
  tFunction0<TYPE> *tFunction0<TYPE>::Mul( tFunctions<TYPE> *fs  )
  {
    if ( fs->GetSize() < 1 )
      return 0L;

    //    tFunction0<TYPE> *res = fs->Steal( 1 );
    tMul<TYPE> *res = new tMul<TYPE>();


    const tConstant<TYPE> *lconst;
    while ( fs->GetSize() > 0 )
      {
#ifdef _SAFE
	if ( fs->Get( 1 ) == 0L )
	  _DBTHROW( "fs->Get( 1 ) == 0L" );
#endif	
	
	lconst = dynamic_cast<const tConstant<TYPE> *>( fs->Get( 1 ) );
	if ( lconst != NULL )
	  {
	    if ( (TYPE)(*lconst) == (TYPE)0.0 )
	      {
		fs->Free();
		delete fs;
		delete res;
		return new tConstant<TYPE>( 0.0 );
	      }
	    if ( (TYPE)(*lconst) == 1.0 ) 
	      delete fs->Steal( 1 );
	    else
	      res->AddTerm( fs->Steal( 1 ) );
	  } else
	  res->AddTerm( fs->Steal( 1 ) );
      }
    
    fs->Free();
    delete fs;

    return res;
  }



  template<class TYPE> 
  tnmmatrix<TYPE> &tFunction0<TYPE>::Ndydx( const tnmmatrix<TYPE> &dx , tnmmatrix<TYPE> &gradient , const tnmmatrix<TYPE> &x )
  {
    //    _DB( std::cout << "Ndydx. dim = " << dim << std::endl; );
    tnmmatrix<TYPE> xtmp( x  );
    TYPE dy;
    TYPE _dx;
    for ( int i = 1 ; i <= x.GetRows() ; i++ )
      {
	_dx = dx.GetRepeated( i , 1 );
	xtmp(i,1) += _dx;
	this->OnChangex( xtmp );
	
	dy = y( xtmp );
	xtmp(i,1) -= 2 * _dx;
	this->OnChangex( xtmp );
	dy -= y( xtmp );
	
	xtmp(i,1) += 2 * _dx;
	this->OnChangex( xtmp ); 
	//	std::cout << "dy = " << dy << std::endl;
	gradient(i,1) = dy / ( _dx + _dx );
      }
    return gradient;
  }
  
  template<class TYPE> 
  tnmmatrix<TYPE> &tFunction0<TYPE>::Nd2ydx2( const tnmmatrix<TYPE> &dx2 , 
					      tnmmatrix<TYPE> &hessian , const tnmmatrix<TYPE> &x )
  {
    //    _DB( std::cout << "Nd2ydx2" << std::endl; );
    TYPE ddy;
    tnmmatrix<TYPE> xtmp( x );
    TYPE e;
    TYPE ex2;
    for ( int i = 1 ; i <= x.GetRows() ; i++ )
      for ( int j = 1 ; j <= x.GetRows() ; j++ )
	{
	  e = dx2.GetRepeated( i , j );
	  ex2 = e + e;
	  xtmp(i,1) += e;
	  xtmp(j,1) += e;
	  this->OnChangex( xtmp );
	  ddy = y( xtmp );
	  
	  xtmp(i,1) -= ex2;
	  this->OnChangex( xtmp );
	  ddy -= y( xtmp );
	  
	  xtmp(j,1) -= ex2;
	  this->OnChangex( xtmp );
	  ddy += y( xtmp );
	  
	  xtmp(i,1) += ex2;
	  this->OnChangex( xtmp );
	  ddy -= y( xtmp );
	  
	  xtmp(i,1) -= e;
	  xtmp(j,1) += e;
	  this->OnChangex( xtmp );
	  
	  hessian(i,j) = ddy / ( 4 * e * e ); 

	}
    return hessian;
  }


  template<class TYPE> 
  int tFunction0<TYPE>::CheckGradient( const tnmmatrix<TYPE> &dx , const tnmmatrix<TYPE> &x )
  {
    tnmmatrix<TYPE> ana( x.GetRows() , 1 );
    tnmmatrix<TYPE> num( x.GetRows() , 1 );

    this->dydx( tnmmatrix<TYPE>( 1,1,0.0) , ana , x );
    tFunction0<TYPE>::Ndydx( dx , num , x );

    ana.PrintTrp( std::cout , "Analytical: \t" , 120 );
    num.PrintTrp( std::cout , "Numerical: \t" , 120 );
    ana.Sub( num );
    for ( int i = 1 ; i <= x.GetRows() ; i++ )
      ana(i,1) /= ( sqrt( num(i,1) * num(i,1) ) + tFunction0<TYPE>::_EPSILON );

    ana.PrintTrp( std::cout , "% Difference: \t" , 120 );
    return 1;
  }
		    
  template<class TYPE> 
  int tFunction0<TYPE>::CheckHessian( const tnmmatrix<TYPE> &dx2 , 
				      const tnmmatrix<TYPE> &x )
  {
    tnmmatrix<TYPE> ana( x.GetRows() , x.GetRows() );
    tnmmatrix<TYPE> num( x.GetRows() , x.GetRows() );
    
    this->d2ydx2( tnmmatrix<TYPE>( 1,1,0.0 ) , tnmmatrix<TYPE>( 1, 1 , 0.0 ) , ana , x );
    tFunction0<TYPE>::Nd2ydx2( dx2 , num , x );
    
    ana.Print( std::cout , "Analytical: \t" , 120 );
    num.Print( std::cout , "Numerical: \t" , 120 );
    ana.Sub( num );
    for ( int i = 1 ; i <= x.GetRows() ; i++ )
      for ( int j = 1 ; j <= x.GetRows() ; j++ )
	ana(i,j) /= ( sqrt( num(i,j) * num(i,j) ) + tFunction0<TYPE>::_EPSILON );
    
    ana.Print( std::cout , "% Difference: \t" , 120 );
    return 1;
  }

  template<class TYPE> 
  tfMatrix<TYPE> *tFunction0<TYPE>::Gradient( TYPE dx , int dim ) const
  {
    tfMatrix<TYPE> *res = new tfMatrix<TYPE>( dim , 1 );
    for ( int i = 1 ; i <= dim ; i++ )
      res->Set(i,1, this->Getdydx( dx , i , -1 )->Clone() );
    return res;
  }
  
  template<class TYPE> 
  tfMatrix<TYPE> *tFunction0<TYPE>::Hessian( TYPE dx1 , TYPE dx2 , int dim ) const
  {
    tfMatrix<TYPE> *res = new tfMatrix<TYPE>( dim , dim );
    const tFunction0<TYPE> *grad = 0L;
    for ( int i = 1 ; i <= dim ; i++ )
      {
	grad = this->Getdydx( dx1 , i , -1 );
	for ( int j = 1 ; j <= dim ; j++ )
	  res->Set(i,j, grad->Getdydx( dx2 , j , -1 )->Clone() );
      }
    return res;
  }
  
  
  template class tFunction0<float>;
  template class tFunction0<double>;

  template class LinkedPtrList<float *>;
  template class LinkedPtrList<double *>;

 
}


