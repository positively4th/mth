AM_CPPFLAGS = \
	-I. \
        -I../../src/slatec \
        -I../../src/core \
        -I../../src/misc \
	-g -fno-default-inline -fpic  -rdynamic -fno-stack-protector -frounding-math -fsignaling-nans -std=gnu++0x \
	-DMTH -DGSL -D_SLATEC

AM_LDFLAGS = -rdynamic -u MAIN__ -lboost_unit_test_framework \
        -I/usr/include \
	-L../../src/misc/.libs \
	-L../../src/slatec \
	-L../../src/slatec/src \
	-L../../src/core/.libs \
        -L/usr/lib \
        -lmthcore \
        -lmthslatec \
        -lmthcslatec \
        -lmthmisc \
	-lf2c \
	-lstdc++ \
        -lm 



# this lists the binaries to produce, the (non-PHONY, binary) targets in
# the previous manual Makefile
bin_PROGRAMS = test_tln test_tnmatrix
test_tln_SOURCES = test_tln.cpp
test_tnmatrix_SOURCES = test_tln.cpp
