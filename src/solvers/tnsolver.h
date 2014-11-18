/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#ifndef _P4TH_TNSOLVER_H_
#define _P4TH_TNSOLVER_H_

#include <misctypes.h>
#include <coretypes.h>
#include <solvertypes.h>


#include <tfmatrix.h>
#include <tnmmatrix.h>
#include <tfunction0.h>
#include <tfunctions.h>
#include <tmathtools.h>
#include <stlstringtools.h>

#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif

#include <sstream>
#include <string>
#include <set>
#include <memory>
#include <options.h>
#include <toption.h>
#include <tstepper.h>

/*

  Notation and definitions taken from 
  
  http://www.scholarpedia.org/article/Numerical_methods_for_nonlinear_equations

*/

using namespace std;

namespace P4th
{

  template<class TYPE> 
    class tNSolver 
  {
  protected:
    MTH_MISC_TYPES(TYPE);
    MTH_CORE_TYPES(TYPE);
    MTH_SOLVER_TYPES(TYPE);
  protected:
    std::unique_ptr<_fs> equations;
    _strVec equationNames;
    _strVec choices;
    $$fm0 F;
    $$fm0 dF;
    
    //Options
    $$options options;

    //States
    bool verboseIteration;
    int currentIteration;
    _m currentX;
    _m currentF;
    _m currentdF;
    _m currentJ;
    TYPE currentDeterminant;
    
    _m Step( const _m &x ) const;
    _m UpdateJ( const _m &x ) const;
    
    void InitF();
    void InitdF();

     static string TrimScalar( TYPE scalar , int width );  
     
     static void PrintRowVector(std::ostream &dest, const vector<string> &labels, const tnmmatrix<TYPE> &vec, int width);
     static void PrintCurrentX( const tNSolver &solver );  
     static void PrintCurrentF( const tNSolver &solver );  
     static void PrintCurrentdF( const tNSolver &solver );  
     static bool AcceptGuess( const tNSolver &solver );  
     static bool TryNextGuess( const tNSolver &solver );  
     void EnsureChoices();
     Options *opts() const {
       return this->options.get();
     }
  public:
     tNSolver();
     virtual ~tNSolver();
     
     
     virtual Options *ResetOptions();
     virtual Options *GetOptions() {
       return this->opts();
     }
     void AddChoice( string name );
     void AddEquation( $$_f aEquation , string name = "" );
     
     
     
     virtual void Open(); 
     virtual bool Solve( const _m &x0 ); 
     virtual void Close(); 
 
     int GetN() const
     { return choices.size(); }
     _m GetCurrentX() const 
     { return currentX; }
    _m GetCurrentF() const 
    { return currentF; }
    _m GetCurrentdF() const 
    { return currentdF; }
    int GetCurrentIteration() const 
    { return currentIteration; }
    TYPE GetCurrentDeterminant() const 
    { return currentDeterminant; }
    
    void PrintX(std::ostream &dest) {
      PrintRowVector(dest, this->choices, this->GetCurrentX(), _intOpt::read(opts(), "scalarWidth"));
    }  

  };



}


#endif
