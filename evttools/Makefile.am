AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	-I$(top_srcdir)/libcstring \
	-I$(top_srcdir)/liberror \
	-I$(top_srcdir)/libsystem \
	@LIBUNA_CPPFLAGS@ \
	@LIBBFIO_CPPFLAGS@ \
	@LIBFDATETIME_CPPFLAGS@ \
	@LIBFWNT_CPPFLAGS@ \
	@LIBFCACHE_CPPFLAGS@ \
	@LIBFDATA_CPPFLAGS@ \
	@LIBEXE_CPPFLAGS@ \
	@LIBREGF_CPPFLAGS@ \
	@LIBWRC_CPPFLAGS@ \
	@MEMWATCH_CPPFLAGS@ \
	@LIBEVT_DLL_IMPORT@

AM_LDFLAGS = @STATIC_LDFLAGS@

bin_PROGRAMS = \
	evtexport \
	evtinfo

evtexport_SOURCES = \
	export_handle.c export_handle.h \
	evtexport.c \
	evtinput.c evtinput.h \
	evtoutput.c evtoutput.h \
	evttools_codepage.c evttools_codepage.h \
	evttools_language.c evttools_language.h \
	evttools_libfcache.h \
	evttools_libfdatetime.h \
	evttools_libevt.h \
	evttools_libexe.h \
	evttools_libregf.h \
	evttools_libuna.h \
	evttools_libwrc.h \
	log_handle.c log_handle.h \
	message_file.c message_file.h

evtexport_LDADD = \
	../libcstring/libcstring.la \
	../liberror/liberror.la \
	../libevt/libevt.la \
	../libsystem/libsystem.la \
	@LIBUNA_LIBADD@ \
	@LIBFDATETIME_LIBADD@ \
	@LIBFWNT_LIBADD@ \
	@LIBEXE_LIBADD@ \
	@LIBREGF_LIBADD@ \
	@LIBWRC_LIBADD@ \
	@LIBINTL@ \
	@MEMWATCH_LIBADD@

evtinfo_SOURCES = \
	evtinfo.c \
	evtinput.c evtinput.h \
	evtoutput.c evtoutput.h \
	evttools_codepage.c evttools_codepage.h \
	evttools_libfdatetime.h \
	evttools_libevt.h \
	evttools_libuna.h \
	info_handle.c info_handle.h

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

