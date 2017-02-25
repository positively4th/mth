AM_CPPFLAGS = \
	-I. \
        -I../../src/misc \
	-g -fno-default-inline -fpic  -rdynamic -fno-stack-protector -frounding-math -fsignaling-nans -std=gnu++0x

#-D_SLATEC

AM_LDFLAGS = -rdynamic -u MAIN__ -lboost_unit_test_framework \
        -I/usr/include \
	-L../../src/misc/.libs \
        -L/usr/lib \
        -lmthmisc \
        -lstdc++ \
        -lm 



# this lists the binaries to produce, the (non-PHONY, binary) targets in
# the previous manual Makefile
bin_PROGRAMS = test_stlstringtools test_options
test_stlstringtools_SOURCES = test_stlstringtools.cpp
test_options_SOURCES = test_options.cpp
