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

#FLAGS = -g
#FLAGS = -O2 -fpermissive
FLAGS = -g -fno-default-inline -fpic  -rdynamic -fno-stack-protector -frounding-math -fsignaling-nans 
# -pg -static

LINKERFLAGS = -rdynamic -u MAIN__
#-pg 
#-static
#LINKERFLAGS = -pg

TEST_LINKERFLAGS = \
	-lboost_unit_test_framework \


DEFS = -D_SAFE -D_DEBUG -D_MATHOPT -DMTH -D_SLATEC 
#DEFS = -D_SAFE -D_DEBUG -D_OCTAVE -D_MATHOPT
#DEFS = -D_DEBUG -D_SAFE

ROOTDIR := $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
export LIBDIR="$(ROOTDIR)/libs"

SUBDIRS = misc slatec core solvers
INSTALLDIRS = $(SUBDIRS:%=install-%)

.PHONY: subdirs $(SUBDIRS)

subdirs: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

install: libs $(INSTALLDIRS)

libs:
	mkdir -p libs

tests:
	make -C tests

$(INSTALLDIRS): 
	$(MAKE) -C $(@:install-%=%) install

slatec: misc

core: slatec

clean:
	make -C tests clean 
	make -C misc clean 
	make -C slatec clean
	make -C core clean
	rm -rf *.o
	rm -rf *.a
	rm -rf libs

all:
	make 
	make libs
