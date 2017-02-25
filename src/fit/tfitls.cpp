#include "tfitls.h"

#include <debug.h>
#include <math.h>
#include <stlstringtools.h>
#include <stdlib.h>
#include <assert.h>
#include <iomanip>
#include <iostream>
#include <tmathtools.h>
#include <tfunctions.h>
#include <tselector.h>
#include <tconstant.h>
#include <tnsolver.h>
#include <tptroption.h>
#include <tderivatives0.h>
#include <texception.h>

namespace P4th
{

  namespace Fit {
    
    namespace FitLS {
      
      template<class T> 
      tTerm<T>::tTerm($$_f _term, $_m _XMat, $_m _yCol, $$_rowPar _xRow, $$_rowPar _yRow) 
      {
	this->XMat = _XMat;
	this->yCol = _yCol;
	this->xRow = std::move(_xRow);
	this->yRow = std::move(_yRow);
	this->term = std::move(_term);
      }

      template<class T> 
      tTerm<T>::tTerm(const tTerm<T> &source) {
	this->XMat = source.XMat;
	this->yCol = source.yCol;
	this->xRow = source.xRow;
	this->yRow = source.yRow;
	this->term = source.term;

      }

      template<class T> 
      T tTerm<T>::y( const tnmmatrix<T> &z ) const
      {
	int r = this->GetRow();
	for (int c = 1 ; c <= this->XMat->GetCols() ; c++ ) {
	  (*this->xRow)[c-1] = this->XMat->Get(r,c);
	}
	(*this->yRow)[0] = this->yCol->Get(r,1);
	//	std::cout << "y = " << this->yCol->Get(r,1) << std::endl;
	return term->y(z); 
      }

      template<class T> 
      tTerm<T> *tTerm<T>::AnalyticalDerivative(int argnr) const {
	const tDerivatives0<T> *tmp = dynamic_cast<const tDerivatives0<T> *>(term.get());
	if ( tmp == NULL) {
	  throw tException<tTerm<T> >("Term function does not have an analytical derivative.");
	}
	unique_ptr<_f> d(tmp->AnalyticalDerivative(argnr)->Clone());
	return new tTerm<T>(std::move(d), this->XMat, this->yCol, 
			    $$_rowPar(new vector<T>(*this->xRow)), 
			    $$_rowPar(new vector<T>(*this->yRow)));
      }
      
     

      template<class T> 
      std::ostream &tTerm<T>::Dump( std::ostream &dest , string lm) const
      {
	dest << lm << __FILE__ << "(" << (void *)this << ")" << std::endl; 
	lm += "\t";
	return term->Dump(dest, lm);
      }
      
      
      template class tTerm<float>;
      template class tTerm<double>;
      
    }
    
    namespace FitLS {
      
      template<class T> 
      tSum<T>::tSum(unique_ptr<_term> aterm, shared_ptr<_m> X , shared_ptr<_m> y) :
	term(std::move(aterm))
      {
	this->_X = X;
	this->_y = y;
      }
      
      
      template<class T> 
      T tSum<T>::y( const tnmmatrix<T> &z ) const
      {
	T res = 0;
	for (int r = 1 ; r <= this->_X->GetRows() ; r++ ) {
	  this->term->SetRow(r);
	  res += this->term->y(z);
	}
	return res;
      }
      
 
      template<class T> 
      std::ostream &tSum<T>::Dump( std::ostream &dest , string lm) const
      {
	dest << lm << __FILE__ << "(" << (void *)this << ")" << std::endl; 
	lm += "\t";
	return term->Dump(dest, lm);
      }
   
      template class tSum<float>;
      template class tSum<double>;
      
    }

    
    

    template<class T> 
    tFitLS<T>::tFitLS( tFit<T> *owner, $$_f af) :
      _estimator0(owner), options(new Options()), objective(new _add()), f(std::move(af)) 
    {
      this->ResetOptions();
    }
    
    template<class T> 
    tFitLS<T>::~tFitLS()
    {
    }
    
    template<class TYPE> 
    void tFitLS<TYPE>::ResetOptions() {
      $_nsolverOpt::set(this->options.get(), "solver" , $_nsolver(new _nsolver()));
      _boolOpt::set(this->options.get(), "_DB_PRINT_DET" , false);
      _boolOpt::set(this->options.get(), "_DB_PRINT_XTX" , false);
    }

    template<class TYPE> 
    int tFitLS<TYPE>::GetM() {
      return this->GetX()->GetCols();
    }
    
