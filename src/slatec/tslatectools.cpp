/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#include "tslatectools.h"

#include <debug.h>
#include <math.h>
#include <string>
#include <stlstringtools.h>
#include "tslatecerror.h"



//#include "src/cslatec.P"
#include "src/cslatec.h"


namespace P4th
{

  template<class TYPE> 
  tSlatecStack<double (*)(double * , void *)> *tSlatecTools<TYPE>::integrandfunction = NULL;

  template<class TYPE> 
  tSlatecStack<void *> *tSlatecTools<TYPE>::integranddata = NULL;


  template<class TYPE> 
  integer tSlatecTools<TYPE>::I1MACH( integer nr )
  {
    integer test;
    test = (integer)nr;
    integer res = i1mach_( &test );
    return res;
  }

  template<class TYPE> 
  void tSlatecTools<TYPE>::GECO( doublereal *A ,  integer LDA , integer N  , integer *ipv , TYPE *rcond )
  {
    /* LDA -- rows */
    /* N -- columns */

    doublereal _rcond;
    doublereal z[N];
    
    
    dgeco_( A , &LDA , &N , ipv , &_rcond , z );

    if ( rcond != NULL )
      (*rcond) = _rcond;

  }

  template<class TYPE> 
  void tSlatecTools<TYPE>::GEDI( doublereal *A ,  integer LDA , integer N , integer *ipv , TYPE *det )
  {

    doublereal work[N];
    integer pivots[N];
    integer job = 1;
    if ( det != NULL )
      job = 11;
    doublereal _det[2];
    
    dgedi_( A , &LDA , &N , ipv ,  _det , work , &job );

    if ( det != NULL )
      (*det) = (TYPE)(_det[0]*pow(10,_det[1]));
    
  }

  template<class TYPE> 
  double tSlatecTools<TYPE>::Integrand0( double *x ) 
  { 
    double (*tmp)(double * , void *);
    tmp = tSlatecStack<double (*)(double * , void *)>::Peek( integrandfunction );
#ifdef _SAFE
    if ( tmp == NULL )
      throw tException<tSlatecTools<TYPE> >("tStackTools::integrandfunction stack is empty.");
#endif
    /*
      _DB( std::cout << "&f = " << (void *)tmp << std::endl; );
      _DB( std::cout << "&d = " << (void *)tSlatecStack<void *>::Peek( integranddata ) << std::endl; );
    */
    return tmp( x , tSlatecStack<void *>::Peek( integranddata ) ); 
  }
  
  template<class TYPE> 
  void tSlatecTools<TYPE>::Inverse( doublereal *A ,  integer LDA , integer N , TYPE *det , TYPE *rcond )
  {
    integer ipv[N];

    GECO( A , LDA , N , ipv , rcond );
    GEDI( A , LDA , N , ipv , det );

  }

  template<class TYPE> 
  TYPE tSlatecTools<TYPE>::Determinant( doublereal *A ,  integer LDA , integer N , TYPE *rcond )
  {
    integer ipv[N];

    GECO( A , LDA , N , ipv , rcond );

    doublereal work[N];
    integer pivots[N];
    integer job = 10;
    doublereal _det[2];

    dgedi_( A , &LDA , &N , ipv ,  _det , work , &job );

    return (TYPE)(_det[0]*pow(10,_det[1]));
  }

  template<class TYPE> 
  TYPE tSlatecTools<TYPE>::QNG( double (*f)( double * , void * ) , void *data , TYPE a , TYPE b , TYPE epsabs , TYPE epsrel , 
				TYPE &abserr , integer &neval , integer &ier  )
  {
    //    _DB( std::cout << "eps abs = " << epsabs << ", eps rel = " << epsrel << std::endl; );
    doublereal _result;
    doublereal _abserr;
    tSlatecStack<double (*)(double * , void *)>::Push( &integrandfunction , f );
    tSlatecStack<void *>::Push( &integranddata , data );
    dqng_( (D_fp)Integrand0 , (doublereal *)&a ,  (doublereal *)&b , (doublereal *)&epsabs , (doublereal *)&epsrel , 
	   &_result , &_abserr , (integer *)&neval , (integer *)&ier  );
    tSlatecStack<double (*)(double * , void *)>::Pop( &integrandfunction );
    tSlatecStack<void *>::Pop( &integranddata );
    abserr = (TYPE)_abserr;
    //    _DB( std::cout << "intgerandfunction = " << (void *)integrandfunction << std::endl );
    //    _DB( std::cout << "intgeranddata = " << (void *)integranddata << std::endl << std::flush );

    if ( ier == 0 )
      return (TYPE)_result;

    /*
    if ( ier == 4 )
      {
	std::cerr << "Warning: DQNG estimated error (" << ier << ") is " << _abserr << "." << std::endl;
	return (TYPE)_result;
      }
    */

    throw _SLATECERROR( ier );

  }

  template<class TYPE> 
  TYPE tSlatecTools<TYPE>::QNG( double (*f)( double * , void * ) , void *data , TYPE a , TYPE b , TYPE epsabs , TYPE epsrel , 
				TYPE &abserr   )
  {
    //    _DB( std::cout << "eps abs = " << epsabs << ", eps rel = " << epsrel << std::endl; );
    integer neval;
    integer ier;
    return QNG( f , data , a , b , epsabs , epsrel , abserr  , neval , ier );

  }


