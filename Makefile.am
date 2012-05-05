# Any copyright is dedicated to the Public Domain.
# http://creativecommons.org/publicdomain/zero/1.0/

ACLOCAL_AMFLAGS = -I m4
AUTOMAKE_OPTIONS = foreign subdir-objects

lib_LTLIBRARIES = libcin.la
libcin_la_SOURCES = \
	src/trie.h \
	src/trie.c
libcin_la_LDFLAGS = \
	-version-info $(LIBCIN_CURRENT):$(LIBCIN_REVISION):$(LIBCIN_AGE) \
	-export-symbols $(top_srcdir)/libcin.sym
libcin_la_DEPENDENCIES = $(top_srcdir)/libcin.sym
