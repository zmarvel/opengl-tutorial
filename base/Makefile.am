

bin_PROGRAMS = opengl_base
opengl_base_SOURCES = \
	src/main.cpp \
	src/logging.cpp \
	src/glad.c

AM_CPPFLAGS = -Iinclude

AM_CFLAGS = @gllibs_CFLAGS@
AM_CXXFLAGS = @gllibs_CFLAGS@
AM_LDFLAGS = @gllibs_LIBS@ -lm -ldl
