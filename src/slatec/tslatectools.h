/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#ifndef _P4TH_TSLATECTOOLS_H_
#define _P4TH_TSLATECTOOLS_H_

#include <algorithm>
#include <string>
#include <limits>

#include <texception.h>
#include <time.h>
#include <linkedptrlist.h>
#include "tslatecstack.h"



//class istream;
//class ::std::ostream;

// Do not include <f2c.h> beacuse it defines min, max globally which mess up alot ...

#include <f2c.h>
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif


namespace P4th
{


  template<class TYPE> 
    class tSlatecTools
    {
    protected:
      static const int LONGWORDSIZE = 8;
      static tSlatecStack<double (*)(double * , void *)> *integrandfunction;
      static tSlatecStack<void *> *integranddata;
      static double Integrand0( double *x );
	    
    public:
      // Misc
      static integer I1MACH( integer nr );
      // Matrix Inverse, condition Number, and Determinant
      static void GECO( doublereal *A ,  integer LDA , integer N , integer *ipv , TYPE *rcond = NULL );
      static void GEDI( doublereal *A ,  integer LDA , integer N , integer *ipv , TYPE *det = NULL );

      static void Inverse( doublereal *A ,  integer LDA , integer N , TYPE *det = NULL , TYPE *rcond = NULL );
      static TYPE Determinant( doublereal *A ,  integer LDA , integer N , TYPE *rcond = NULL );

      
      
      // Integration
      static TYPE QNG( double (*f)( double * , void * ) , void * , TYPE a , TYPE b , TYPE epsabs , TYPE epsrel , 
		       TYPE &abserr , integer &neval , integer &ier  ); 
      static TYPE QNG( double (*f)( double * , void * ) , void * , TYPE a , TYPE b , TYPE epsabs , TYPE epsrel , 
		       TYPE &abserr ); 
      
      static TYPE QAG( double (*f)( double * , void * ) , void * , TYPE a , TYPE b , 
		       TYPE epsabs , TYPE epsrel , 
		       integer key ,  
		       TYPE &abserr , integer &neval , integer &ier , integer limit , integer &last ); 

      static TYPE QAGI( double (*f)( double * , void * ) , void * , TYPE bound , integer inf , TYPE epsabs , TYPE epsrel  ,  
			TYPE &abserr , integer &neval , integer &ier , integer limit , integer &last ); 

      static TYPE QAG( double (*f)( double * , void *) , void * , TYPE a , TYPE b , 
		       TYPE epsabs , TYPE epsrel , integer limit , 
		       TYPE &abserr , integer key = 0 ); 
    };



}



#endif
