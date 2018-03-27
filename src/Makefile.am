
bin_PROGRAMS = effz

effz_SOURCES = effz_atomic_data.cpp effz_exceptions.cpp\
			   effz_main_loop_functions.cpp effz_python_utility.cpp\
			   effz_result.cpp effz_spec_func.cpp effz_user_input.cpp\
			   effz_utility.cpp effz_zeroth_order.cpp\
			   effz_zeroth_order_python.cpp grid.cpp\
			   integrate_nd.cpp main.cpp monte_complex.cpp

effz_CPPFLAGS = -I$(top_srcdir)/src -I@PYTHONINCLUDE@\
				@GSLCPPFLAGS@ @TBBCPPFLAGS@

effz_LDFLAGS = -L@PYTHONLDFLAGS@ @GSLLDFLAGS@ @TBBLDFLAGS@

effz_LDADD = @PYTHONLIBS@
