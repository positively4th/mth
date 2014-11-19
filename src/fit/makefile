
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
	tfit.o \
	twasher0.o \
	twasher.o \
	testimator0.o \
	tols.o \
	templates.o \



#FLAGS = -g
#FLAGS = -O2 -fpermissive
FLAGS = -g -fno-default-inline -fpic  -rdynamic -fno-stack-protector -frounding-math -fsignaling-nans 
# -pg -static

LINKERFLAGS = -rdynamic -u MAIN__
#-pg 
#-static
#LINKERFLAGS = -pg


DEFS = -D_SAFE -D_DEBUG -D_MATHOPT -DMTH -D_SLATEC 
#DEFS = -D_SAFE -D_DEBUG -D_OCTAVE -D_MATHOPT
#DEFS = -D_DEBUG -D_SAFE


STATIC_LIB=libmthfit.a
DYNAMIC_LIB=libmthfit.so.1.0.1
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

tols.o : tols.h tols.cpp tfit.h twasher.h
	cc -c tols.cpp -std=gnu++0x $(INCS) $(DEFS) $(FLAGS)

tfit.o : tfit.h tfit.cpp twasher.h    
	cc -c tfit.cpp -std=gnu++0x $(INCS) $(DEFS) $(FLAGS)

twasher.o : twasher.h twasher.cpp twasher0.h    
	cc -c twasher.cpp -std=gnu++0x $(INCS) $(DEFS) $(FLAGS)

twasher0.o : twasher0.h twasher0.cpp    
	cc -c twasher0.cpp -std=gnu++0x $(INCS) $(DEFS) $(FLAGS)

testimator0.o : testimator0.h testimator0.cpp    
	cc -c testimator0.cpp -std=gnu++0x $(INCS) $(DEFS) $(FLAGS)

templates.o : templates.cpp ../misc/tptroption.h twasher.h twasher.cpp twasher0.h twasher0.cpp   
	cc -c templates.cpp -std=gnu++0x $(INCS) $(DEFS) $(FLAGS)



clean:
	rm -f $(STATIC_LIB)
	rm -f $(DYNAMIC_LIB)
	rm -f *.o
	rm -f *.a

all:
	make 
