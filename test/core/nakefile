
INCS = 	-I. \
	-I../../misc \
	-I../../slatec \
	-I../../core \
	-I/usr/include \


LIBS =	\
	-L../../libs \
	-L/usr/lib \
	-lmthcore \
	-lmthmisc \
	-lmthslatec \
	-lmthcore \
	-lmthmisc \
	-lmthslatec \
	-lf2c \
	-lstdc++ \
	-lm \

OBJS = \
	test_tln.o \

	FLAGS = -g -fno-default-inline -fpic  -rdynamic -fno-stack-protector -frounding-math -fsignaling-nans 

#LINKERFLAGS = -u MAIN__
LINKERFLAGS = -rdynamic -u MAIN__

TEST_LINKERFLAGS = \
	-lboost_unit_test_framework \

DEFS = -D_SAFE -D_DEBUG -D_MATHOPT -DMTH -D_SLATEC 
#DEFS = -D_SAFE -D_DEBUG -D_OCTAVE -D_MATHOPT
#DEFS = -D_DEBUG -D_SAFE


tests: $(OBJS) 
	cc test_tln.o -o test_tln $(INCS) $(LINKERFLAGS) $(TEST_LINKERFLAGS) $(LIBS)

install:

test_tln.o : test_tln.cpp  
	cc -c test_tln.cpp -std=gnu++0x $(INCS) $(DEFS) $(FLAGS)


clean:
	rm -rf *.o
	rm -rf *.a

all:
	make 
