
INCS = 	-I. \
	-I../misc\
	-I../slatec\
	-I../core\
	-I../solvers\
	-I/usr/include \

LIBS =	\
	-L./ \
	-L../ \
	-L/usr/lib \
	-lmthmisc \
	-lmthslatec \
	-lmthcore \
	-lstdc++ \
	-lm \

OBJS = \
	templates.o \
	tintervalstepper.o \
	tnsolver.o \



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


STATIC_LIB=libmthsolvers.a
DYNAMIC_LIB=libmthsolvers.so.1.0.1
ifndef LIBDIR
LIBDIR=.
endif

libs: $(OBJS)
	gcc -shared -Wl,-soname,libmthcore.so.1 -Wl,-export-dynamic \
	-o $(DYNAMIC_LIB) $(OBJS)  $(FLAGS)
	ar cru $(STATIC_LIB) $(OBJS) 
	ranlib $(STATIC_LIB)

install: 
	cp $(STATIC_LIB) $(LIBDIR)	
	cp $(DYNAMIC_LIB) $(LIBDIR)	

tnsolver.o : tnsolver.h tnsolver.cpp tstepper.h   
	cc -c tnsolver.cpp -std=gnu++0x $(INCS) $(DEFS) $(FLAGS)

tintervalstepper.o : tintervalstepper.cpp tintervalstepper.h tnsolver.h    
	cc -c tintervalstepper.cpp -std=gnu++0x $(INCS) $(DEFS) $(FLAGS)

templates.o : templates.cpp   
	cc -c templates.cpp -std=gnu++0x $(INCS) $(DEFS) $(FLAGS)



clean:
	rm -f $(STATIC_LIB)
	rm -f $(DYNAMIC_LIB)
	rm -f *.o
	rm -f *.a

all:
	make 
