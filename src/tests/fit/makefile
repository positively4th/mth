
INCS = 	-I. \
	-I../../misc \
	-I../../slatec \
	-I../../core \
	-I../../solvers \
	-I../../fit \
	-I/usr/include \


LIBS =	\
	-L../../libs \
	-L/usr/lib \
	-lmthfit \
	-lmthsolvers \
	-lmthcore \
	-lmthmisc \
	-lmthslatec \
	-lf2c \
	-lstdc++ \
	-lm \

OBJS = \
	test_tols.o \

	FLAGS = -g -fno-default-inline -fpic  -rdynamic -fno-stack-protector -frounding-math -fsignaling-nans 

#LINKERFLAGS = -u MAIN__
LINKERFLAGS = -rdynamic -u MAIN__

TEST_LINKERFLAGS = \
	-lboost_unit_test_framework \

DEFS = -D_SAFE -D_DEBUG -D_MATHOPT -DMTH -D_SLATEC 
#DEFS = -D_SAFE -D_DEBUG -D_OCTAVE -D_MATHOPT
#DEFS = -D_DEBUG -D_SAFE


tests: $(OBJS) 
	cc test_tols.o -o test_tols $(INCS) $(LINKERFLAGS) $(TEST_LINKERFLAGS) $(LIBS)

install:

test_tols.o : test_tols.cpp  
	cc -c test_tols.cpp -std=gnu++0x $(INCS) $(DEFS) $(FLAGS)


clean:
	rm -rf *.o

all:
	make 
