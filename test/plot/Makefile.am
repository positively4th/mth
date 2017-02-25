AM_CPPFLAGS = \
	-I. \
        -I../../src/plot \
        -I../../src/fit \
        -I../../src/solvers \
        -I../../src/slatec \
        -I../../src/core \
        -I../../src/misc \
	-g -fno-default-inline -fpic  -rdynamic -fno-stack-protector -frounding-math -fsignaling-nans -std=gnu++0x \
	-DMTH -DGSL -D_SLATEC

AM_LDFLAGS = \
	-rdynamic -u MAIN__ -lboost_unit_test_framework \
        -I/usr/include \
	-L../../src/plot/.libs \
	-L../../src/fit/.libs \
	-L../../src/solvers/.libs \
	-L../../src/misc/.libs \
	-L../../src/slatec \
	-L../../src/slatec/src \
	-L../../src/core/.libs \
        -L/usr/lib \
        -lmthplot \
        -lmthfit \
        -lmthsolvers \
        -lmthcore \
        -lmthslatec \
        -lmthcslatec \
        -lmthmisc \
	-lf2c \
	-lstdc++ \
        -lm 



bin_PROGRAMS = test_tplot
test_tplot_SOURCES = test_tplot.cpp
