# Autoconf processes this file to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([glowing-tribble],[0.1],[orionphoenix93@gmail.com])
AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
	Makefile 
	src/Makefile
])

# Place weird scripts generated by autoreconf in auxiliary directory
AC_CONFIG_AUX_DIR([build-aux])

# Initialize automake
AM_INIT_AUTOMAKE([foreign])
AM_MAINTAINER_MODE([enable])

# Checks for programs
AC_PROG_CC

# Checks for libraries
PKG_CHECK_MODULES([gllibs], [gl glew glfw3 >= 3.0])

# Checks for header files

AC_OUTPUT
