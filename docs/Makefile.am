MAINTAINERCLEANFILES = Makefile.in

if enable_docs
TARGET = riskquantify.texi
else
TARGET =
endif

ALL: $(TARGET)
# riskquantify_funcs.texi

EXTRA_DIST = riskquantify.doxyfile

info_TEXINFOS = riskquantify.texi

riskquantify_TEXINFOS =

#riskquantify_TEXINFOS = riskquantify_funcs.texi

#riskquantify_funcs.texi: riskquantify.doxyfile
#	doxygen riskquantify.doxyfile
#	../bin/rq_texinfo_funclist_create.sh


SUFFIXES = .html

html_docs = riskquantify.html

.texi.html:
	$(MAKEINFO) --html $<

.PHONY: html
html: version.texi $(html_docs)
