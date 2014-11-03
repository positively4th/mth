
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

OBJS = \
	tnmmatrix.o tmathtools.o \
	tfunction0.o tderivatives0.o tfunctions.o tfmatrix0.o tfmatrix.o \
	tconstant.o \
	tbinary.o tadd.o tsub.o tmul.o tdiv.o tnegative.o \
	tatom.o tpow.o tsquare.o tln.o \
	tabs.o tselector.o tfog.o tintegral.o treciprocal.o tsqrt.o \
	tnumericalderivative.o \
	ttranslate.o tparameter.o tsign.o \
	tinterval.o \
	tlinear.o tquadratic.o \
	templates.o \
#	tgridfunction.o \
#	tmathexception.o \
#	texp.o  \
#	tsin.o tcos.o ttan.o tsec.o tarccos.o tarcsin.o tarctan.o tarcsec.o \
#	tparameters.o \
#	tconstants.o \
#	tsolver.o tlagrange.o tguesses.o tkuhntucker.o toptimalcontrol.o \
#	tnsolver.o tintervalstepper.o tnmaximizer.o \
#	tregression.o tols.o tfitls.o \
#	tbinregression.o tlogit.o \
#	tstat2.o \
#	tplot.o tmodel.o \
#	tformula.o tnamedfunction.o tnamedpointer.o tuniverse.o \
#	tcommander.o tcommand.o \
#	tdefinecommand.o tevaluatecommand.o tplotcommand.o \
#	tsolvecommand.o targmaxcommand.o \
#	tmax.o tmin.o tintervalfunction.o \
#	tmatrixalias.o signalhandler.o \

TEST_OBJS = \
	tnmmatrix_test.o \
#	tfitls_test.o \
#	tuniverse_test.o \
#	tregression_test.o \
#	tlogit_test.o \
#	tsolver_test.o \
#	tnsolver_test.o \
#	tnmaximizer_test.o \
#	tfog_test.o \

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


STATIC_LIB=libmthcore.a
DYNAMIC_LIB=libmthcore.so.1.0.1
ifndef LIBDIR
LIBDIR=.
endif

libs: $(OBJS)
	gcc -shared -Wl,-soname,libmthcore.so.1 -Wl,-export-dynamic \
	-o libmthcore.so.1.0.1 $(OBJS)  $(FLAGS)
	ar cru $(STATIC_LIB) $(OBJS) 
	ranlib $(STATIC_LIB)

install: 
	cp $(STATIC_LIB) $(LIBDIR)	
	cp $(DYNAMIC_LIB) $(LIBDIR)	


tests : $(TEST_OBJS) 
	make libs
	cc tnmmatrix_test.o -o tnmmatrix_test $(INCS) $(LINKERFLAGS) $(TEST_LINKERFLAGS) $(LIBS)
#	cc tfitls_test.o -o tfitls_test $(INCS) $(LINKERFLAGS) $(TEST_LINKERFLAGS) $(LIBS)
#	cc tnmaximizer_test.o -o tnmaximizer_test $(INCS) $(LINKERFLAGS) $(TEST_LINKERFLAGS) $(LIBS)
#	cc tnsolver_test.o -o tnsolver_test $(INCS) $(LINKERFLAGS) $(TEST_LINKERFLAGS) $(LIBS)
#	cc tuniverse_test.o -o tuniverse_test $(INCS) $(LINKERFLAGS) $(TEST_LINKERFLAGS) $(LIBS)
#	cc tregression_test.o -o tregression_test $(INCS) $(LINKERFLAGS) $(TEST_LINKERFLAGS) $(LIBS)
#	cc tlogit_test.o -o tlogit_test $(INCS) $(LINKERFLAGS) $(TEST_LINKERFLAGS) $(LIBS) 
#	cc tsolver_test.o -o tsolver_test $(INCS) $(LINKERFLAGS) $(TEST_LINKERFLAGS) $(LIBS)
#	cc tfog_test.o -o tfog_test $(INCS) $(LINKERFLAGS) $(TEST_LINKERFLAGS) $(LIBS)


tconstant.o : tconstant.h tconstant.cpp tnmmatrix.h tderivatives0.h ../misc/texception.h ../misc/debug.h  
	cc -c tconstant.cpp $(INCS) $(DEFS) $(FLAGS)

