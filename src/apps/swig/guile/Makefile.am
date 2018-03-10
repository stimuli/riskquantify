if enable_swig
TARGET = librqguile.la
SRCS = rq_wrap.c
else
TARGET =
SRCS = 
endif

lib_LTLIBRARIES = $(TARGET)

librqguile_la_SOURCES = $(SRCS)

librqguile_la_LIBADD = ../../../rq/librq.a
CFLAGS = -I${PERL_INC}/CORE -I../../../rq

rq_wrap.c: ../librq.i
	swig -I../../../rq -guile -o rq_wrap.c ../librq.i

CLEANFILES = rq_wrap.c
