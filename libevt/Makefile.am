AM_CPPFLAGS = \
	-DLOCALEDIR=\"$(datadir)/locale\" \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCTHREADS_CPPFLAGS@ \
	@LIBCDATA_CPPFLAGS@ \
	@LIBCLOCALE_CPPFLAGS@ \
	@LIBCNOTIFY_CPPFLAGS@ \
	@LIBCSPLIT_CPPFLAGS@ \
	@LIBUNA_CPPFLAGS@ \
	@LIBCFILE_CPPFLAGS@ \
	@LIBCPATH_CPPFLAGS@ \
	@LIBBFIO_CPPFLAGS@ \
	@LIBFCACHE_CPPFLAGS@ \
	@LIBFDATA_CPPFLAGS@ \
	@LIBFDATETIME_CPPFLAGS@ \
	@LIBFGUID_CPPFLAGS@ \
	@LIBFVALUE_CPPFLAGS@ \
	@LIBFWNT_CPPFLAGS@ \
	@PTHREAD_CPPFLAGS@

lib_LTLIBRARIES = libevt.la

libevt_la_SOURCES = \
	evt_file_header.h \
	evt_record.h \
	libevt.c \
	libevt_codepage.h \
	libevt_debug.c libevt_debug.h \
	libevt_definitions.h \
	libevt_error.c libevt_error.h \
	libevt_extern.h \
	libevt_file.c libevt_file.h \
	libevt_io_handle.c libevt_io_handle.h \
	libevt_libbfio.h \
	libevt_libcdata.h \
	libevt_libcerror.h \
	libevt_libclocale.h \
	libevt_libcnotify.h \
	libevt_libfcache.h \
	libevt_libfdata.h \
	libevt_libfdatetime.h \
	libevt_libfvalue.h \
	libevt_libfwnt.h \
	libevt_libuna.h \
	libevt_notify.c libevt_notify.h \
	libevt_record.c libevt_record.h \
	libevt_record_values.c libevt_record_values.h \
	libevt_support.c libevt_support.h \
	libevt_types.h \
	libevt_unused.h

libevt_la_LIBADD = \
	@LIBCERROR_LIBADD@ \
	@LIBCTHREADS_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBBFIO_LIBADD@ \
	@LIBFCACHE_LIBADD@ \
	@LIBFDATA_LIBADD@ \
	@LIBFDATETIME_LIBADD@ \
	@LIBFGUID_LIBADD@ \
	@LIBFVALUE_LIBADD@ \
	@LIBFWNT_LIBADD@ \
	@PTHREAD_LIBADD@

libevt_la_LDFLAGS = -no-undefined -version-info 1:0:0

EXTRA_DIST = \
	libevt_definitions.h.in \
	libevt.rc \
	libevt.rc.in

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f libevt_definitions.h
	/bin/rm -f libevt.rc
	/bin/rm -f Makefile

splint:
	@echo "Running splint on libevt ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(libevt_la_SOURCES)