tnmmatrix.o : tnmmatrix.h tnmmatrix.cpp ../misc/tnmcontainer.h ../misc/debug.h  
	cc -c tnmmatrix.cpp $(INCS) $(DEFS) $(FLAGS)

tnmmatrix_test.o : tnmmatrix_test.cpp tnmmatrix.h tnmmatrix.cpp  
	cc -c tnmmatrix_test.cpp $(INCS) $(DEFS) $(FLAGS)

tfunction0.o : tfunction0.h tfunction0.cpp ../misc/debug.h  
	cc -c tfunction0.cpp $(INCS) $(DEFS) $(FLAGS)

tnumericalderivative.o : tnumericalderivative.h tnumericalderivative.cpp tfunction0.h tderivatives0.h ../misc/debug.h  
	cc -c tnumericalderivative.cpp $(INCS) $(DEFS) $(FLAGS)

tderivatives0.o : tderivatives0.h tderivatives0.cpp tfunction0.h tfunctions.h tnmmatrix.h ../misc/debug.h  
	cc -c tderivatives0.cpp $(INCS) $(DEFS) $(FLAGS)

tfmatrix0.o : tfmatrix0.h tfmatrix0.cpp tderivatives0.h tconstant.h tnmmatrix.h ../misc/debug.h  
	cc -c tfmatrix0.cpp $(INCS) $(DEFS) $(FLAGS)

tfmatrix.o : tfmatrix.h tfmatrix.cpp tfmatrix0.h tderivatives0.h tconstant.h tnmmatrix.h ../misc/debug.h  
	cc -c tfmatrix.cpp $(INCS) $(DEFS) $(FLAGS)

tmathtools.o : tmathtools.h tmathtools.cpp ../misc/debug.h  
	cc -c tmathtools.cpp $(INCS) $(DEFS) $(FLAGS)



templates.o : templates.cpp 
	cc -c templates.cpp $(INCS) $(DEFS) $(FLAGS)


tadd.o : tadd.h tadd.cpp tfunction0.h ../misc/texception.h ../misc/debug.h
	cc -c tadd.cpp $(INCS) $(DEFS) $(FLAGS)

tsub.o : tsub.h tsub.cpp tbinary.h tfunction0.h ../misc/texception.h ../misc/debug.h
	cc -c tsub.cpp $(INCS) $(DEFS) $(FLAGS)

tmul.o : tmul.h tmul.cpp tbinary.h tfunction0.h ../misc/texception.h ../misc/debug.h
	cc -c tmul.cpp $(INCS) $(DEFS) $(FLAGS)

tdiv.o : tdiv.h tdiv.cpp tbinary.h tfunction0.h ../misc/texception.h ../misc/debug.h
	cc -c tdiv.cpp $(INCS) $(DEFS) $(FLAGS)


tatom.o : tatom.h tatom.cpp tfunction0.h tnmmatrix.h ../misc/debug.h  
	cc -c tatom.cpp $(INCS) $(DEFS) $(FLAGS)

tnegative.o : tnegative.h tnegative.cpp tderivatives0.h tatom.h tnmmatrix.h ../misc/debug.h  
	cc -c tnegative.cpp $(INCS) $(DEFS) $(FLAGS)

tsign.o : tsign.h tsign.cpp tfunction0.h tatom.h tnmmatrix.h ../misc/texception.h ../misc/debug.h  
	cc -c tsign.cpp $(INCS) $(DEFS) $(FLAGS)

treciprocal.o : treciprocal.h treciprocal.cpp tfunction0.h tatom.h tnmmatrix.h ../misc/texception.h ../misc/debug.h  
	cc -c treciprocal.cpp $(INCS) $(DEFS) $(FLAGS)

ttranslate.o : ttranslate.h ttranslate.cpp tfunction0.h tderivatives0.h tatom.h tnmmatrix.h ../misc/texception.h ../misc/debug.h  
	cc -c ttranslate.cpp $(INCS) $(DEFS) $(FLAGS)

tsquare.o : tsquare.h tsquare.cpp tfunction0.h tatom.h tnmmatrix.h ../misc/texception.h ../misc/debug.h  
	cc -c tsquare.cpp $(INCS) $(DEFS) $(FLAGS)

