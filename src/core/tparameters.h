#ifndef _P4TH_TPARAMETERS_H_
#define _P4TH_TPARAMETERS_H_

#include "tfunction0.h"
#include "tfmatrix0.h"
#include "tnmmatrix.h"
#include "tparameter.h"
#include <iostream>

//class istream;
//class ::std::ostream;


namespace P4th
{

  template<class TYPE> 
    class tParameters : public tfMatrix<TYPE>
    {
    protected:
    public:
      tParameters( const TYPE *_v , int R = 1 , int C = 1  );
      tParameters( const tnmmatrix<TYPE> *_m );
      
      tParameters( const tParameters<TYPE> &source  );
      virtual ~tParameters()
	{};

      virtual tParameters *Clone() const
	{ return new tParameters<TYPE>( *this ); }

      virtual const tParameter<TYPE> *operator()( int i , int j = 1 ) const
      { return static_cast<const tParameter<TYPE> *>(tfMatrix<TYPE>::operator()(i,j)); }
      
      virtual bool InitDerivatives( int depth , int dim  );
      
      virtual std::ostream &Dump( std::ostream &dest , string lm = "" ) const
	{ 
	  dest << lm << "tParameters<TYPE>" << std::endl; 
	  lm += "\t";
	  dest << lm << this->GetRows() << " rows and "  << this->GetCols() << " columns." << std::endl;
	  //	  tfMatrix<TYPE>::Dump( dest , lm ); 
	  return dest;
	}

    
    
  };


}


#endif
