if HAVE_PYTHON
AM_CFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCSTRING_CPPFLAGS@ \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCLOCALE_CPPFLAGS@ \
	@LIBUNA_CPPFLAGS@ \
	@LIBCFILE_CPPFLAGS@ \
	@LIBCPATH_CPPFLAGS@ \
	@LIBBFIO_CPPFLAGS@ \
	@LIBFWNT_CPPFLAGS@ \
	@MEMWATCH_CPPFLAGS@

pyexec_LTLIBRARIES = pyevt.la

pyevt_la_SOURCES = \
	pyevt.c pyevt.h \
	pyevt_codepage.c pyevt_codepage.h \
	pyevt_datetime.c pyevt_datetime.h \
	pyevt_file.c pyevt_file.h \
	pyevt_file_object_io_handle.c pyevt_file_object_io_handle.h \
	pyevt_libbfio.h \
	pyevt_libcerror.h \
	pyevt_libclocale.h \
	pyevt_libcstring.h \
	pyevt_libevt.h \
	pyevt_libfwnt.h \
	pyevt_python.h \
	pyevt_record.c pyevt_record.h

pyevt_la_LIBADD = \
	@LIBCSTRING_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	../libevt/libevt.la \
	@LIBCLOCALE_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBBFIO_LIBADD@ \
	@LIBFWNT_LIBADD@

pyevt_la_CPPFLAGS = $(PYTHON_CPPFLAGS)
pyevt_la_LDFLAGS  = -module -avoid-version $(PYTHON_LDFLAGS)

endif

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

