AM_CPPFLAGS = \
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
	@LIBFVALUE_CPPFLAGS@ \
	@LIBFWEVT_CPPFLAGS@ \
	@LIBFWNT_CPPFLAGS@ \
	@LIBEXE_CPPFLAGS@ \
	@LIBREGF_CPPFLAGS@ \
	@LIBWRC_CPPFLAGS@ \
	@LIBCDIRECTORY_CPPFLAGS@ \
	@PTHREAD_CPPFLAGS@ \
	@LIBEVT_DLL_IMPORT@

AM_LDFLAGS = @STATIC_LDFLAGS@

bin_PROGRAMS = \
	evtexport \
	evtinfo

evtexport_SOURCES = \
	export_handle.c export_handle.h \
	evtexport.c \
	evtinput.c evtinput.h \
	evttools_getopt.c evttools_getopt.h \
	evttools_i18n.h \
	evttools_libbfio.h \
	evttools_libcdirectory.h \
	evttools_libcerror.h \
	evttools_libclocale.h \
	evttools_libcnotify.h \
	evttools_libcpath.h \
	evttools_libcsplit.h \
	evttools_libfcache.h \
	evttools_libfdatetime.h \
	evttools_libfwnt.h \
	evttools_libevt.h \
	evttools_libexe.h \
	evttools_libregf.h \
	evttools_libuna.h \
	evttools_libwrc.h \
	evttools_output.c evttools_output.h \
	evttools_signal.c evttools_signal.h \
	evttools_system_split_string.h \
	evttools_unused.h \
	evttools_wide_string.c evttools_wide_string.h \
	log_handle.c log_handle.h \
	message_handle.c message_handle.h \
	message_string.c message_string.h \
	path_handle.c path_handle.h \
	registry_file.c registry_file.h \
	resource_file.c resource_file.h

evtexport_LDADD = \
	@LIBREGF_LIBADD@ \
	@LIBWRC_LIBADD@ \
	@LIBEXE_LIBADD@ \
	@LIBFVALUE_LIBADD@ \
	@LIBFWNT_LIBADD@ \
	@LIBFWEVT_LIBADD@ \
	@LIBFGUID_LIBADD@ \
	@LIBFDATETIME_LIBADD@ \
	@LIBFDATA_LIBADD@ \
	@LIBFCACHE_LIBADD@ \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBCDIRECTORY_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libevt/libevt.la \
	@LIBCTHREADS_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBINTL@ \
	@PTHREAD_LIBADD@

evtinfo_SOURCES = \
	evtinfo.c \
	evtinput.c evtinput.h \
	evttools_getopt.c evttools_getopt.h \
	evttools_i18n.h \
	evttools_libcerror.h \
	evttools_libclocale.h \
	evttools_libcnotify.h \
	evttools_libfdatetime.h \
	evttools_libfwnt.h \
	evttools_libevt.h \
	evttools_libuna.h \
	evttools_output.c evttools_output.h \
	evttools_signal.c evttools_signal.h \
	evttools_unused.h \
	evttools_wide_string.c evttools_wide_string.h \
	info_handle.c info_handle.h

evtinfo_LDADD = \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	../libevt/libevt.la \
	@LIBCTHREADS_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBINTL@ \
	@PTHREAD_LIBADD@

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

splint:
	@echo "Running splint on evtexport ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(evtexport_SOURCES)
	@echo "Running splint on evtinfo ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(evtinfo_SOURCES)

