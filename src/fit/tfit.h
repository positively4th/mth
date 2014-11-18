#ifndef _P4TH_TREGRESSION_H_
#define _P4TH_TREGRESSION_H_

#include <misctypes.h>
#include <coretypes.h>
#include <fittypes.h>


#include <vector>
#include <map>
#include <memory>
#include <debug.h>
#include <twasher.h>
#include <tnmmatrix.h>
#include <string>
#include <options.h>
#include <toption.h>
#include <tptroption.h>

//class istream;
//class ::std::ostream;

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
    int iterations;

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
    shared_ptr<string> regressorMask;
    shared_ptr<string> observationMask;

    //These are set by the concrete child estimator class
    $_m  matrixM; 
    std::vector<$_m> bCovariances;
    $_fs predictors;

    $_m  B; 
    $_m  e;
    $_m  s2;

    $options options;

    

    vector<_m > xs;
    vector<_m > ys;
    

    vector<string> ynames;
    vector<string> xnames;

    void Validate( const string &str ) const
    {
      if ( ys.size() != xs.size() )
	throw str + (string)" : ys.size() != xs.size()";
    }

  public:
    tFit( /* X columns */ int _M , /* Y columns */ int _K , /* Betas/Params/z */ int _P = -1);

    virtual $_m EstimateB() = 0;
    virtual _m &yhat( _m &res , const  _m &xrow , const  _m &B ) = 0;
    virtual $_fs CreatePredictors() = 0; 

    void ResetOptions();
    $options GetOptions() 
    { return this->options; }
    $options GetWasherOptions() 
      { return $_washerOpt::read(GetOptions().get(), "washer")->GetOptions(); }
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

    virtual _m Getyhat( const _m &xrow )
    { _m tmp(1,GetK()); return this->yhat( tmp , xrow , *GetB() ); }
    
    virtual _m &yhat( _m &res , const  _m &xrow  )
    { return this->yhat( res , xrow , *GetB() ); }

    virtual TYPE Getyhat( const _m &xrow  , const _m &b )
    { _m tmp(1,GetK()); return this->yhat( tmp , xrow , b ).Get(1,1); }
    
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
      if ( nr < 0 ) _DBTHROW( "string tRegressio<TYPE>::GetxName( int nr ) : nr < 0" ); 
      if ( nr >= xnames.size() ) return ""; 
      return xnames[nr];
    }
    string GetyName( int nr ) const
    { 
      if ( nr < 0 ) _DBTHROW( "string tRegressio<TYPE>::GetyName( int nr ) : nr < 0" ); 
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
    shared_ptr<string> GetObservationMask();
    shared_ptr<string> GetRegressorMask();

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
    //Covariance matrix
    $_m GetBCovariance(int k = 0);
    //Get predictor functions
    $_fs GetPredictors(); 



    virtual std::ostream &PrintObservations( std::ostream &dest , string lm = "" , int width = 10 );
    virtual std::ostream &PrintMu( const _m &mu , std::ostream &dest , string lm , int width );
    virtual std::ostream &PrintEstimate( std::ostream &dest , string lm = "" , int width = 10 );
    virtual std::ostream &PrintSummary( std::ostream &dest , string lm = "" , int width = 10 );
    
    //Abstract washer class defining an inerface for washer class implementations
    friend class tWasher0<TYPE>;
    
  };


}


#endif