tsqrt.o : tsqrt.h tsqrt.cpp tfunction0.h tatom.h tnmmatrix.h ../misc/texception.h ../misc/debug.h  
	cc -c tsqrt.cpp $(INCS) $(DEFS) $(FLAGS)

tln.o : tln.h tln.cpp tfunction0.h tatom.h tnmmatrix.h ../misc/texception.h ../misc/debug.h  
	cc -c tln.cpp $(INCS) $(DEFS) $(FLAGS)

texp.o : texp.h texp.cpp tfunction0.h tatom.h tnmmatrix.h ../misc/texception.h ../misc/debug.h  
	cc -c texp.cpp $(INCS) $(DEFS) $(FLAGS)

tpow.o : tpow.h tpow.cpp tfunction0.h tbinary.h tnmmatrix.h ../misc/texception.h ../misc/debug.h  
	cc -c tpow.cpp $(INCS) $(DEFS) $(FLAGS)

tabs.o : tabs.h tabs.cpp tfunction0.h tatom.h tnmmatrix.h ../misc/texception.h ../misc/debug.h  
	cc -c tabs.cpp $(INCS) $(DEFS) $(FLAGS)

tsin.o : tsin.h tsin.cpp tfunction0.h tatom.h tnmmatrix.h ../misc/texception.h ../misc/debug.h  
	cc -c tsin.cpp $(INCS) $(DEFS) $(FLAGS)

tcos.o : tcos.h tcos.cpp tfunction0.h tatom.h tnmmatrix.h ../misc/texception.h ../misc/debug.h  
	cc -c tcos.cpp $(INCS) $(DEFS) $(FLAGS)

tsec.o : tsec.h tsec.cpp tfunction0.h tatom.h ttan.h  ../misc/texception.h ../misc/debug.h  
	cc -c tsec.cpp $(INCS) $(DEFS) $(FLAGS)

ttan.o : ttan.h ttan.cpp tfunction0.h tatom.h tsec.h  ../misc/texception.h ../misc/debug.h  
	cc -c ttan.cpp $(INCS) $(DEFS) $(FLAGS)

tarccos.o : tarccos.h tarccos.cpp tfunction0.h tatom.h tnmmatrix.h ../misc/texception.h ../misc/debug.h  
	cc -c tarccos.cpp $(INCS) $(DEFS) $(FLAGS)

tarcsin.o : tarcsin.h tarcsin.cpp tfunction0.h tatom.h tnmmatrix.h ../misc/texception.h ../misc/debug.h  
	cc -c tarcsin.cpp $(INCS) $(DEFS) $(FLAGS)

tarctan.o : tarctan.h tarctan.cpp tfunction0.h tatom.h tnmmatrix.h ../misc/texception.h ../misc/debug.h  
	cc -c tarctan.cpp $(INCS) $(DEFS) $(FLAGS)

tarcsec.o : tarcsec.h tarcsec.cpp tfunction0.h tatom.h tnmmatrix.h ../misc/texception.h ../misc/debug.h  
	cc -c tarcsec.cpp $(INCS) $(DEFS) $(FLAGS)

tfunctions.o : tfunctions.h tfunctions.cpp tderivatives0.h tnmmatrix.h ../misc/texception.h ../misc/debug.h  
	cc -c tfunctions.cpp $(INCS) $(DEFS) $(FLAGS)

#tderivative.o : tderivative.h tderivative.cpp tnmmatrix.h ../misc/texception.h ../misc/debug.h  
#	cc -c tderivative.cpp $(INCS) $(DEFS) $(FLAGS)

tintegral.o : tintegral.h tintegral.cpp tnmmatrix.h tderivatives0.h tfunction0.h ../misc/texception.h ../misc/debug.h  
	cc -c tintegral.cpp $(INCS) $(DEFS) $(FLAGS)

tgridfunction.o : tgridfunction.h tgridfunction.cpp tfog.h tnmmatrix.h ../misc/texception.h ../misc/debug.h  
	cc -c tgridfunction.cpp $(INCS) $(DEFS) $(FLAGS)

#tgridtaylor2.o : tgridtaylor2.h tgridtaylor2.cpp tgridfunction.h tfog.h tnmmatrix.h ../misc/texception.h ../misc/debug.h  
#	cc -c tgridtaylor2.cpp $(INCS) $(DEFS) $(FLAGS)

