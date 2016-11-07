# Note that we cannot use: AUTOMAKE_OPTIONS = subdir-objects
# subdir-objects will compile the source files to a single version of Python.
# Since subdir-objects is being deprecated we copy the source files instead.
am__installdirs = "$(DESTDIR)$(pyexecdir2)"
pyexecdir = $(pyexecdir2)

if HAVE_PYTHON2
AM_CFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCDATA_CPPFLAGS@ \
	@LIBCLOCALE_CPPFLAGS@ \
	@LIBCSPLIT_CPPFLAGS@ \
	@LIBUNA_CPPFLAGS@ \
	@LIBCFILE_CPPFLAGS@ \
	@LIBCPATH_CPPFLAGS@ \
	@LIBBFIO_CPPFLAGS@

am_pyevt_la_rpath = -rpath $(pyexecdir2)

BUILT_SOURCES = \
	pyevt.c pyevt.h \
	pyevt_codepage.c pyevt_codepage.h \
	pyevt_datetime.c pyevt_datetime.h \
	pyevt_error.c pyevt_error.h \
	pyevt_event_types.c pyevt_event_types.h \
	pyevt_file.c pyevt_file.h \
	pyevt_file_flags.c pyevt_file_flags.h \
	pyevt_file_object_io_handle.c pyevt_file_object_io_handle.h \
	pyevt_integer.c pyevt_integer.h \
	pyevt_libbfio.h \
	pyevt_libcerror.h \
	pyevt_libclocale.h \
	pyevt_libevt.h \
	pyevt_python.h \
	pyevt_record.c pyevt_record.h \
	pyevt_records.c pyevt_records.h \
	pyevt_strings.c pyevt_strings.h \
	pyevt_unused.h

pyexec_LTLIBRARIES = pyevt.la

nodist_pyevt_la_SOURCES = $(BUILT_SOURCES)

pyevt_la_LIBADD = \
	@LIBCERROR_LIBADD@ \
	../libevt/libevt.la \
	@LIBCDATA_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBBFIO_LIBADD@

pyevt_la_CPPFLAGS = $(PYTHON2_CPPFLAGS)
pyevt_la_LDFLAGS  = -module -avoid-version $(PYTHON2_LDFLAGS)

$(BUILT_SOURCES):
	/bin/cp -f $(top_srcdir)/pyevt/$@ $@

endif

MAINTAINERCLEANFILES = \
	Makefile.in

clean-local:
	/bin/rm -f pyevt*.[ch]

distclean: clean
	/bin/rm -f Makefile

