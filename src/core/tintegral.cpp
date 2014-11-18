/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#include "tintegral.h"

#include <debug.h>
#include <string>
#include <math.h>
#include <tconstant.h>
#include <tmathtools.h>
#include <tnmmatrix.h>
#include <tfog.h>
#include <tselector.h>
#include <tslatecerror.h>

//#include <gsl/gsl_integration.h>
//#include <gsl/gsl_errno.h>


#include <tslatectools.h>



namespace P4th
{

  template<class TYPE> 
  tIntegral<TYPE>::tIntegral( tFunction0<TYPE> *foy , 
			      tFunction0<TYPE> *_lb , 
			      tFunction0<TYPE> *_ub , 
			      int maxints  , 
			      TYPE maxabserr  , 
			      TYPE maxrelerr 
			      ) :
    tDerivatives0<TYPE>()  
  {
    if ( foy == NULL )
      _DBTHROW( "foy == NULL" );
    lb = _lb;
    ub = _ub;
    f = foy;

    if ( maxrelerr <= 0 )
      maxrelativeerror = _MAXRELATIVEERROR;
    else
      maxrelativeerror = maxrelerr;
    
    if ( maxabserr <= 0 )
      maxabsoluteerror = _MAXABSOLUTEERROR;
    else
      maxabsoluteerror = maxabserr;
    
    if ( maxints <= 0 )
      maxintervals = _MAXINTERVALS;
    else
      maxintervals = maxints;

    activex = new tnmmatrix<TYPE>( 0 ,0 );

  }



  template<class TYPE> 
  tIntegral<TYPE>::tIntegral( const tnmmatrix<int> &mapping , 
			      tFunction0<TYPE> *_f ,  
			      tFunction0<TYPE> *_lb , 
			      tFunction0<TYPE> *_ub , 
			      int maxints , 
			      TYPE maxabserr , 
			      TYPE maxrelerr ) :
    tDerivatives0<TYPE>()  
  {
    //    argnr = anr;


      f = tFunction0<TYPE>::Composite( f , (tFunction0<TYPE> *)NULL );

      tFoG<TYPE> *tfog = dynamic_cast<tFoG<TYPE> *>( f );
      if ( tfog != NULL )
	for ( int i = 1 ; i <= mapping.GetRows() ; i++ )
	  if ( mapping.Get(i,1) != _MAGICSELECTOR )
	    tfog->AddFunction( new tSelector<TYPE>( mapping.Get(i,1) ) );
	  else
	    tfog->AddFunction( new tIntegralSelector<TYPE> );
      
      lb = _lb;
      ub = _ub;
      activex = new tnmmatrix<TYPE>( 0 ,0 );
      
      if ( maxrelerr <= 0 )
	maxrelativeerror = _MAXRELATIVEERROR;
      else
	maxrelativeerror = maxrelerr;
      
    if ( maxabserr <= 0 )
      maxabsoluteerror = _MAXABSOLUTEERROR;
    else
      maxabsoluteerror = maxabserr;
    
    if ( maxints <= 0 )
      maxintervals = _MAXINTERVALS;
    else
      maxintervals = maxints;

  }

  template<class TYPE> 
  tIntegral<TYPE>::tIntegral( const tnmmatrix<int> &mapping , 
			      tFunction0<TYPE> *_f , TYPE _lb , TYPE _ub , 
			      int maxints , 
			      TYPE maxabserr , 
			      TYPE maxrelerr ) :
    tDerivatives0<TYPE>() 
  {
    //    argnr = anr;

      f = tFunction0<TYPE>::Composite( f , (tFunction0<TYPE> *)NULL );

      tFoG<TYPE> *tfog = dynamic_cast<tFoG<TYPE> *>( f );
      if ( tfog != NULL )
	for ( int i = 1 ; i <= mapping.GetRows() ; i++ )
	  if ( mapping.Get(i,1) != _MAGICSELECTOR )
	    tfog->AddFunction( new tSelector<TYPE>( mapping.Get(i,1) ) );
	  else
	    tfog->AddFunction( new tIntegralSelector<TYPE> );
      
    
    lb = new tConstant<TYPE>( _lb );
    ub = new tConstant<TYPE>( _ub );
    activex = new tnmmatrix<TYPE>( 0 ,0 );

    if ( maxrelerr <= 0 )
      maxrelativeerror = _MAXRELATIVEERROR;
    else
      maxrelativeerror = maxrelerr;
    
    if ( maxabserr <= 0 )
      maxabsoluteerror = _MAXABSOLUTEERROR;
    else
      maxabsoluteerror = maxabserr;
    
    if ( maxints <= 0 )
      maxintervals = _MAXINTERVALS;
    else
      maxintervals = maxints;

  }

