#pragma once

#include <misctypes.h>
#include <coretypes.h>
#include <fittypes.h>


#include <string>
#include <vector>
#include <map>
#include <memory>
#include <texception.h>
#include <tnmmatrix.h>
#include <options.h>
#include <toption.h>
#include <tptroption.h>

#include <twasher0.h>
#include <testimator0.h>


/* 

N number of observations
M number of explanatories
K number of dependent 
P number of parameters (M=P for Linear Regressions!)
*/
using namespace std;


namespace P4th
{
 
  using namespace Fit;

  template<class TYPE> 
    class tFit 
  {
  public:
    MTH_MISC_TYPES(TYPE);
    MTH_CORE_TYPES(TYPE);
    MTH_FIT_TYPES(TYPE);
  public:
  protected:
    int M;
    int K;
    int P;

    //These are generics that can be computed by this class
    $_m mu;
    $_m sigma;
    $_m rho;

    $_m EY;
    $_m SST; //1xK
    $_m SSE; //1xK
    $_m MST; //1xK
    $_m MSE; //1xK
    $_m R2,R2Adj; //1xK

    //These are set by the Washer composite
    $_m X;
    $_m Y;
    $string regressorMask;
    $string observationMask;

    //These are set by the concrete child estimator class
    $_fs predictors;
    $_m  B; 
    $_m  e;
    $_m  s2;
    $_m  predictions;

    $options options;
    _mVec xs;
    _mVec ys;
    _strVec ynames;
    _strVec xnames;

    void Estimate();
    $_washer0 GetWasher() 
      { return $_washerOpt::read(GetOptions().get(), "washer"); };
    $_estimator0 GetEstimator() 
      { return $_estimatorOpt::read(GetOptions().get(), "estimator"); };

  public:
    tFit( /* X columns */ int _M , /* Y columns */ int _K , /* Betas/Params/z */ int _P = -1);

    void ResetOptions();
    $options GetOptions() 
    { return this->options; }
    int GetN() const
    { return xs.size(); }
    int GetM() const
    { return M; }
    int GetK() const
    { return K; }
    int GetP() const
    { return P; }
    int GetRegressors() 
    { return GetX()->GetRows(); }
    map<string,double> Getb( int k );

    virtual _m GetPredictions();

    void AddObservation( TYPE y , const _m &xrow ) {
      AddObservation( _m( 1 , 1 , y ) , xrow );
    }
    virtual void AddObservation( const _m &Y , const _m &X );

    void AddyNames( const string name );
    //Todo: Rename as AddParameterName
    virtual void AddxName( string names );
    //Todo: Rename as AddParametersName
    virtual void AddxNames( string names ); // separated by ","
    string GetxName( int nr ) const
    { 
      if ( nr < 0 ) throw aException(tFit<TYPE>, "GetxName( int nr ) : nr < 0" ); 
      if ( nr >= xnames.size() ) return ""; 
      return xnames[nr];
    }
    string GetyName( int nr ) const
    { 
      if ( nr < 0 ) throw aException(tFit<TYPE>, "GetyName( int nr ) : nr < 0" ); 
      if ( nr >= ynames.size() ) return ""; 
      return ynames[nr];
    }
    _m xMean( const _m &mu ) const
      { 
	_m xMean( 1 , GetM() );
	_m::Get( xMean , mu , 1 , 1 );
	return xMean;
      }


    //Simple statistics of [Y,X] (for unwashed data)
    $_m Getmu();
    $_m Getsigma();
    $_m Getrho();

    //Independent variables (washed data)
    $_m GetX();
    //Dependent variables (washed data)
    $_m GetY();
    $string GetObservationMask();
    $string GetRegressorMask();

    //Estimated parameters (for unwashed data) 
    $_m GetB();

    //Summary statistics (for washed data)
    $_m GetEY();
    $_m GetSSE();
    $_m GetSST();
    $_m GetMSE();
    $_m GetMST();
    $_m GetR2();
    $_m GetR2Adj();
    //Get predictor functions
    $_fs GetPredictors(); 
    $_f GetPredictor(int ith = 1); 


    virtual std::ostream &PrintObservations( std::ostream &dest , string lm = "" , int width = 10 );
    virtual std::ostream &PrintMu( const _m &mu , std::ostream &dest , string lm , int width );
    virtual std::ostream &PrintEstimate( std::ostream &dest , string lm = "" , int width = 10 );
    virtual std::ostream &PrintSummary( std::ostream &dest , string lm = "" , int width = 10 );

    
    //Abstract washer class defining an inerface for washer class implementations
    friend class tWasher0<TYPE>;
    friend class tEstimator0<TYPE>;
    
  };


}

