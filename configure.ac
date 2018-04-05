dnl Process this file with autoconf to produce configure script

AC_INIT([effz], [0.1], [olegskor@gmail.com])
AM_INIT_AUTOMAKE([foreign])

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_SRCDIR([src/effz_zeroth_order.cpp])
AC_CONFIG_FILES([
				 Makefile
				 src/Makefile
				 man/Makefile])

dnl Switch to a C++ compiler and check if it works.
AC_LANG([C++])
AC_PROG_CXX
AC_PROG_CXXCPP
m4_include([m4/m4_ax_cxx_compile_stdcxx.m4])
AX_CXX_COMPILE_STDCXX(14)

dnl c++ standard lib headers
AC_CHECK_HEADERS(m4_normalize(iostream vector array string cmath memory
				 functional utility unordered_map complex
				 cstdlib type_traits numeric algorithm
				 stdexcept locale codecvt regex tuple unordered_set
				 iterator sstream exception fstream system_error
				 cerrno), [],
				 [AC_MSG_ERROR([standard library headers are not found])])

dnl check for gsl
AC_ARG_WITH([gsl-prefix],
			[AS_HELP_STRING([--with-gsl-prefix=prefix],
							[non standard install of gsl])],
							[gsl_prefix_set=yes],
							[gsl_prefix_set=no])

GSLPREFIX=
GSLCPPFLAGS=
GSLLDFLAGS=
AC_SUBST(GSLPREFIX,"")
AC_SUBST(GSLCPPFLAGS,"")
AC_SUBST(GSLLDFLAGS,"")
AS_IF([test "x$gsl_prefix_set" = xyes],
	  [
	   AC_SUBST(GSLPREFIX,["$with_gsl_prefix"])
	   AC_SUBST(GSLCPPFLAGS,["-I$GSLPREFIX/include"])
	   AC_SUBST(GSLLDFLAGS,["-L$GSLPREFIX/lib"])
	   ],
	   [])

dnl save cppflags
ac_save_CPPFLAGS=$CPPFLAGS
CPPFLAGS="$GSLCPPFLAGS $CPPFLAGS"

dnl check gsl headers
AC_CHECK_HEADERS(m4_normalize(gsl/gsl_math.h gsl/gsl_errno.h
				 gsl/gsl_integration.h gsl/gsl_monte.h
				 gsl/gsl_sf_result.h gsl/gsl_sf_coupling.h
				 gsl/gsl_sf_coulomb.h gsl/gsl_sf_legendre.h), [],
				 [AC_MSG_ERROR([gsl headers are not found])])

dnl restore cppflags
CPPFLAGS=$ac_save_CPPFLAGS

dnl save ldflags
ac_save_LDFLAGS=$LDFLAGS
LDFLAGS="$GSLLDFLAGS $LDFLAGS"

dnl check to link with libraries
dnl -lm
AC_CHECK_LIB([m],[cos], [], [AC_MSG_ERROR([no cos() in -lm])])
dnl -lgslcblas
AC_CHECK_LIB([gslcblas],[cblas_dgemm], [],
			 [AC_MSG_ERROR([gslcblas is not working])])
dnl -lgsl
AC_CHECK_LIB([gsl],[gsl_blas_dgemm], [],
			 [AC_MSG_ERROR([gsl is not working])])
dnl restore ldflags
LDFLAGS=$ac_save_LDFLAGS

dnl intel tbb
AC_ARG_WITH([tbb-prefix],
			[AS_HELP_STRING([--with-tbb-prefix=prefix],
							[non standard install of intel tbb])],
							[tbb_prefix_set=yes],
							[tbb_prefix_set=no])

TBBPREFIX=
TBBCPPFLAGS=
TBBLDFLAGS=
AC_SUBST(TBBPREFIX,"")
AC_SUBST(TBBCPPFLAGS,"")
AC_SUBST(TBBLDFLAGS,"")
AS_IF([test "x$tbb_prefix_set" = xyes],
	  [
	   AC_SUBST(TBBPREFIX,["$with_tbb_prefix"])
	   AC_SUBST(TBBCPPFLAGS,["-I$TBBPREFIX/include"])
	   AC_SUBST(TBBLDFLAGS,["-L$TBBPREFIX/lib"])
	   ],
	   [])
dnl save cppflags
ac_save_CPPFLAGS=$CPPFLAGS
CPPFLAGS="$TBBCPPFLAGS $CPPFLAGS"
AC_CHECK_HEADERS([tbb/tbb.h], [],
				 [AC_MSG_ERROR([intel tbb.h headers are not found])])
dnl restore cppflags
CPPFLAGS=$ac_save_CPPFLAGS
dnl save ldflags
ac_save_LDFLAGS=$LDFLAGS
LDFLAGS="$TBBLDFLAGS $LDFLAGS"
dnl -ltbb
AC_CHECK_LIB([tbb], [TBB_runtime_interface_version], [],
			 [AC_MSG_ERROR([Intel tbb is not working])])

dnl restore ldflags
LDFLAGS=$ac_save_LDFLAGS