  template<class TYPE> 
  tIntegral<TYPE>::tIntegral( const tIntegral<TYPE> &source ) :
    tDerivatives0<TYPE>( source ) 
  {
    //    _DB( std::cout << "tIntegral<TYPE>::tIntegral( const tIntegral &source )" << std::endl; );
    //    argnr = source.argnr;
    f = source.f->Clone();
    lb = source.lb->Clone();
    ub = source.ub->Clone();
    activex = new tnmmatrix<TYPE>( (*source.activex) );

    maxrelativeerror = source.maxrelativeerror;
    maxabsoluteerror = source.maxabsoluteerror;
    maxintervals = source.maxintervals;
  }

  template<class TYPE> 
  tIntegral<TYPE>::~tIntegral()
  {
    if ( f != 0L )
      delete f;
    if ( lb != 0L )
      delete lb;

    if ( ub != 0L )
      delete ub;

    if ( activex != 0L )
      delete activex;
  }

  template<class TYPE> 
  tFunction0<TYPE> *tIntegral<TYPE>::Reduce( int rargnr , tFunction0<TYPE> *s  ) const
  { 

    //    _DB( std::cout << "tIntegral<TYPE> *tIntegral<TYPE>::Reduce( int rargnr , tFunction0<TYPE> *s  ) const" << std::endl; );
    
    Heap<tFunction0<TYPE> >h;
    h += s->Clone();
    h += s->Clone();

    tFunction0<TYPE> *fred = this->f->Reduce( rargnr , h.Pop() );

    return new tIntegral<TYPE>( fred  , 
				this->lb->Reduce( rargnr , h.Pop() )  ,
				this->ub->Reduce( rargnr , s ) , 
				maxintervals , 
				maxabsoluteerror , 
				maxrelativeerror 
				);
      

    
  }

  template<class TYPE> 
  tFunction0<TYPE> *tIntegral<TYPE>::AnalyticalDerivative( int _argnr ) const
  {
    //    _DB( std::cout << "tFunction0<TYPE> *tIntegral<TYPE>::AnalyticalDerivative( int _argnr ) const" << std::endl; );
    Heap<tFunction0<TYPE> > h;
    tFunction0<TYPE> *A = 0L;
    tFunction0<TYPE> *B = 0L;
    tFunction0<TYPE> *C = 0L;
    
    // derivative under integral sign
    A = new tIntegral<TYPE>( this->f->Getdydx( 0.0 , _argnr , -1 )->Clone() ,  
			     lb->Clone() , ub->Clone() , 
			     maxintervals , 
			     maxabsoluteerror , 
			     maxrelativeerror  
			     );
    //    _DB( A->Dump( std::cout , "A: \t" ) );
	
    // Upper bound
    //    _DB( std::cout << "Derivative " << argnr << " upper bound." << std::endl; );
    B = tFunction0<TYPE>::Mul( ub->Getdydx( _argnr )->Clone() , 
			       f->Reduce( tIntegralSelector<TYPE>::_REDUCENR , 
					  ub->Clone() )  );
    //    _DB( B->Dump( std::cout , "B: \t" ) );

    //    B = new tConstant<TYPE>( 0 );
    // Lower bound
    //    _DB( std::cout << "Derivative " << argnr << " lower bound." << std::endl; );
    //    C = new tConstant<TYPE>( 0 );
    C = tFunction0<TYPE>::Mul( lb->Getdydx( _argnr )->Clone() , 
			       f->Reduce( tIntegralSelector<TYPE>::_REDUCENR , 
					  lb->Clone() )    );
    //    _DB( C->Dump( std::cout , "C: \t" ) );
    
    
    return tFunction0<TYPE>::Add( A , B , tFunction0<TYPE>::Negate( C ) , 0L );
    
  }
  
