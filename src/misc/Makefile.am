lib_LTLIBRARIES = libmthmisc.la
libmthmisc_la_LDFLAGS = -module -shared
libmthmisc_la_SOURCES = \
        debug.cpp \
        stlstringtools.cpp \
        tnmcontainer.cpp \
        linkedptrlistpost.cpp \
	linkedptrlist.cpp \
        rand.cpp randomtools.cpp \
        timetools.cpp \
        tpointerstack.cpp \
        linkedlist.cpp linkedlistpost.cpp \
        option0.cpp options.cpp toption.cpp tptroption.cpp \
        filetools.cpp

#       clocker.cpp \
#       xmlparser.cpp xmlfinder.cpp \
#       tracer.cpp tracertag.cpp tracerstring.cpp \
#       timers.cpp timer.cpp timer0.cpp timestamp.cpp 


AM_CPPFLAGS = \
	-I/usr/include \
        -I./ \
	-L/usr/lib \
        -L./ \
        -L../libs \
        -lstdc++ \
        -lm \
	-g -fno-default-inline -fpic -rdynamic -std=gnu++0x -D_GSL

#-D_SLATEC

#AM_LDFLAGS =
