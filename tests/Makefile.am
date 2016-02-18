AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCSTRING_CPPFLAGS@ \
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
	@LIBEVT_DLL_IMPORT@

if HAVE_PYTHON_TESTS
TESTS_PYEVT = \
	test_pyevt_set_ascii_codepage.sh \
	test_pyevt_open_close.sh
endif

TESTS = \
	test_set_ascii_codepage.sh \
	test_open_close.sh \
	test_evtinfo.sh \
	test_evtexport.sh \
	$(TESTS_PYEVT)

check_SCRIPTS = \
	pyevt_test_open_close.py \
	pyevt_test_set_ascii_codepage.py \
	test_open_close.sh \
	test_pyevt_open_close.sh \
	test_pyevt_set_ascii_codepage.sh \
	test_evtexport.sh \
	test_evtinfo.sh \
	test_runner.sh \
	test_set_ascii_codepage.sh

EXTRA_DIST = \
	$(check_SCRIPTS)

check_PROGRAMS = \
	evt_test_open_close \
	evt_test_set_ascii_codepage

evt_test_open_close_SOURCES = \
	evt_test_libcerror.h \
	evt_test_libevt.h \
	evt_test_libcstring.h \
	evt_test_open_close.c

evt_test_open_close_LDADD = \
	../libevt/libevt.la \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBCSTRING_LIBADD@

evt_test_set_ascii_codepage_SOURCES = \
	evt_test_libcerror.h \
	evt_test_libclocale.h \
	evt_test_libevt.h \
	evt_test_libcstring.h \
	evt_test_set_ascii_codepage.c

evt_test_set_ascii_codepage_LDADD = \
	../libevt/libevt.la \
	@LIBCLOCALE_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBCSTRING_LIBADD@

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

