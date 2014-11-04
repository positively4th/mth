/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#pragma once

#include <string>
#include <fstream>
#include <memory>

//#include "tcommand.h"
#include "tfunction0.h"
#include "tgridfunction.h"
//#include "tformula.h"
#include <tfunctions.h>
#include <polymorphic.h>
#include "tnmmatrix.h"
#include "tinterval.h"
#include "linkedptrlist.h"
#include <stlstringtools.h>
#include <vector>
#include <options.h>
#include <toption.h>
#include <memory>

//class istream;
//class ::std::ostream;

using namespace std;

namespace P4th
{
  template<class TYPE>
    class tGridFunction;
  
  template<class TYPE> 
    class tPlot : public Polymorphic {
  public:

    typedef tFunction0<TYPE> _f;
    typedef tFunctions<TYPE> _fs;
    typedef std::vector<string> _strVec;
    typedef _strVec::const_iterator cStrVecee;
    typedef _strVec::iterator _strVecee;
    typedef std::vector<tInterval<TYPE> > _intervalList;
    typedef typename _intervalList::const_iterator _constIntervalListIt;
    typedef typename _intervalList::iterator _intervalListIt;
    typedef unique_ptr<Options> _options;
    typedef tOption<string> _stringOpt;
    typedef tOption<_strVec> _strVecOpt;
    typedef tOption<int> _intOpt;
    typedef tOption<bool> _boolOpt;
    typedef tOption<TYPE> _typeOpt;
    typedef tOption<double> _doubleOpt;
    typedef tOption<tnmmatrix<TYPE> > _matrixOpt;
  protected:

    _options options;
    int dim;
    _strVec initdos;
    _strVec plotdos; 

    _strVec files;
    _strVec gnuPlotCmds;
    _intervalList intervals;
    std::ostream *scriptstream;
    std::ofstream _scriptstream;

    //    tGridFunction<TYPE> gf;
    TYPE min,max;

    void AddInitDo( const string &_do )
    { initdos.push_back( _do ); }
    void AddPlotDo( const string &_do )
    { plotdos.push_back( _do ); }
    void DeleteInitDos( int until = -1 );
    void DeletePlotDos( int until = -1 );
    virtual Options *ResetOptions();
    virtual  Options *Opts() const {
      return this->options.get();
    }
    string GetLabelPosition(unsigned int ith);
    void PlotBefore();
    void PlotDo( string heading, string labelstr, const _fs *funcs);
    void PlotAfter();
  public:
    tPlot();
    virtual ~tPlot();

    Options *GetOptions() {
      return this->Opts();
    } 
    string GetLineColor(unsigned int ith) const;
    string GetTextColor(unsigned int ith) const;

    virtual void AddInterval( const tInterval<TYPE> &_int ) 
    { intervals.push_back( _int ); }
    virtual void SetView( double ang1 , double ang2  ) 
    { AddPlotDo( (string)"set view " + 
		      STLStringTools::AsString( ang1 , 2 , '.' ) + "," + 
		      STLStringTools::AsString( ang2 , 2 , '.' ) ); }
    void SetScriptFile( std::ofstream &dest )
    {  scriptstream = &dest; }
    virtual void SetRemoveHidden( bool flag ) 
    { 
      if ( flag ) 
	this->AddPlotDo( (string)"set hidden3d" ); 
      else
	this->AddPlotDo( (string)"unset hidden3d" ); 
    }
    virtual void SetShowContour( bool flag ) 
    { 
      if ( ! flag ) { this->AddPlotDo( (string)"unset contour" ); return; }
      this->AddPlotDo( (string)"set contour base" );
      this->AddPlotDo( (string)"set clabel" );
      this->AddPlotDo( (string)"set cntrparam level auto 7" ); 
    }
    virtual void AddFile( const string &file ) 
    { files.push_back( file ); }
    virtual void AddGnuplotCommand( const string &cmd ) 
    { this->AddPlotDo( cmd ); }
    
    void Reset();
    virtual void Plot( string heading, string labelstr, const _fs *funcs );
    
    virtual std::ofstream &OpenTextFile( std::ofstream &dest , 
					 string filename , 
					 const string &throwstring = "" ) const;

    virtual std::ostream &Dump( std::ostream &dest , P4th::string lm = "" ) const; 

    std::ofstream &OpenTextFile( std::ofstream &dest , const string &file ) const;
    std::ofstream &CloseFile( std::ofstream &dest ) const;
    std::ostream &WriteInitDos();
    std::ostream &WritePlotDos();
    static const tFunction0<TYPE> *WriteData( const _f *f , int index, Polymorphic *_this );
  };



}

