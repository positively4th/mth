INCS = 	-I. \
	-I../misc/ \
	-I/usr/include \

LIBS =	\
	-L./ \
	-L./src/ \
	-L../ \
	-L/usr/lib \
	-lp4thcslatec \
	-lp4th$ \
	-lm \
	-lstdc++ \
	-lc \
#	-lF2C \


OBJS = \
	tslatectools.o \
#	tslatecstack.o \
#	tslatecerror.o \
#	templates.o \

#FLAGS = -g
#FLAGS = -O2 -fpermissive
FLAGS = -g -fno-default-inline -fpic -rdynamic -fno-stack-protector -frounding-math -fsignaling-nans
TESTER_FLAGS = -g -fno-default-inline -fpie -pie -rdynamic -fno-stack-protector  
#-pg -static

DEFS = -D_SAFE -D_DEBUG 
#DEFS = -D_DEBUG -D_SAFE


#tester :  tester.o libp4thtslatectools.a
#	gcc -o tester tester.o $(LIBS) $(FLAGS)

STATIC_LIB=libmthslatec.a
ifndef LIBDIR
LIBDIR=.
endif

$(STATIC_LIB) : libmthcslatec.a $(OBJS) 
	make -C src
	ar cru $(STATIC_LIB) src/*.o $(OBJS)   
	ranlib $(STATIC_LIB)

install: 
	cp -p $(STATIC_LIB) $(LIBDIR) 

libmthcslatec.a : 
	make -C src/ libmthcslatec.a

tslatectools.o : tslatectools.h tslatectools.cpp tslatecstack.h  
	cc -c tslatectools.cpp -fpermissive $(INCS) $(DEFS) $(FLAGS)

tslatecstack.o : tslatecstack.h tslatecstack.cpp 
	cc -c tslatecstack.cpp $(INCS) $(DEFS) $(FLAGS)

tslatecerror.o : tslatecerror.h tslatecerror.cpp 
	cc -c tslatecerror.cpp $(INCS) $(DEFS) $(FLAGS)

templates.o : templates.cpp 
	cc -c templates.cpp $(INCS) $(DEFS) $(FLAGS)


clean:
	make -C src/ clean
	rm -rf *.o
	rm -rf *.a
#	ln -s /usr/lib/libc.a

#download: 
#	make download -C src/

all :
	make $(STATIC_LIB)
	make tester


