/*
	Copyright 2014 Klas Sandén.
 	Released under the GPLv3 license.
 	https://github.com/positively4th/mth
*/
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE tOptions
#include <boost/test/unit_test.hpp>

//Include common typedefs
#include <misctypes.h>

#include <math.h>
#include <memory>
#include <options.h>
#include <toption.h>

using namespace P4th;
using namespace std;

bool isOptionsException( const tException<Options> &ex) { return true; };
bool isStringOptionException( const tException<_strOpt > &ex) { return true; };
bool isIntOptionException( const tException<_intOpt> &ex) { return true; };


BOOST_AUTO_TEST_CASE( ints )
{

  unique_ptr<Options> options(new Options());
  _intOpt::set(options.get(), "first", 1);
  _intOpt::set(options.get(), "second", 2);
  _intOpt::set(options.get(), "third", 3);
  {
    int exp = 1;
    int act = _intOpt::read(options.get(), "first");
    BOOST_CHECK_EQUAL(exp, act); 
  }
  {
    int exp = 2;
    int act = _intOpt::read(options.get(), "second");
    BOOST_CHECK_EQUAL(exp, act); 
  }
  {
    int exp = 3;
    int act = _intOpt::read(options.get(), "third");
    BOOST_CHECK_EQUAL(exp, act); 
  }
  {
    BOOST_CHECK_EXCEPTION(_intOpt::read(options.get(), "fourth"), tException<Options>, isOptionsException); 
  }
  {
    BOOST_CHECK_EXCEPTION(_strOpt::read(options.get(), "first"), tException<_strOpt>, isStringOptionException); 
  }
}

BOOST_AUTO_TEST_CASE( strings )
{

  unique_ptr<Options> options(new Options());
  _strOpt::set(options.get(), "first", "f");
  _strOpt::set(options.get(), "second", "s");
  _strOpt::set(options.get(), "third", "t");
  
  {
    string exp = "f";
    string act = _strOpt::read(options.get(), "first");
    BOOST_CHECK_EQUAL(exp, act); 
  }
  {
    string exp = "s";
    string act = _strOpt::read(options.get(), "second");
    BOOST_CHECK_EQUAL(exp, act); 
  }
  {
    string exp = "t";
    string act = _strOpt::read(options.get(), "third");
    BOOST_CHECK_EQUAL(exp, act); 
  }
  {
    BOOST_CHECK_EXCEPTION(_strOpt::read(options.get(), "fourth"), tException<Options>, isOptionsException); 
  }
  {
    BOOST_CHECK_EXCEPTION(_intOpt::read(options.get(), "first"), tException<_intOpt>, isIntOptionException); 
  }
}

