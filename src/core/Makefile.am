lib_LTLIBRARIES = libmthcore.la
libmthcore_la_LDFLAGS = -module -shared -export-dynamic 
libmthcore_la_SOURCES = \
	tabs.cpp tbinary.cpp tdiv.cpp tfmatrix.cpp tfunctions.cpp tlinear.cpp tmul.cpp tparameters.cpp treciprocal.cpp tsqrt.cpp ttranslate.cpp \
	tadd.cpp tconstant.cpp tfog.cpp tintegral.cpp tln.cpp tnegative.cpp tnumericalderivative.cpp tpow.cpp tselector.cpp tsquare.cpp \
	tatom.cpp tderivatives0.cpp tfmatrix0.cpp tfunction0.cpp tinterval.cpp tmathtools.cpp tnmmatrix.cpp tparameter.cpp tquadratic.cpp tsign.cpp tsub.cpp \
	templates.cpp

#tnmmatrix_test.cpp

AM_CPPFLAGS = \
	-I. \
        -I../misc\
        -I../slatec \
        -I/usr/include \
	-g -fno-default-inline -fpic  -rdynamic -fno-stack-protector -frounding-math -fsignaling-nans \
	-DMTH -D_SLATEC


