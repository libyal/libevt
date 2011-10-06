AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	-I$(top_srcdir)/libcstring \
	-I$(top_srcdir)/liberror \
	-I$(top_srcdir)/libsystem \
	@LIBUNA_CPPFLAGS@ \
	@LIBBFIO_CPPFLAGS@ \
	@LIBFDATETIME_CPPFLAGS@ \
	@LIBFGUID_CPPFLAGS@ \
	@LIBFVALUE_CPPFLAGS@ \
	@LIBFWNT_CPPFLAGS@ \
	@MEMWATCH_CPPFLAGS@ \
	@LIBEVT_DLL_IMPORT@

AM_LDFLAGS = @STATIC_LDFLAGS@

bin_PROGRAMS = \
	evtexport \
	evtinfo

evtexport_SOURCES = \
	export_handle.c export_handle.h \
	log_handle.c log_handle.h \
	evtexport.c \
	evtinput.c evtinput.h \
	evtoutput.c evtoutput.h \
	evttools_libevt.h

evtexport_LDADD = \
	../libcstring/libcstring.la \
	../liberror/liberror.la \
	../libevt/libevt.la \
	../libsystem/libsystem.la \
	@LIBUNA_LIBADD@ \
	@LIBINTL@ \
	@MEMWATCH_LIBADD@

evtinfo_SOURCES = \
	info_handle.c info_handle.h \
	evtinfo.c \
	evtinput.c evtinput.h \
	evtoutput.c evtoutput.h \
	evttools_libfdatetime.h \
	evttools_libevt.h

evtinfo_LDADD = \
	../libcstring/libcstring.la \
	../liberror/liberror.la \
	../libevt/libevt.la \
	../libsystem/libsystem.la \
	@LIBUNA_LIBADD@ \
	@LIBFDATETIME_LIBADD@ \
	@LIBINTL@ \
	@MEMWATCH_LIBADD@

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

splint:
	@echo "Running splint on evtexport ..." 
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(evtexport_SOURCES) 
	@echo "Running splint on evtinfo ..." 
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(evtinfo_SOURCES) 

