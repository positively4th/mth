#pragma once

#include <misctypes.h>
#include <coretypes.h>
#include <solvertypes.h>
#include <fittypes.h>

#include <testimator0.h>

#include <string.h>

#include <texception.h>
#include <tnsolver.h>
#include <tadd.h>
#include <tparameters.h>
#include <tselector.h>

#include <memory>


/*

Fit the function f(x_i;z) to y_i by selecting z where:

x_i is the ith observation vector with M elements 
z is the parameter vector with P elements
y_i is the ith outcome scalar

the vector function g is used to replace x_i with the 
observed values as:

g_i(z) = (x_i1 x_i2 ... z1 z2 ...)

Given h_i(z) = f(g_i(z)) - y_i

The problem is then to solve:

z* = argmin(z) sum(h_i(z)^2)

*/

using namespace std;

namespace P4th
{

  namespace Fit {

    namespace FitLS {

    template<class T> 
      class tTerm : public tDerivatives0<T> {
    public:
      MTH_MISC_TYPES(T);
      MTH_CORE_TYPES(T);
      typedef shared_ptr<vector<T > > $_rowPar;
      typedef unique_ptr<vector<T > > $$_rowPar;
      typedef tTerm<T> _term;
    protected:
      int row;
      $_rowPar xRow;
      $_rowPar yRow;
      $_m XMat;
      $_m yCol;
      $_f term;
    public:
      tTerm($$_f _term, $_m _XMat, $_m _yCol, $$_rowPar _xRow, $$_rowPar _yRow);
      tTerm(const tTerm<T> &source);

      virtual tTerm<T> *Clone() const 
      { return new tTerm(*this); }
      virtual int Nodes() const {
	return term->Nodes();
      };
      virtual bool DimOk( int d ) const {
	return this->term->DimOk(d);
      }

      void SetRow(int r) 
      { this->row = r; }
      int GetRow()  const
      { return this->row; }
      virtual T y( const tnmmatrix<T> &z ) const;
      virtual tTerm *Reduce( int argnr , _f *f  ) const {
	return new tTerm<T>( $$_f(this->term->Reduce(argnr, f)), this->XMat, this->yCol, 
			     $$_rowPar(new std::vector<T>(*this->xRow)), 
			     $$_rowPar(new vector<T>(*this->yRow)));
      }
      virtual tTerm<T> *AnalyticalDerivative(int argnr) const;
      virtual std::ostream &Dump( std::ostream &dest , string lm = "" ) const;
    
    };

    template<class T> 
      class tSum : public tDerivatives0<T> {

      MTH_MISC_TYPES(T);
      MTH_CORE_TYPES(T);
      typedef tTerm<T> _term;
      typedef shared_ptr<_term> $_term;
      typedef unique_ptr<_term> $$_term;
    protected:
      unique_ptr<_term> term;
      $_m _X;
      $_m _y;
    public:
      tSum($$_term term, $_m X , $_m y);
      
      virtual tSum<T> *Clone() const {
	return new tSum<T>( unique_ptr<_term>(this->term->Clone()), this->_X , this->_y ); }
      virtual int Nodes() const {
	return this->_X->GetRows();
      };
      virtual bool DimOk( int d ) const {
	return this->term->DimOk(d);
      }

      virtual tSum<T> *AnalyticalDerivative( int argnr ) const 
      { return new tSum<T>( unique_ptr<_term>(this->term->AnalyticalDerivative(argnr)), this->_X , this->_y ); }
      
      virtual T y( const tnmmatrix<T> &z ) const;

      virtual tSum *Reduce( int argnr , _f *f  ) const {
	return new tSum<T>( unique_ptr<_term>(this->term->Reduce(argnr, f)), this->_X , this->_y);
      }

      virtual std::ostream &Dump( std::ostream &dest , string lm = "" ) const;
    
    };
  }

  template<class T> 
    class tFitLS : public tEstimator0<T> {

    MTH_MISC_TYPES(T);
    MTH_CORE_TYPES(T);
    MTH_SOLVER_TYPES(T);
    MTH_FIT_TYPES(T);
  protected:
    $options options;
    $_m B;
    $_fs predictors;

    $$_f f;
    $$_f objective;

    void ResetOptions();
    void InitObjective();
    void InitFOCs();
    int GetM();
    virtual void Estimate();
    $_nsolver GetSolver() 
      { return $_nsolverOpt::read(options.get(), "solver"); };
    
  public:
    tFitLS<T>( tFit<T> *owner, $$_f af);
    virtual ~tFitLS<T>();

    $options GetOptions() 
      { return this->options; }

    //tEstimator must define these....
    virtual $_m GetB();
    virtual $_fs GetPredictors(); 
    


  };

  }
}
