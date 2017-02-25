lib_LTLIBRARIES = libmthplot.la
libmthplot_la_LDFLAGS = -module -shared -export-dynamic 
libmthplot_la_SOURCES = \
	templates.cpp \
	tplot.cpp 

AM_CPPFLAGS = \
	-I. \
        -I../misc\
        -I../slatec \
        -I../core \
        -I../solvers \
        -I/usr/include \
	-g -fno-default-inline -fpic  -rdynamic -fno-stack-protector -frounding-math -fsignaling-nans -std=gnu++0x \
	-D_SAFE -D_DEBUG -D_MATHOPT -DMTH -D_SLATEC -DGSL
