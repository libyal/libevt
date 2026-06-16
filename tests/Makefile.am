AUTOMAKE_OPTIONS = subdir-objects

AM_CPPFLAGS = \
	-I../include -I$(top_srcdir)/include \
	-I../common -I$(top_srcdir)/common \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCTHREADS_CPPFLAGS@ \
	@LIBCDATA_CPPFLAGS@ \
	@LIBCLOCALE_CPPFLAGS@ \
	@LIBCNOTIFY_CPPFLAGS@ \
	@LIBCSPLIT_CPPFLAGS@ \
	@LIBUNA_CPPFLAGS@ \
	@LIBCDIRECTORY_CPPFLAGS@ \
	@LIBCFILE_CPPFLAGS@ \
	@LIBCPATH_CPPFLAGS@ \
	@LIBBFIO_CPPFLAGS@ \
	@LIBFCACHE_CPPFLAGS@ \
	@LIBFDATA_CPPFLAGS@ \
	@LIBFDATETIME_CPPFLAGS@ \
	@LIBFGUID_CPPFLAGS@ \
	@LIBFVALUE_CPPFLAGS@ \
	@LIBFWEVT_CPPFLAGS@ \
	@LIBFWNT_CPPFLAGS@ \
	@LIBEXE_CPPFLAGS@ \
	@LIBREGF_CPPFLAGS@ \
	@LIBWRC_CPPFLAGS@ \
	@PTHREAD_CPPFLAGS@ \
	@LIBEVT_DLL_IMPORT@

check_PYTHONdir = $(abs_top_builddir)

BUILT_SOURCES = package.m4

check_AUTOTESTS = \
	test_library \
	test_manpages \
	test_tools

if HAVE_PYTHON_TESTS
check_AUTOTESTS += test_python_module
endif

check_PYTHON_SCRIPTS = \
	pyevt_test_file.py \
	pyevt_test_support.py

check_PROGRAMS = \
	evt_test_end_of_file_record \
	evt_test_error \
	evt_test_event_record \
	evt_test_file \
	evt_test_file_header \
	evt_test_io_handle \
	evt_test_notify \
	evt_test_record \
	evt_test_record_values \
	evt_test_strings_array \
	evt_test_support \
        evt_test_tools_info_handle \
        evt_test_tools_message_handle \
        evt_test_tools_message_string \
        evt_test_tools_output \
        evt_test_tools_path_handle \
        evt_test_tools_registry_file \
        evt_test_tools_resource_file \
	evt_test_tools_signal

evt_test_end_of_file_record_SOURCES = \
	evt_test_end_of_file_record.c \
	evt_test_libcerror.h \
	evt_test_libevt.h \
	evt_test_macros.h \
	evt_test_memory.c evt_test_memory.h \
	evt_test_unused.h

evt_test_end_of_file_record_LDADD = \
	../libevt/libevt.la \
	@LIBCERROR_LIBADD@

evt_test_error_SOURCES = \
	evt_test_error.c \
	evt_test_libevt.h \
	evt_test_macros.h \
	evt_test_unused.h

evt_test_error_LDADD = \
	../libevt/libevt.la

evt_test_event_record_SOURCES = \
	evt_test_event_record.c \
	evt_test_libcerror.h \
	evt_test_libevt.h \
	evt_test_macros.h \
	evt_test_memory.c evt_test_memory.h \
	evt_test_unused.h

evt_test_event_record_LDADD = \
	../libevt/libevt.la \
	@LIBCERROR_LIBADD@

evt_test_file_SOURCES = \
	evt_test_file.c \
	evt_test_functions.c evt_test_functions.h \
	evt_test_getopt.c evt_test_getopt.h \
	evt_test_libbfio.h \
	evt_test_libcerror.h \
	evt_test_libclocale.h \
	evt_test_libcnotify.h \
	evt_test_libevt.h \
	evt_test_libuna.h \
	evt_test_macros.h \
	evt_test_memory.c evt_test_memory.h \
	evt_test_unused.h

evt_test_file_LDADD = \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libevt/libevt.la \
	@LIBCTHREADS_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@PTHREAD_LIBADD@

evt_test_file_header_SOURCES = \
	evt_test_file_header.c \
	evt_test_functions.c evt_test_functions.h \
	evt_test_libbfio.h \
	evt_test_libcerror.h \
	evt_test_libevt.h \
	evt_test_macros.h \
	evt_test_memory.c evt_test_memory.h \
	evt_test_unused.h

evt_test_file_header_LDADD = \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libevt/libevt.la \
	@LIBCERROR_LIBADD@

evt_test_io_handle_SOURCES = \
	evt_test_io_handle.c \
	evt_test_libcerror.h \
	evt_test_libevt.h \
	evt_test_macros.h \
	evt_test_memory.c evt_test_memory.h \
	evt_test_unused.h

evt_test_io_handle_LDADD = \
	../libevt/libevt.la \
	@LIBCERROR_LIBADD@

evt_test_notify_SOURCES = \
	evt_test_libcerror.h \
	evt_test_libevt.h \
	evt_test_macros.h \
	evt_test_notify.c \
	evt_test_unused.h

