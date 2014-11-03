/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE tPlot
#include <boost/test/unit_test.hpp>

#include <memory>
#include <vector>

#include <tfunction0.h>
#include <tfunctions.h>
#include <tconstant.h>
#include <tselector.h>
#include <tinterval.h>
#include <tplot.h>
#include <randomtools.h>
#include <options.h>
#include <toption.h>

using namespace P4th;
using namespace std;

typedef tFunction0<double> _f;
typedef tFunctions<double> _fs;
typedef tInterval<double> _interval;
typedef tConstant<double> _c;
typedef tSelector<double> _arg;
typedef tnmmatrix<double> _m;
typedef tPlot<double> _plot;
typedef tOption<bool> _boolOpt;
typedef tOption<string> _strOpt;
typedef vector<string> _strVec;;
typedef tOption<_strVec> _strVecOpt;

BOOST_AUTO_TEST_CASE( plot2d )
{
  unique_ptr<_fs> funcs(new _fs());
  funcs.get()->AddFunction(new _c(2.0));
  funcs.get()->AddFunction(new _arg(1));
  funcs.get()->AddFunction(_f::Sub(new _c(2.0), new _arg(1)));

  unique_ptr<_plot> plot(new _plot());
  Options *opts = plot->GetOptions();
  _boolOpt::set(opts, "keepFiles", false);
  plot->AddInterval(_interval("x",0,5,5));
  plot->Plot("test_tplot: ploat2d" , "y=2|y=x" , funcs.get());

}

BOOST_AUTO_TEST_CASE( plot3d )
{

  unique_ptr<_fs> funcs(new _fs());
  funcs.get()->AddFunction(new _c(0.0));
  funcs.get()->AddFunction(_f::Sub(_f::Square(new _arg(1)), _f::Square(new _arg(2))));
  funcs.get()->AddFunction(_f::Add(new _arg(1),new _arg(2), 0L));

  unique_ptr<_plot> plot(new _plot());
  Options *opts = plot->GetOptions();
  _boolOpt::set(opts, "keepFiles", false);
  _strOpt::set(opts, "type", "X11");

  _strVec colors;
  colors.push_back("rgb \"#f0f0f0\"");
  colors.push_back("rgb \"#000000\"");
  colors.push_back("rgb \"#ff00ff\"");
  _strVecOpt::set(opts, "colors", colors);
  plot->AddInterval(_interval("x",-5,5,100));
  plot->AddInterval(_interval("y",-5,5,100));


  plot->Plot("test_tplot: plot3d" , "z=0|z=x*x-y*y|z=x+y" , funcs.get());

}


