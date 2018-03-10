if enable_swig
TESTS = \
	test_ccy_lookups.pl \
	test_bootstrap_yld.pl
else
TESTS =
endif

TESTS_ENVIRONMENT = LD_LIBRARY_PATH=../../src/apps/swig/perl5/.libs PERL5LIB=../../src/apps/swig/perl5/

EXTRA_DIST = $(TESTS) rqlib.pm
