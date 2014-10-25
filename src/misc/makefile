INCS = 	-I/usr/include \
	-I./

LIBS =	-L/usr/lib \
	-L./ \
	-L../libs \
	-lstdc++ \
	-lm \


OBJS =  \
	debug.o \
	stlstringtools.o \
	tnmcontainer.o \
	linkedptrlist.o linkedptrlistpost.o \
	rand.o randomtools.o \
	timetools.o \
	tpointerstack.o \
	linkedlist.o linkedlistpost.o \
#	filetools.o
#	clocker.o \
#	xmlparser.o xmlfinder.o \
#	tracer.o tracertag.o tracerstring.o \
#	timers.o timer.o timer0.o timestamp.o \

FLAGS = -g -fno-default-inline -fpic -rdynamic -D_GSL
TEST_LINKERFLAGS = \
	-lboost_unit_test_framework \
# -pg -static
#FLAGS = -g

DEFS = -D_SAFE -D_DEBUG
#DEFS = -D_DEBUG -D_SAFE

STATIC_LIB=libmthmisc.a
SHARED_LIB=libmthmisc.so.1.0.1
ifndef LIBDIR
LIBDIR=.
endif

libs: $(OBJS) 
	gcc -shared -Wl,-soname,libmthmisc.so.1 -Wl,-export-dynamic \
	-o libmthmisc.so.1.0.1 $(OBJS)  $(FLAGS)
	ar cru $(STATIC_LIB) $(OBJS) 
	ranlib $(STATIC_LIB)

install: libs
	cp -p $(STATIC_LIB) $(LIBDIR)
	cp -p $(SHARED_LIB) $(LIBDIR)

teststlstringtools : teststlstringtools.cpp stlstringtools.cpp stlstringtools.h $(OBJS)
	make $(STATIC_LIB)
	gcc -c teststlstringtools.cpp $(FLAGS) $(INCS)  $(DEFS)
	gcc -o teststlstringtools teststlstringtools.o $(FLAGS) $(DEFS) $(LIBS) -lmthmisc $(TEST_LINKERFLAGS)

testtpointerstack : testtpointerstack.cpp tpointerstack.cpp tpointerstack.h $(OBJS)
	make $(STATIC_LIB)
	gcc -c testtpointerstack.cpp $(FLAGS) $(INCS)  $(DEFS)
	gcc -o testtpointerstack testtpointerstack.o $(FLAGS) $(DEFS) $(LIBS) -lmthmisc $(TEST_LINKERFLAGS)

testclocker : testclocker.cpp clocker.cpp clocker.h $(OBJS)
	make $(STATIC_LIB)
	gcc -c testclocker.cpp $(FLAGS) $(INCS)  $(DEFS)
	gcc -o testclocker testclocker.o $(FLAGS) $(DEFS) $(LIBS) \
		-lmthmiscmath -lmthmisctslatectools -lmthmisccslatec -lf2cstatic -lmthmisc $(TEST_LINKERFLAGS)

debug.o : debug.h debug.cpp  
	cc -c debug.cpp $(INCS) $(DEFS) $(FLAGS)

tnmcontainer.o : tnmcontainer.cpp tnmcontainer.h debug.h  
	cc -c tnmcontainer.cpp $(INCS) $(DEFS) $(FLAGS)

randomtools.o : randomtools.h randomtools.cpp debug.h  
	cc -c randomtools.cpp $(INCS) $(DEFS) $(FLAGS)

stlstringtools.o : stlstringtools.h stlstringtools.cpp debug.h  
	cc -c stlstringtools.cpp -DGCC $(INCS) $(DEFS) $(FLAGS)


xmlparser.o : xmlparser.h xmlparser.cpp debug.h  
	cc -c xmlparser.cpp $(INCS) $(DEFS) $(FLAGS)

linkedlist.o : linkedlist.h linkedlist.cpp linkedlistpost.o debug.h
	cc -c linkedlist.cpp $(INCS) $(DEFS) $(FLAGS)

linkedlistpost.o : linkedlistpost.h linkedlistpost.cpp debug.h
	cc -c linkedlistpost.cpp $(INCS) $(DEFS) $(FLAGS)

linkedptrlist.o : linkedptrlist.h linkedptrlist.cpp linkedptrlistpost.o debug.h
	cc -c linkedptrlist.cpp $(INCS) $(DEFS) $(FLAGS)

linkedptrlistpost.o : linkedptrlistpost.h linkedptrlistpost.cpp debug.h
	cc -c linkedptrlistpost.cpp $(INCS) $(DEFS) $(FLAGS)

rand.o : rand.h rand.cpp debug.h  
	cc -c rand.cpp $(INCS) $(DEFS) $(FLAGS)

xmlfinder.o : xmlfinder.h xmlfinder.cpp xmlparser.h debug.h  
	cc -c xmlfinder.cpp $(INCS) $(DEFS) $(FLAGS)

tpointerstack.o : tpointerstack.h tpointerstack.cpp debug.h  
	g++ -c tpointerstack.cpp -DGCC $(INCS) $(DEFS) $(FLAGS)

filetools.o : filetools.h filetools.cpp debug.h  
	cc -c filetools.cpp $(INCS) $(DEFS) $(FLAGS)

timetools.o : timetools.h timetools.cpp debug.h  
	cc -c timetools.cpp $(INCS) $(DEFS) $(FLAGS)

timers.o : timers.h timers.cpp timer0.h timestamp.h debug.h  
	cc -c timers.cpp $(INCS) $(DEFS) $(FLAGS)

timer.o : timer.h timer.cpp timer0.h timestamp.h debug.h  
	cc -c timer.cpp $(INCS) $(DEFS) $(FLAGS)

timer0.o : timer0.h timer0.cpp timestamp.h debug.h  
	cc -c timer0.cpp $(INCS) $(DEFS) $(FLAGS)

clocker.o : clocker.h clocker.cpp timestamp.h debug.h  
	cc -c clocker.cpp $(INCS) $(DEFS) $(FLAGS)

timestamp.o : timestamp.h timestamp.cpp debug.h  
	cc -c timestamp.cpp $(INCS) $(DEFS) $(FLAGS)

tracertag.o : tracertag.h tracertag.cpp debug.h  
	cc -c tracertag.cpp $(INCS) $(DEFS) $(FLAGS)

tracerstring.o : tracerstring.h tracerstring.cpp tracertag.h debug.h  
	cc -c tracerstring.cpp $(INCS) $(DEFS) $(FLAGS)

tracer.o : tracer.h tracer.cpp tracertag.h debug.h  
	cc -c tracer.cpp $(INCS) $(DEFS) $(FLAGS)

doxygen: *.h *.cpp
	doxygen classes.dxcfg

classes: *.h *.cpp *.dxcfg
	make libs
	make tests
	make doxygen

clean:
	rm -f *.o
	rm -f *.a

tests: 
	make teststlstringtools

all:
	make
	make tests
