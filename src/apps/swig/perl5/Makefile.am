if enable_swig
TARGET = librqperl.la
SRCS = rq_wrap.c
else
TARGET =
SRCS = 
endif

lib_LTLIBRARIES = $(TARGET)

librqperl_la_SOURCES = $(SRCS)

PERL_INC = @archlibexp@

##noinst_SCRIPTS = rq_wrap.c

##rq_la_LIBADD = ../../../rq/librq.a -L/usr/lib/perl5/5.6.1/i386-linux/CORE/ -lperl -lcrypt -lm
librqperl_la_LIBADD = ../../../rq/librq.a
CFLAGS = -I${PERL_INC}/CORE -I../../../rq

rq_wrap.c: ../librq.i
	swig -I../../../rq -perl5 -o rq_wrap.c ../librq.i

CLEANFILES = rq_wrap.c