tselector.o : tselector.h tselector.cpp tderivatives0.h ../misc/texception.h ../misc/debug.h  
	cc -c tselector.cpp $(INCS) $(DEFS) $(FLAGS)

tmatrixalias.o : tmatrixalias.h tmatrixalias.cpp tderivatives0.h tnamedpointer.h  ../misc/texception.h ../misc/debug.h  
	cc -c tmatrixalias.cpp $(INCS) $(DEFS) $(FLAGS)

tparameters.o : tparameters.h tparameters.cpp tderivatives0.h tparameter.h  ../misc/texception.h ../misc/debug.h  
	cc -c tparameters.cpp $(INCS) $(DEFS) $(FLAGS)

tbinary.o : tbinary.h tbinary.cpp tfunction0.h ../misc/texception.h ../misc/debug.h  
	cc -c tbinary.cpp $(INCS) $(DEFS) $(FLAGS)

tfog.o : tfog.h tfog.cpp tfunction0.h tderivatives0.h ../misc/texception.h ../misc/debug.h  
	cc -c tfog.cpp $(INCS) $(DEFS) $(FLAGS)

tfog_test.o : tfog_test.cpp tfog.h tfog.cpp  
	cc -c tfog_test.cpp -std=gnu++0x $(INCS) $(DEFS) $(FLAGS)

tconstants.o : tconstants.h tconstants.cpp tfmatrix.h tfmatrix0.h  tderivatives0.h ../misc/texception.h ../misc/debug.h  
	cc -c tconstants.cpp $(INCS) $(DEFS) $(FLAGS)

tparameter.o : tparameter.h tparameter.cpp tnmmatrix.h tderivatives0.h ../misc/texception.h ../misc/debug.h  
	cc -c tparameter.cpp $(INCS) $(DEFS) $(FLAGS)

tnmaximizer.o : tnmaximizer.h tnmaximizer.cpp tnsolver.h tnmmatrix.h tderivatives0.h  ../misc/debug.h  
	cc -c tnmaximizer.cpp -std=gnu++0x $(INCS) $(DEFS) $(FLAGS)

tnmaximizer_test.o : tnmaximizer_test.cpp tnmaximizer.h tnmaximizer.cpp  
	cc -c tnmaximizer_test.cpp -std=gnu++0x $(INCS) $(DEFS) $(FLAGS)

tnsolver.o : tnsolver.h tnsolver.cpp tnmmatrix.h tderivatives0.h tfmatrix.h ../misc/debug.h  
	cc -c tnsolver.cpp -std=gnu++0x $(INCS) $(DEFS) $(FLAGS)

tintervalstepper.o : nsolver/tintervalstepper.h nsolver/tintervalstepper.cpp tnsolver.h tnmmatrix.h ../misc/debug.h  
	cc -c nsolver/tintervalstepper.cpp -std=gnu++0x $(INCS) $(DEFS) $(FLAGS)

tnsolver_test.o : tnsolver_test.cpp tnsolver.h tnsolver.cpp  
	cc -c tnsolver_test.cpp -std=gnu++0x $(INCS) $(DEFS) $(FLAGS)

tsolver.o : tsolver.h tsolver.cpp tnmmatrix.h tderivatives0.h tguesses.h ../misc/texception.h ../misc/debug.h  
	cc -c tsolver.cpp $(INCS) $(DEFS) $(FLAGS)

tsolver_test.o : tsolver_test.cpp tsolver.h tsolver.cpp  
	cc -c tsolver_test.cpp $(INCS) $(DEFS) $(FLAGS)

tlagrange.o : tlagrange.h tlagrange.cpp tnmmatrix.h tderivatives0.h tsolver.h  tguesses.h ../misc/texception.h ../misc/debug.h  
	cc -c tlagrange.cpp $(INCS) $(DEFS) $(FLAGS)

tguesses.o : tguesses.h tguesses.cpp tnmmatrix.h ../misc/texception.h ../misc/debug.h  
	cc -c tguesses.cpp $(INCS) $(DEFS) $(FLAGS)

tkuhntucker.o : tkuhntucker.h tkuhntucker.cpp tlagrange.h tsolver.h tguesses.h tnmmatrix.h ../misc/texception.h ../misc/debug.h  
	cc -c tkuhntucker.cpp $(INCS) $(DEFS) $(FLAGS)