evt_test_notify_LDADD = \
	../libevt/libevt.la \
	@LIBCERROR_LIBADD@

evt_test_record_SOURCES = \
	evt_test_libcerror.h \
	evt_test_libevt.h \
	evt_test_macros.h \
	evt_test_memory.c evt_test_memory.h \
	evt_test_record.c \
	evt_test_unused.h

evt_test_record_LDADD = \
	../libevt/libevt.la \
	@LIBCERROR_LIBADD@

evt_test_record_values_SOURCES = \
	evt_test_libcerror.h \
	evt_test_libevt.h \
	evt_test_macros.h \
	evt_test_memory.c evt_test_memory.h \
	evt_test_record_values.c \
	evt_test_unused.h

evt_test_record_values_LDADD = \
	../libevt/libevt.la \
	@LIBCERROR_LIBADD@

evt_test_strings_array_SOURCES = \
	evt_test_libcerror.h \
	evt_test_libevt.h \
	evt_test_macros.h \
	evt_test_memory.c evt_test_memory.h \
	evt_test_strings_array.c \
	evt_test_unused.h

evt_test_strings_array_LDADD = \
	../libevt/libevt.la \
	@LIBCERROR_LIBADD@

evt_test_support_SOURCES = \
	evt_test_functions.c evt_test_functions.h \
	evt_test_getopt.c evt_test_getopt.h \
	evt_test_libbfio.h \
	evt_test_libcerror.h \
	evt_test_libclocale.h \
	evt_test_libcnotify.h \
	evt_test_libevt.h \
	evt_test_libuna.h \
	evt_test_macros.h \
	evt_test_memory.c evt_test_memory.h \
	evt_test_support.c \
	evt_test_unused.h

evt_test_support_LDADD = \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libevt/libevt.la \
	@LIBCERROR_LIBADD@

evt_test_tools_info_handle_SOURCES = \
	../evttools/evtinput.c ../evttools/evtinput.h \
	../evttools/info_handle.c ../evttools/info_handle.h \
	evt_test_libcerror.h \
	evt_test_macros.h \
	evt_test_memory.c evt_test_memory.h \
	evt_test_tools_info_handle.c \
	evt_test_unused.h

evt_test_tools_info_handle_LDADD = \
	@LIBFDATETIME_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	../libevt/libevt.la \
	@LIBCERROR_LIBADD@

evt_test_tools_message_handle_SOURCES = \
	../evttools/message_handle.c ../evttools/message_handle.h \
	../evttools/message_string.c ../evttools/message_string.h \
	../evttools/resource_file.c ../evttools/resource_file.h \
	../evttools/path_handle.c ../evttools/path_handle.h \
	../evttools/registry_file.c ../evttools/registry_file.h \
	evt_test_libcerror.h \
	evt_test_macros.h \
	evt_test_memory.c evt_test_memory.h \
	evt_test_tools_message_handle.c \
	evt_test_unused.h

evt_test_tools_message_handle_LDADD = \
	@LIBWRC_LIBADD@ \
	@LIBREGF_LIBADD@ \
	@LIBEXE_LIBADD@ \
	@LIBFWEVT_LIBADD@ \
	@LIBFVALUE_LIBADD@ \
	@LIBFWNT_LIBADD@ \
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
	@LIBCERROR_LIBADD@

evt_test_tools_message_string_SOURCES = \
	../evttools/message_string.c ../evttools/message_string.h \
	evt_test_libcerror.h \
	evt_test_macros.h \
	evt_test_memory.c evt_test_memory.h \
	evt_test_tools_message_string.c \
	evt_test_unused.h

evt_test_tools_message_string_LDADD = \
	@LIBWRC_LIBADD@ \
	@LIBEXE_LIBADD@ \
	@LIBFWEVT_LIBADD@ \
	@LIBFVALUE_LIBADD@ \
	@LIBFWNT_LIBADD@ \
	@LIBFGUID_LIBADD@ \
	@LIBFDATETIME_LIBADD@ \
	@LIBFDATA_LIBADD@ \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libevt/libevt.la \
	@LIBCERROR_LIBADD@

evt_test_tools_output_SOURCES = \
	../evttools/evttools_output.c ../evttools/evttools_output.h \
	evt_test_libcerror.h \
	evt_test_macros.h \
	evt_test_tools_output.c \
	evt_test_unused.h

evt_test_tools_output_LDADD = \
	../libevt/libevt.la \
	@LIBCERROR_LIBADD@ \
	@LIBINTL@

evt_test_tools_path_handle_SOURCES = \
	../evttools/path_handle.c ../evttools/path_handle.h \
	evt_test_libcerror.h \
	evt_test_macros.h \
	evt_test_memory.c evt_test_memory.h \
	evt_test_tools_path_handle.c \
	evt_test_unused.h

evt_test_tools_path_handle_LDADD = \
	@LIBCDIRECTORY_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libevt/libevt.la \
	@LIBCERROR_LIBADD@