  template<class TYPE> 
  TYPE tIntegral<TYPE>::y( const tnmmatrix<TYPE> &x  ) const
  {
    //    _DB( x.PrintTrp( std::cout , "x: \t" ) );
    TYPE res; 
#ifdef _SAFE
    if ( ! DimOk( x.GetRows() ) )
      {
	_DBTHROW( "x dimension not ok." ); 
      }
#endif
    //    _DB( std::cout << "x.GetRows() = " << x.GetRows() << std::endl; );
 
   int actdim = x.GetRows() + 1;


#ifdef _SAFE
    if ( ! f->DimOk( actdim ) )
      _DBTHROW( "! f->DimOk( actdim )" ); 
#endif

    //    this->Dump( std::cout , "* * * " );

    activex->Resize( actdim , 1 );
    for ( int i = 1 ; i <= x.GetRows()  ; i++ )
      activex->Set( i , 1 , x.Get(i,1) );
    
    //    _DB( activex->PrintTrp( std::cout , "x*: \t" ) );

    /*
    _DB( f->Dump( std::cout , "f:\t " ); ); 
    _DB( lb->Dump( std::cout , "lb:\t " ); ); 
    _DB( ub->Dump( std::cout , "ub:\t " ); );
    */

    TYPE lb0,ub0;
    lb0 = lb->y( x );
    ub0 = ub->y( x );

    //    _DB( std::cout << "*lb = " << lb0 << ", *ub = " << ub0 << std::endl; );


    if ( tMathTools<TYPE>::Abs( ub0 - lb0 ) < tFunction0<TYPE>::Epsilon() )
      return 0;
    TYPE abserr;



    // Since throwing inside a catch invokes terminate(), store error message, then throw error 
    string error = "";
    try {
      res = tSlatecTools<TYPE>::QAG( integrand , (void *)this , 
				     (TYPE)lb0 , (TYPE)ub0 , 
				     (TYPE)maxabsoluteerror, 
				     (TYPE)maxrelativeerror, 
				     maxintervals , 
				     abserr ); 
    } 
    catch ( tSlatecError<TYPE> sle )
      {
	error = "Error computing integral.";
	//	sle.Print( std::cerr );
      }
    catch ( ... )
      {
	error = "Error computing integral.";
      }

    if ( error != (string)"" )
      throw aException( tIntegral<TYPE>, error );
    
    return res;
    
  }

  template<class TYPE> 
  std::ostream &tIntegral<TYPE>::Dump( std::ostream &dest , string lm ) const
  {
    dest << lm << __FILE__ << "(" << (void *)this << ")" << std::endl; 
    lm = lm + "\t";
    //    tDerivatives0<TYPE>::Dump( dest , lm );
    //    dest << lm << "argnr = " << argnr << std::endl;
    dest << lm << "f:" << (void *)f << std::endl;
    f->Dump( dest , lm + "\t" );
    dest << lm << "lb:" << (void *)lb << std::endl;
    lb->Dump( dest , lm + "\t" );
    dest << lm << "ub:" << (void *)ub << std::endl;
    ub->Dump( dest , lm + "\t" );
    return dest;
  } 

  
}

template class P4th::tIntegral<float>;
template class P4th::tIntegral<double>;
template class P4th::tIntegralSelector<float>;
template class P4th::tIntegralSelector<double>;



