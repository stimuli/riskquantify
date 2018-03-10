TESTS = \
	test_vector \
	test_stream \
	test_type_id_mgr \
	test_calendar \
	test_monte_carlo \
	test_interpreter \
	test_asset_mgr \
	test_spot_price_mgr \
	test_forward_curve

bin_PROGRAMS = \
	test_vector \
	test_stream \
	test_type_id_mgr \
	test_calendar \
	test_monte_carlo \
	test_interpreter \
	test_asset_mgr \
	test_spot_price_mgr \
	test_forward_curve

test_monte_carlo_SOURCES = \
	test_monte_carlo.c

#test_finite_differences_SOURCES = \
#	test_finite_differences.c

test_vector_SOURCES = \
	test_vector.c

test_stream_SOURCES = \
	test_stream.c

test_type_id_mgr_SOURCES = \
	test_type_id_mgr.c

test_calendar_SOURCES = \
	test_calendar.c

test_interpreter_SOURCES = \
	test_interpreter.c

test_asset_mgr_SOURCES = \
	test_asset_mgr.c

test_spot_price_mgr_SOURCES = \
	test_spot_price_mgr.c

test_forward_curve_SOURCES = \
	test_forward_curve.c

CFLAGS = -I$(srcdir)/../../src/rq -g
LDADD = ../../src/rq/librq.a -lm
AM_LDFLAGS = -g