  template<class TYPE> 
  TYPE tSlatecTools<TYPE>::QAG( double (*f)( double * , void * ) , void *data , TYPE a , TYPE b , TYPE epsabs , TYPE epsrel , integer key , 
			       TYPE &abserr , integer &neval , integer &ier , integer limit , integer &last )
  {
    //    _DB( std::cout << "eps abs = " << epsabs << ", eps rel = " << epsrel << ", limit = " << limit << std::endl; );
    //    _DB( std::cout << "QAG" << std::endl; );


    integer lenw = (integer)(limit * 4);
    //    integer lenw = (integer)(limit * sizeof(doublereal));
    integer iwork[limit];
    doublereal work[lenw];
    doublereal _result;
    doublereal _abserr;
    tSlatecStack<double (*)(double * , void *)>::Push( &integrandfunction , f );
    tSlatecStack<void *>::Push( &integranddata , data );

    /*
    _DB( std::cout << "limit = " << limit << std::endl; );
    _DB( std::cout << "lenw = " << lenw << std::endl; );
    */
    dqag_( (D_fp)Integrand0 , (doublereal *)&a ,  (doublereal *)&b , (doublereal *)&epsabs , 
	   (doublereal *)&epsrel , 
	   (integer *)&key , (doublereal *)&_result ,  (doublereal *)&_abserr  , (integer *)&neval , 
	   (integer *)&ier  , (integer *)&limit , (integer *)&lenw , (integer *)&last , (integer *)&iwork , 
	   (doublereal *)&work );
    tSlatecStack<double (*)(double * , void *)>::Pop( &integrandfunction );
    tSlatecStack<void *>::Pop( &integranddata );
    //    _DB( std::cout << "intgerandfunction = " << (void *)integrandfunction << std::endl );
    //    _DB( std::cout << "intgeranddata = " << (void *)integranddata << std::endl << std::flush );
    abserr = _abserr;

    if ( ier == 0 )
      return (TYPE)_result;

    /*
    if ( ier == 4 )
      {
	std::cerr << "Warning: DQAG estimated error (" << ier << ") is " << _abserr << "." << std::endl;
	return (TYPE)_result;
      }
    */

    throw _SLATECERROR( ier );
  }
  
  template<class TYPE> 
  TYPE tSlatecTools<TYPE>::QAGI( double (*f)( double *  , void *) , void *data , TYPE bound , integer inf , TYPE epsabs , TYPE epsrel  , 
			       TYPE &abserr , integer &neval , integer &ier , integer limit , integer &last )
  {
    //    _DB( std::cout << "eps abs = " << epsabs << ", eps rel = " << epsrel << ", limit = " << limit << std::endl; );
    //    _DB( std::cout << "QAGI " << inf << std::endl; );
    integer lenw = (integer)(limit * 4);
    integer iwork[limit];
    doublereal work[lenw];
    doublereal _result;
    doublereal _abserr;

    tSlatecStack<double (*)(double * , void *)>::Push( &integrandfunction , f );
    tSlatecStack<void *>::Push( &integranddata , data );
    dqagi_( (D_fp)Integrand0 , (doublereal *)&bound , (integer *)&inf , (doublereal *)&epsabs , (doublereal *)&epsrel , 
	   (doublereal *)&_result ,  (doublereal *)&_abserr  , (integer *)&neval , 
	   (integer *)&ier  , (integer *)&limit , (integer *)&lenw , (integer *)&last , (integer *)&iwork , 
	   (doublereal *)&work );
    //    _DB( std::cout << "result = " << _result << std::endl; );
    tSlatecStack<double (*)(double * , void *)>::Pop( &integrandfunction );
    tSlatecStack<void *>::Pop( &integranddata );
    //    _DB( std::cout << "intgerandfunction = " << (void *)integrandfunction << std::endl );
    //    _DB( std::cout << "intgeranddata = " << (void *)integranddata << std::endl << std::flush );
    abserr = _abserr;
    
    if ( ier == 0 )
      return (TYPE)_result;

    /*
    if ( ier == 4 )
      {
	std::cerr << "Warning: DQAGI estimated error (" << ier << ") is " << _abserr << "." << std::endl;
	return (TYPE)_result;
      }
    */


    throw _SLATECERROR( ier );
  }
  
  template<class TYPE> 
  TYPE tSlatecTools<TYPE>::QAG( double (*f)( double *  , void *) , void *data , TYPE a , TYPE b , 
				TYPE epsabs , TYPE epsrel , 
				integer limit , TYPE &abserr , integer key  )
  {
    //    _DB( std::cout << "eps abs = " << epsabs << ", eps rel = " << epsrel << ", limit = " << limit << std::endl; );
    
    if ( a > b )
      {
	return -QAG( f , data , b , a , epsabs , epsrel , limit , abserr , key );
      }
    integer ier;
    integer last;
    integer neval;
    
    //    if ( a <= (-1.0)/(0.0) )
    if ( a <= - std::numeric_limits<TYPE>::infinity() )
      {
	if ( b >= std::numeric_limits<TYPE>::infinity() )
	  return QAGI( f , data , 0 , 2 , epsabs , epsrel , abserr , neval , ier , limit , last );
	else
	  return QAGI( f , data , b , -1 , epsabs , epsrel , abserr , neval , ier , limit , last );
      } else {
      if ( b >= std::numeric_limits<TYPE>::infinity() )
	return QAGI( f , data , a , 1 , epsabs , epsrel , abserr , neval , ier , limit , last );
    }
    return QAG( f , data , a , b  , epsabs ,epsrel , key , abserr , neval , ier , limit , last );
  }


  template class tSlatecTools<int>;
  template class tSlatecTools<double>;
  template class tSlatecTools<float>;
}

