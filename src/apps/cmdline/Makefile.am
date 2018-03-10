SUBDIRS = msvc

bin_PROGRAMS = \
	rq_price

rq_price_SOURCES = \
	bh_strcpy.c \
	bh_getopt.c \
	rq_price.c

pkginclude_HEADERS = \
	bh_strcpy.h \
	bh_getopt.h

CFLAGS = -I$(srcdir)/../../rq
LDADD = ../../rq/librq.a -lm