    template<class T>
    shared_ptr<tnmmatrix<T> > tFitLS<T>::GetB()
    {
      if (B) {
	return B;
      }

      this->Estimate();

      assert(B);
      return B;
    }

  template<class T>
  shared_ptr<tFunctions<T> > tFitLS<T>::GetPredictors()
  {
      if (predictors) {
	return predictors;
      }
      $_m B = GetB();
      $$_fs g(new _fs());
      for (int arg = 1 ; arg <= GetM() ; arg++) {
	g->AddFunction(new _arg(arg));
      }
      for (int par = 1 ; par <= this->GetP() ; par++) {
	g->AddFunction(new _c(B->Get(par,1)));
      }
      predictors = $_fs();
      predictors->AddFunction(_f::Composite(this->f->Clone(), g.release()));
      return predictors;
  }



  template<class T>
  void tFitLS<T>::InitObjective() 
  {
    /*

    typename Fit::FitLS::tTerm<T>::$$_rowPar xRow(new vector<T>);
    typename Fit::FitLS::tTerm<T>::$$_rowPar yRow(new vector<T>);
    
    
    $$_fs g_i(new _fs());
    for (int i = 1 ; i <= this->GetM() ; i++) {
      xRow->push_back((T)0);
      g_i->AddFunction(new _par(&xRow->back()));
    }
    for (int i = 1 ; i <= this->GetP() ; i++) {
      g_i->AddFunction(new _arg(i));
    }
    
    
    yRow->push_back((T)0);
    unique_ptr<FitLS::tTerm<T> > term(new Fit::FitLS::tTerm<T>($$_f(_f::Square(
									       _f::Sub(
										       _f::Composite(f.release(), g_i.release()),
										       new _par(&yRow->back()))) 
								    ), 
							       this->GetX(), this->GetY(), 
							       move(xRow),  move(yRow)));
    
    std::unique_ptr<FitLS::tSum<T> > add(new FitLS::tSum<T>(std::move(term), this->GetX(), this->GetY()));
    
    this->objective.reset(add.release());
    this->objective->Dump(std::cout, "Obj:\t ");
    */

    std::unique_ptr<tAdd<T> > add(new tAdd<T>());

    std::unique_ptr<tFunctions<T> > g_i(new tFunctions<T>());
    tSelector<T> *selector = NULL;
    tConstant<T> *constant = NULL;

    int row = 1;
    typename Fit::tEstimator0<T>::$_m X = this->GetX();
    typename Fit::tEstimator0<T>::$_m Y = this->GetY();
    for (int obsIndex = 0 ; obsIndex < X->GetRows() ; obsIndex++) {
      //Substitute x_is for the observed values
      for (int col = 1 ; col <= X->GetCols() ; col++) {
        if (row == 1 && g_i->GetSize() < col) {
          g_i->AddFunction(new tConstant<T>());
        }
        constant = dynamic_cast<tConstant<T> *>(g_i->Get(col));
        assert(constant != NULL);
        constant->Set(X->Get(row, col));
      }
      //Remap z_is 
      if (row == 1) {
        for (int col = 1 ; col <= this->GetP() ; col++) {
          g_i->AddFunction(new tSelector<T>(col));
        }
      }

      add->AddTerm(
                   _f::Square(
                              _f::Sub(
                                      _f::Composite(this->f->Clone(), g_i->Clone()),
                                      new tConstant<T>(this->GetY()->Get(row,1))
                                      )
                              )
                   );
      
      row++;
    }
    
    this->objective.reset(add.release());
    
  }


  template<class T>
  void tFitLS<T>::InitFOCs() {
    assert(this->objective.get() != NULL);
    $_nsolver solver = this->GetSolver(); 
    assert(solver);
    $$_f dfdx;
    for (int zRow = 1 ; zRow <= this->GetP() ; zRow++) {
      dfdx.reset(this->objective->Getdydx(zRow)->Clone());
      //      dfdx->Dump(std::cout, "dfdx: \t");
      solver->AddEquation(std::move(dfdx)); //Should be derivatives!
    }
  }


  template<class T>
  void tFitLS<T>::Estimate()
  {
    
    this->InitObjective();
    this->InitFOCs();
    $_nsolver solver = this->GetSolver(); 
    assert(solver);

    solver->Open();
    if (solver->Solve(_matrixOpt::read(GetOptions().get(), "guess"))) {
      B = $_m(new _m(solver->GetCurrentX()));
    }
    solver->Close();

  }


  template class tFitLS<float>;
  template class tFitLS<double>;
  

  }

  template class tException<Fit::tFitLS<float> >;
  template class tException<Fit::tFitLS<double> >;
  
}
  
  



