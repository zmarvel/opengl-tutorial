

bin_PROGRAMS = opengl_base
opengl_base_SOURCES = \
	src/main.cpp \
	src/logging.cpp \
	src/gl_program.cpp \
	src/glad.c

AM_CPPFLAGS = -Iinclude

AM_CFLAGS = @gllibs_CFLAGS@
AM_CXXFLAGS = -std=c++14 @gllibs_CFLAGS@
AM_LDFLAGS = @gllibs_LIBS@ @pngpp_LIBS@ -lm -ldl