toptimalcontrol.o : toptimalcontrol.h toptimalcontrol.cpp  tnmmatrix.h tfmatrix.h tfunction0.h tkuhntucker.h ../misc/texception.h ../misc/debug.h  
	cc -c toptimalcontrol.cpp $(INCS) $(DEFS) $(FLAGS)

tlinear.o : tlinear.h tlinear.cpp tderivatives0.h ../misc/texception.h ../misc/debug.h  
	cc -c tlinear.cpp $(INCS) $(DEFS) $(FLAGS)

tquadratic.o : tquadratic.h tquadratic.cpp tderivatives0.h ../misc/texception.h ../misc/debug.h  
	cc -c tquadratic.cpp $(INCS) $(DEFS) $(FLAGS)

tregression.o : tregression.h tregression.cpp tnmmatrix.h ../misc/texception.h ../misc/debug.h  
	cc -c tregression.cpp $(INCS) $(DEFS) $(FLAGS)

tregression_test.o : tregression_test.cpp tregression.h tregression.cpp  
	cc -c tregression_test.cpp $(INCS) $(DEFS) $(FLAGS)

tfitls.o : tfitls.h tfitls.cpp tregression.h tnmmatrix.h ../misc/texception.h ../misc/debug.h  
	cc -c tfitls.cpp -std=gnu++0x $(INCS) $(DEFS) $(FLAGS)

tfitls_test.o : tfitls_test.cpp tfitls.h tfitls.cpp  
	cc -c tfitls_test.cpp -std=c++0x $(INCS) $(DEFS) $(FLAGS)

tols.o : tols.h tols.cpp tregression.h tnmmatrix.h ../misc/texception.h ../misc/debug.h  
	cc -c tols.cpp $(INCS) $(DEFS) $(FLAGS)

tbinregression.o : tbinregression.h tbinregression.cpp tregression.o tsolver.h tnmmatrix.h ../misc/texception.h ../misc/debug.h  
	cc -c tbinregression.cpp $(INCS) $(DEFS) $(FLAGS)

tlogit.o : tlogit.h tlogit.cpp tbinregression.h tnmmatrix.h ../misc/texception.h ../misc/debug.h  
	cc -c tlogit.cpp $(INCS) $(DEFS) $(FLAGS)

tlogit_test.o : tlogit_test.cpp tbinregression.h tnmmatrix.h ../misc/texception.h ../misc/debug.h  
	cc -c tlogit_test.cpp $(INCS) $(DEFS) $(FLAGS)

tstat2.o : tstat2.h tstat2.cpp tnmmatrix.h ../misc/texception.h ../misc/debug.h  
	cc -c tstat2.cpp $(INCS) $(DEFS) $(FLAGS)

tmodel.o : tmodel.h tmodel.cpp tnmmatrix.h tkuhntucker.h ../misc/texception.h ../misc/debug.h  
	cc -c tmodel.cpp $(INCS) $(DEFS) $(FLAGS)

tplot.o : tplot.h tplot.cpp tmodel.h tnmmatrix.h ../misc/texception.h ../misc/debug.h  
	cc -c tplot.cpp $(INCS) $(DEFS) $(FLAGS)

linprog.o : linprog.h linprog.cpp tmatrix.h tvector.o ../misc/texception.h ../misc/debug.h  
	cc -c linprog.cpp $(INCS) $(DEFS) $(FLAGS)

simplex.o : simplex.h simplex.cpp linprog.o tmatrix.h tvector.o ../misc/texception.h ../misc/debug.h  
	cc -c simplex.cpp $(INCS) $(DEFS) $(FLAGS)

p4thplex.o : p4thplex.h p4thplex.cpp soplextools.o tmatrix.h tvector.o ../misc/texception.h ../misc/debug.h  
	cc -c p4thplex.cpp $(INCS) $(DEFS) $(FLAGS)

soplextools.o : soplextools.h soplextools.cpp tmatrix.h ../misc/texception.h ../misc/debug.h  
	cc -c soplextools.cpp $(INCS) $(DEFS) $(FLAGS)

tformula.o : tformula.h tformula.cpp tderivatives0.h  ../misc/texception.h ../misc/debug.h  
	cc -c tformula.cpp $(INCS) $(DEFS) $(FLAGS)

