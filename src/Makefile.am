
bin_PROGRAMS = effz

effz_SOURCES = effz_main_loop_functions.cpp\
			   effz_result.cpp effz_user_input.cpp\
			   main.cpp\
			   \
			   effz_main_loop_functions.h effz_result.h\
			   effz_user_input.h

effz_CPPFLAGS = -I$(top_srcdir)/effz_lib\
				-I$(top_srcdir)/src\
				@PYTHONINCLUDE@\
				@GSLCPPFLAGS@ @TBBCPPFLAGS@#\
				#-DEFFZ_PYTHON_SRC_DIR='"$(effzpythondir)"'

effz_LDFLAGS = @PYTHONLDFLAGS@ @GSLLDFLAGS@ @TBBLDFLAGS@

effz_LDADD = effz_lib/libeffzlib.a @PYTHONLIBS@
