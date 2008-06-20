AC_INIT([tpadmin-cpp],[0.0.1],[mavrinac@gmail.com])
AC_CONFIG_SRCDIR(tpadmin/main.cpp)

AC_CANONICAL_TARGET([])

AM_INIT_AUTOMAKE(tpadmin-cpp, 0.0.1)
AM_CONFIG_HEADER(config.h)

AM_MAINTAINER_MODE

CXXFLAGS="-g -Wall"


AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_MAKE_SET

AC_LANG_CPLUSPLUS
AC_REQUIRE_CPP

AC_HEADER_STDC

AC_CHECK_HEADERS(signal.h)
    
PKG_CHECK_MODULES(TPRL, libtprl >= 0.1.2,
    [
        CXXFLAGS="$CXXFLAGS $TPRL_CFLAGS"
        LDFLAGS="$LDFLAGS $TPRL_LIBS"
    ],
    AC_MSG_ERROR(Couldn't find libtprl. Please install it before trying again)
)

PKG_CHECK_MODULES(TP, libtpproto >= 0.1.0,
	[
        CXXFLAGS="$CXXFLAGS $TP_CFLAGS"
        LDFLAGS="$LDFLAGS $TP_LIBS"
	],
	AC_MSG_ERROR(Couldn't find libtpproto-cpp. Please install it before trying again)
)

AC_CONFIG_FILES([
Makefile
tpadmin/Makefile
]) 

AC_OUTPUT
