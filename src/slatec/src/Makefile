FLAGS = -g -fpic -rdynamic -fno-stack-protector
#  -pg -static


libmthcslatec.a: cslatec.h  
	gcc -g  -xc -fpic -c *.c $(FLAGS)
	ar cru libmthcslatec.a *.o
	ranlib libmthcslatec.a

cslatec.h: slatec_src.tgz 
	f2c -C -a -P -trapuv *.f
	cat *.P > cslatec.h
	cat *.P | sed 's/extern/& "C"'/ > cslatec.h

slatec_src.tgz:
	wget http://www.netlib.org/slatec/slatec_src.tgz
	tar xvzf slatec_src*.tgz
	mv src/*.f ./
	rm -rf src


clean:
	rm -f *.a
	rm -f *.f
	rm -f *.P
	rm -f *.o
	rm -f *.c
	rm -f *.tgz
	rm -f slatec_src*
	rm -f cslatec.h
