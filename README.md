mth
===

Yet another c++ library for vector algebra, optimization, regression and more.

I created this library years ago and I have not worked on it for a
some years. However I have found it useful and realtively easy to work
with. I need to do some cosmetical changes to the source before
uploading all of it. If I can keep my motivation up and/or there is
any interest the following parts will be added in the coming months:

* Core - available

Inlucdes the matrix/vector class. The function class. Basic
functions. All functions support function composition and analytical derivatives. 

* Solvers - in progress

A n-dimensional Newton–Raphson solver.

* Optimization - not ready

Includes an unconstrained n-dimensional maximizer, a n-dimsnsional
maximizer for problems with equality constraints, and a n-dimensional
maximizer for problems with inequality constraints. 

* Curve fiting - not ready

Includes an OLS estimator, logit and  tobit estimators and a general
curve fiter.

* Plot - not ready

General plot utility that plots using gnuplot.

The code id made public as is and to build the libraries run make &&
make install in the src directory. The built libraries are placed in the
libs dir.

* and more ...

Docs
----

See the src/tests dir for how to use the classes.

Tests
-----

All tests are placed in the src/test folder. Read the source code to
see examples of how to use the classes.

Dependencies
------------

The following dependencies are mandatory:

* f2c

* and more