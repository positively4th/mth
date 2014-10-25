INCS = 	-I. \
	-I../misc\
	-I../slatec \
	-I/usr/include \


LIBS =	\
	-L./ \
	-L../ \
	-L/usr/lib \
	-lmthmisc \
	-lmthcore \
	-lstdc++ \
	-lm \

FLAGS = -g -fno-default-inline -fpic  -rdynamic -fno-stack-protector -frounding-math -fsignaling-nans 

LINKERFLAGS = -rdynamic -u MAIN__

TEST_LINKERFLAGS = \
	-lboost_unit_test_framework \


DEFS = -D_SAFE -D_DEBUG -D_MATHOPT -DMTH -D_SLATEC 

ROOTDIR := $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
export LIBDIR="$(ROOTDIR)/libs"

SUBDIRS = core
.PHONY: subdirs $(SUBDIRS)

subdirs: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@


clean:
	make -C misc clean 
	make -C slatec clean
	make -C core clean
	rm -rf *.o
	rm -rf *.a
	rm libs/*

all:
	make 
	make libs
