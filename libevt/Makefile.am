AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	-I$(top_srcdir)/libcstring \
	-I$(top_srcdir)/liberror \
	-I$(top_srcdir)/libnotify \
	@LIBUNA_CPPFLAGS@ \
	@LIBBFIO_CPPFLAGS@ \
	@LIBFDATETIME_CPPFLAGS@ \
	@LIBFGUID_CPPFLAGS@ \
	@LIBFVALUE_CPPFLAGS@ \
	@LIBFWNT_CPPFLAGS@ \
	@MEMWATCH_CPPFLAGS@

lib_LTLIBRARIES = libevt.la

libevt_la_SOURCES = \
	libevt.c \
	libevt_array_type.c libevt_array_type.h \
	libevt_codepage.h \
	libevt_debug.c libevt_debug.h \
	libevt_definitions.h \
	libevt_error.c libevt_error.h \
	libevt_extern.h \
	libevt_file.c libevt_file.h \
	libevt_item.c libevt_item.h \
	libevt_io_handle.c libevt_io_handle.h \
	libevt_libbfio.h \
	libevt_libfdatetime.h \
	libevt_libfvalue.h \
	libevt_libuna.h \
	libevt_notify.c libevt_notify.h \
	libevt_record.c libevt_record.h \
	libevt_support.c libevt_support.h \
	libevt_types.h \
	libevt_unused.h \
	evt_file_header.h \
	evt_record.h

libevt_la_LIBADD = \
	../libcstring/libcstring.la \
	../liberror/liberror.la \
	../libnotify/libnotify.la \
	@LIBUNA_LIBADD@ \
	@LIBBFIO_LIBADD@ \
	@LIBFDATETIME_LIBADD@ \
	@LIBFGUID_LIBADD@ \
	@LIBFVALUE_LIBADD@ \
	@LIBFWNT_LIBADD@

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