evt_test_tools_registry_file_SOURCES = \
	../evttools/registry_file.c ../evttools/registry_file.h \
	evt_test_libcerror.h \
	evt_test_macros.h \
	evt_test_memory.c evt_test_memory.h \
	evt_test_tools_registry_file.c \
	evt_test_unused.h

evt_test_tools_registry_file_LDADD = \
	@LIBREGF_LIBADD@ \
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
	@LIBCERROR_LIBADD@

evt_test_tools_resource_file_SOURCES = \
	../evttools/message_string.c ../evttools/message_string.h \
	../evttools/resource_file.c ../evttools/resource_file.h \
	evt_test_libcerror.h \
	evt_test_macros.h \
	evt_test_memory.c evt_test_memory.h \
	evt_test_tools_resource_file.c \
	evt_test_unused.h

evt_test_tools_resource_file_LDADD = \
	@LIBWRC_LIBADD@ \
	@LIBEXE_LIBADD@ \
	@LIBFWEVT_LIBADD@ \
	@LIBFVALUE_LIBADD@ \
	@LIBFWNT_LIBADD@ \
	@LIBFGUID_LIBADD@ \
	@LIBFDATETIME_LIBADD@ \
	@LIBFDATA_LIBADD@ \
	@LIBFCACHE_LIBADD@ \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libevt/libevt.la \
	@LIBCERROR_LIBADD@

evt_test_tools_signal_SOURCES = \
	../evttools/evttools_signal.c ../evttools/evttools_signal.h \
	evt_test_libcerror.h \
	evt_test_macros.h \
	evt_test_tools_signal.c \
	evt_test_unused.h

evt_test_tools_signal_LDADD = \
	../libevt/libevt.la \
	@LIBCERROR_LIBADD@

AUTOM4TE = autom4te
AUTOTEST = $(AUTOM4TE) --language=autotest

SUFFIXES = .at

.at:
	$(AM_V_GEN)rm -f $@ $@.tmp; \
	$(AUTOTEST) -I '$(srcdir)' -o $@.tmp $< && \
	mv $@.tmp $@

atconfig: $(top_builddir)/config.status
	cd $(top_builddir) && $(SHELL) ./config.status tests/$@

check-build: $(check_PROGRAMS)

check-local: $(check_AUTOTESTS)
	@fail=0; \
	for test_suite in $(check_AUTOTESTS); do \
		$(AM_TESTS_ENVIRONMENT) $(SHELL) "./$$test_suite" $(TESTSUITEFLAGS) || fail=1; \
		echo ""; \
	done; \
	exit $$fail

clean-local:
	for test_suite in $(check_AUTOTESTS); do \
		test ! -f "$$test_suite" || $(SHELL) "./$$test_suite" --clean; \
	done

package.m4: $(top_srcdir)/configure.ac
	$(AM_V_GEN):; \
	{ \
	  echo 'm4_define([AT_PACKAGE_NAME], [$(PACKAGE_NAME)])' ; \
	  echo 'm4_define([AT_PACKAGE_TARNAME], [$(PACKAGE_TARNAME)])' ; \
	  echo 'm4_define([AT_PACKAGE_VERSION], [$(PACKAGE_VERSION)])' ; \
	  echo 'm4_define([AT_PACKAGE_STRING], [$(PACKAGE_STRING)])' ; \
	  echo 'm4_define([AT_PACKAGE_BUGREPORT], [$(PACKAGE_BUGREPORT)])' ; \
	} > $@

test_inputs_libevt.at: generate_test_inputs.sh
	$(SHELL) $(srcdir)/generate_test_inputs.sh $(PACKAGE_NAME) libevt

test_inputs_pyevt.at: generate_test_inputs.sh
	$(SHELL) $(srcdir)/generate_test_inputs.sh $(PACKAGE_NAME) pyevt

test_inputs_evtinfo.at: generate_test_inputs.sh
	$(SHELL) $(srcdir)/generate_test_inputs.sh $(PACKAGE_NAME) evtinfo

test_inputs_evtexport.at: generate_test_inputs.sh
	$(SHELL) $(srcdir)/generate_test_inputs.sh $(PACKAGE_NAME) evtexport

test_library: \
	package.m4 \
	test_inputs_libevt.at \
	test_library.at \
	test_macros.at

test_manpages: \
	package.m4 \
	test_manpages.at

test_python_module: \
	package.m4 \
	test_inputs_pyevt.at \
	test_python_module.at

test_tools: \
	package.m4 \
	test_inputs_evtexport.at \
	test_inputs_evtinfo.at \
	test_macros.at \
	test_tools.at

EXTRA_DIST = \
	$(check_AUTOTESTS:=.at) \
	$(check_PYTHON_SCRIPTS) \
	generate_test_inputs.sh \
	package.m4 \
	test_macros.at

CLEANFILES = \
	$(check_AUTOTESTS) \
	*.exe \
	*.tmp \
	notify_stream.log \
	test_inputs_*.at

DISTCLEANFILES = \
	*.log \
	atconfig \
	Makefile \
	Makefile.in