tnamedfunction.o : tnamedfunction.h tnamedfunction.cpp tderivatives0.h  ../misc/texception.h ../misc/debug.h  
	cc -c tnamedfunction.cpp $(INCS) $(DEFS) $(FLAGS)

tnamedpointer.o : tnamedpointer.h tnamedpointer.cpp tderivatives0.h  ../misc/texception.h ../misc/debug.h  
	cc -c tnamedpointer.cpp $(INCS) $(DEFS) $(FLAGS)

tinterval.o : tinterval.h tinterval.cpp   ../misc/texception.h ../misc/debug.h  
	cc -c tinterval.cpp $(INCS) $(DEFS) $(FLAGS)

tuniverse.o : tuniverse.h tuniverse.cpp tformula.h tderivatives0.h tfmatrix.h tfmatrix0.h  tnamedpointer.h ../misc/texception.h ../misc/debug.h  
	cc -c tuniverse.cpp $(INCS) $(DEFS) $(FLAGS)

tuniverse_test.o : tuniverse_test.cpp tuniverse.o  
	cc -c tuniverse_test.cpp $(INCS) $(DEFS) $(FLAGS)

tcommander.o : tcommander.h tcommander.cpp tcommand.h tdefinecommand.h tplotcommand.h tevaluatecommand.h tsolvecommand.h targmaxcommand.h tuniverse.h tformula.h tderivatives0.h tmathtools.h tplotcommand.h ../misc/texception.h ../misc/debug.h  
	cc -c tcommander.cpp $(INCS) $(DEFS) $(FLAGS)

tcommand.o : tcommand.h tcommand.cpp tuniverse.h tderivatives0.h  tmathtools.h ../misc/texception.h ../misc/debug.h  
	cc -c tcommand.cpp $(INCS) $(DEFS) $(FLAGS)

tevaluatecommand.o : tevaluatecommand.h tevaluatecommand.cpp tcommand.h tuniverse.h tderivatives0.h  tmathtools.h ../misc/texception.h ../misc/debug.h  
	cc -c tevaluatecommand.cpp $(INCS) $(DEFS) $(FLAGS)

tdefinecommand.o : tdefinecommand.h tdefinecommand.cpp tcommand.h tuniverse.h tderivatives0.h  tmathtools.h ../misc/texception.h ../misc/debug.h  
	cc -c tdefinecommand.cpp $(INCS) $(DEFS) $(FLAGS)

tplotcommand.o : tplotcommand.h tplotcommand.cpp tcommand.h tuniverse.h tcommander.h tderivatives0.h  tmathtools.h ../misc/texception.h ../misc/debug.h  
	cc -c tplotcommand.cpp $(INCS) $(DEFS) $(FLAGS)

tsolvecommand.o : tsolvecommand.h tsolvecommand.cpp tcommand.h tuniverse.h tderivatives0.h  tmathtools.h ../misc/texception.h ../misc/debug.h  
	cc -c tsolvecommand.cpp $(INCS) $(DEFS) $(FLAGS)

targmaxcommand.o : targmaxcommand.h targmaxcommand.cpp tcommand.h tuniverse.h tderivatives0.h tlagrange.h  tmathtools.h ../misc/texception.h ../misc/debug.h  
	cc -c targmaxcommand.cpp $(INCS) $(DEFS) $(FLAGS)

tmax.o : tmax.h tmax.cpp tfunction0.h ../misc/texception.h ../misc/debug.h  
	cc -c tmax.cpp $(INCS) $(DEFS) $(FLAGS)

tmin.o : tmin.h tmin.cpp tmax.h tfunction0.h ../misc/texception.h ../misc/debug.h  
	cc -c tmin.cpp $(INCS) $(DEFS) $(FLAGS)

tintervalfunction.o : tintervalfunction.h tintervalfunction.cpp tderivatives0.h tfunction0.h ../misc/texception.h ../misc/debug.h  
	cc -c tintervalfunction.cpp $(INCS) $(DEFS) $(FLAGS)
signalhandler.o : signalhandler.h signalhandler.cpp  ../misc/debug.h  
	cc -c signalhandler.cpp $(INCS) $(DEFS) $(FLAGS)


clean:
	rm -f $(STATIC_LIB)
	rm -f $(DYNAMIC_LIB)
	rm -f *.o
	rm -f *.a

all:
	make 
