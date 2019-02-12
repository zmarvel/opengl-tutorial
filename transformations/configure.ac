

AC_INIT([opengl_base],[0.1],[zpmarvel@gmail.com])
AC_CONFIG_SRCDIR([src/main.cpp])
AM_INIT_AUTOMAKE([foreign subdir-objects])
AM_MAINTAINER_MODE([enable])

AC_PROG_CC
AC_PROG_CXX
dnl This is required for the AC_CHECK_HEADER tests
AC_LANG(C++)

PKG_CHECK_MODULES([gllibs],[gl glfw3 >= 3.0 glm])

dnl libfreeimage(plus) doesn't support pkg-config :(
dnl AC_CHECK_HEADER(FreeImagePlus.h,[],
dnl                 [AC_MSG_ERROR([could not find FreeImagePlus header])],
dnl                 [])
dnl AC_SEARCH_LIBS(FreeImage_GetInfo,[freeimageplus],[],
dnl                [AC_MSG_ERROR([could not find libfreeimageplus])],
dnl                [-lfreeimage])

dnl libpng++ doesn't support pkg-config :(
AC_CHECK_HEADER(png++/png.hpp,
                [AC_SUBST(pngpp_LIBS,"-lpng")],
                [AC_MSG_ERROR([could not find libpng++ header])],
                [])


AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