dnl python
dnl check for python
AM_PATH_PYTHON([3.4])
AS_IF([test "x$PYTHON_VERSION" = "x3.4"],
	  [AC_DEFINE_UNQUOTED([EFFZ_PYTHON_VERSION_FOUR],[$PYTHON_VERSION],
						  [define python version 3.4]
						  )
	  ],
	  [test "x$PYTHON_VERSION" = "x3.5"],
	  [AC_DEFINE_UNQUOTED([EFFZ_PYTHON_VERSION_FIVE],[$PYTHON_VERSION],
						  [define python version 3.5])
	  ],
	  [test "x$PYTHON_VERSION" = "x3.6"],
	  [AC_DEFINE_UNQUOTED([EFFZ_PYTHON_VERSION_SIX],[$PYTHON_VERSION],
						  [define python version 3.6])
	  ],
	  [AC_DEFINE_UNQUOTED([EFFZ_PYTHON_VERSION_OTHER],[$PYTHON_VERSION],
						  [define python version other])
	  ])
dnl export shell variables to build and link with python
dnl python prefix
AC_ARG_VAR(PYTHONPREFIX, [python prefix path. The default value is obtained from `python3-config --prefix`])
dnl python include
AC_ARG_VAR(PYTHONINCLUDE, [python include path, i.e. -I<path to Python.h>. The default value is `python3-config --includes`])
dnl python ldflags
AC_ARG_VAR(PYTHONLDFLAGS, [python lib path, i.e. -L<path to python libs>. The default value is `python3-config --ldflags`])
dnl python libs
AC_ARG_VAR(PYTHONLIBS, [python libs to link with, i.e. -l<python libs>. The default value is `python3-config --libs`])
dnl check if python3-config is present and define PYTHONPREFIX
AC_CHECK_PROG(py3_config_exists, python3-config, yes, [no])
AS_IF([test "x$py3_config_exists" != xno],
	  [AC_SUBST(PYTHONPREFIX,["`python3-config --prefix`"])],
	  [AC_MSG_ERROR(python3-config is not found in PATH. Please read ./configure -h and specify python shell variables manually)])
dnl define PYTHONINCLUDE
AC_SUBST(PYTHONINCLUDE,["`python3-config --includes`"])
dnl define PYTHONLDFLAGS
AC_SUBST(PYTHONLDFLAGS,["`python3-config --ldflags`"])
dnl define PYTHONLIBS
AC_SUBST(PYTHONLIBS,["`python3-config --libs`"])
dnl check if python.h is present
ac_save_CPPFLAGS=$CPPFLAGS
CPPFLAGS="$CPPFLAGS $PYTHONINCLUDE"
AC_CHECK_HEADERS([Python.h], [], AC_MSG_ERROR([not found Python.h]))
CPPFLAGS=$ac_save_CPPFLAGS
dnl python program test
ac_save_CPPFLAGS=$CPPFLAGS
CPPFLAGS="$CPPFLAGS $PYTHONINCLUDE"
ac_save_LDFLAGS=$LDFLAGS
LDFLAGS="$LDFLAGS $PYTHONLDFLAGS"
ac_save_LIBS=$LIBS
LIBS="$LIBS $PYTHONLIBS"
AC_LINK_IFELSE([AC_LANG_PROGRAM(
				[[#ifdef HAVE_PYTHON_H
				 #include<Python.h>
				 #endif
				 ]],
				 [[Py_Initialize();
				  if(PyRun_SimpleString("import sys\nsys.hexversion")!=0){
					  return 1;
				  } else {return -1;}
				  #ifdef EFFZ_PYTHON_VERSION_SIX
				  if(Py_FinalizeEx() < 0){
					  return 120;
				  }
				  #elif defined(EFFZ_PYTHON_VERSION_FIVE) || defined(EFFZ_PYTHON_VERSION_FOUR)
				  Py_Finalize();
				  #endif
				  ]])], [],
				  [AC_MSG_FAILURE([fail link with python])])
CPPFLAGS=$ac_save_CPPFLAGS
LDFLAGS=$ac_save_LDFLAGS
LIBS=$ac_save_LIBS
dnl define zeroth order
AC_DEFINE([EFFZ_ZEROTH_ORDER],[1],[define zeroth order])
dnl define home user directory
AC_ARG_WITH([effz-home],
			[AS_HELP_STRING([--with-effz-home=prefix],
							[dir to store program data [$HOME/.config]])],
							[effz_home_set=yes],
							[effz_home_set=no])

AS_IF([test "x$effz_home_set" = xyes],
	  [
	   AC_DEFINE_UNQUOTED([EFFZ_HOME_DIRECTORY],["$with_effz_home"],
						  [define home folder])
	   ],
	   [
		AC_DEFINE_UNQUOTED([EFFZ_HOME_DIRECTORY],["$HOME/.config"],
						   [define home folder])
		])
dnl enable debug mainteriner option only
AC_MSG_CHECKING([whether to build with debug output])
AC_ARG_ENABLE([debug],
			  [AS_HELP_STRING([--enable-debug],
							  [enable debug outputs (def=no)])
			  ],
			  [effz_debug="$enable_debug"],
			  [effz_debug=no])
AC_MSG_RESULT([$effz_debug])
AS_IF([test "x$effz_debug" = xyes],
	  [AC_DEFINE([DEBUG],[],[debug mode])],
	  [test "x$effz_debug" = xno],
	  [AC_DEFINE([NDEBUG],[],[no-debug mode])])
AC_OUTPUT
