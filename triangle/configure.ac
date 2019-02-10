

AC_INIT([opengl_base],[0.1],[zpmarvel@gmail.com])
AC_CONFIG_SRCDIR([src/main.cpp])
AM_INIT_AUTOMAKE([foreign subdir-objects])
AM_MAINTAINER_MODE([enable])

AC_PROG_CC
AC_PROG_CXX

PKG_CHECK_MODULES([gllibs],[gl glfw3 >= 3.0])

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
