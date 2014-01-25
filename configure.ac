dnl ====================================================================
dnl   configure.in for URANIA
dnl ====================================================================

AC_PREREQ([2.69])
AC_INIT(urania, 0.6.0, [acy@hiemalis.org])
AC_CONFIG_SRCDIR([wmain.cpp])
AC_CONFIG_HEADER([config.h])
AM_INIT_AUTOMAKE


dnl =======================================
dnl   Checks for programs.
dnl =======================================
AC_PROG_CC
AC_PROG_CXX

AC_PROG_RANLIB
AC_PROG_YACC
AM_PROG_LEX


dnl ===================================
dnl   Checks for libraries.
dnl ===================================


dnl ===================================
dnl   Checks for header files.
dnl ===================================


dnl ==================================================================
dnl   Checks for typedefs, structures, and compiler characteristics.
dnl ==================================================================
AC_C_CONST
AC_C_INLINE


dnl ==================================================================
dnl   Checks for library functions.
dnl ==================================================================
AC_HEADER_STDC
AC_CHECK_FUNCS([memset])


dnl ==============================
CFLAGS="-DWINVER=0x500 -D_UNICODE -DUNICODE $CFLAGS"
CXXFLAGS="-DWINVER=0x500 -D_UNICODE -DUNICODE $CXXFLAGS"


dnl ==================================================================
dnl   Enable ShittJIS source code
dnl   option : --enable-sjissrc
dnl ==================================================================
AC_ARG_ENABLE(sjissrc,
[  --enable-sjissrc        enable SJIS source code[default no]])
if test "$enable_sjissrc" = "yes"; then
	CFLAGS="-finput_charset=cp932 $CFLAGS"
	CXXFLAGS="-finput_charset=cp932 $CXXFLAGS"
fi


dnl ==================================================================
dnl   Enable ShittJIS output
dnl   option : --enable-sjisout
dnl ==================================================================
AC_ARG_ENABLE(sjisout,
[  --enable-sjisout        enable SJIS output[default no]])
if test "$enable_sjis_out" = "yes"; then
	CFLAGS="-finput_charset=cp932 $CFLAGS"
	CXXFLAGS="-finput_charset=cp932 $CXXFLAGS"
fi


dnl ==================================================================
dnl   Without cygwin
dnl   option : --without-cygwin
dnl ==================================================================
AC_ARG_WITH(cygwin,
[  --without-cygwin        add option -mno-cygwin])
if test "$with_cygwin" = "no"; then
	CFLAGS="-mno-cygwin $CFLAGS"
	CXXFLAGS="-mno-cygwin $CXXFLAGS"
	LDFLAGS="-mno-cygwin $LDFLAGS"
fi




dnl =====================================
dnl   Output
dnl =====================================
AC_CONFIG_FILES([Makefile])
AC_OUTPUT


dnl EOF
