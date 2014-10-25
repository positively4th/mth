/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE tLn
#include <boost/test/unit_test.hpp>

#include <tln.h>
#include <cmath>
#include <memory>
#include <vector>
#include "tmathtools.h"

using namespace P4th;

BOOST_AUTO_TEST_CASE( testLn )
{
  typedef tLn<double> _ln;
  typedef tFunction0<double> _f;

  const double xMax = 100000;
  const double xMin = 0.0001;
  
  double x = xMin;
  double yExp;
  double yAct;

  unique_ptr<_ln> ln(new _ln());
  while (x < xMax) {
    yExp = log(x);
    //    yAct = ln->y(tnmmatrix<double>(1,1,x));
    yAct = ln->_f::y(x);
    BOOST_CHECK_CLOSE_FRACTION( yExp , yAct , 0.00001  );
    x *= 2.0;
  }
}
  
BOOST_AUTO_TEST_CASE( testdLn )
{
  typedef tLn<double> _ln;
  typedef tFunction0<double> _f;
  
  const double xMax = 100000;
  const double xMin = 0.0001;
  
  double x = xMin;
  double yExp;
  double yAct;
  
  // Create the ln function
  unique_ptr<_ln> ln(new _ln());
  // Get the the derivative of ln using Getdydx
  // Getdydx returns a const _f *, a function owned by ln.
  // Use Clone() to create a private copy.
  unique_ptr<_f> dln(ln->_f::Getdydx(1)->Clone());
  
  while (x < xMax) {
    yExp = 1.0 / x;
    //    yAct = ln->y(tnmmatrix<double>(1,1,x));
    yAct = dln->_f::y(x);
    BOOST_CHECK_CLOSE_FRACTION( yExp , yAct , 0.00001  );
    x *= 2.0;
  }
  
}


