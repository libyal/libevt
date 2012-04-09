ACLOCAL_AMFLAGS = -I m4

SUBDIRS = \
	include \
	common \
	libcstring \
	libcerror \
	libclocale \
	libcnotify \
	libuna \
	libcfile \
	libcpath \
	libbfio \
	libfdatetime \
	libfvalue \
	libfwnt \
	libevt \
	libfcache \
	libfdata \
	libexe \
	libregf \
	libwrc \
	libcdirectory \
	libcsystem \
	evttools \
	po \
	manuals \
	examples \
	tests \
	msvscpp

DPKG_FILES = \
	dpkg/changelog \
	dpkg/changelog.in \
       	dpkg/compat \
       	dpkg/control \
       	dpkg/copyright \
       	dpkg/copyright.in \
       	dpkg/rules \
       	dpkg/libevt-dev.docs \
       	dpkg/libevt.docs \
       	dpkg/libevt-tools.docs

GETTEXT_FILES = \
	config.rpath \
	po/Makevars.in

PKGCONFIG_FILES = \
	libevt.pc.in

SPEC_FILES = \
	libevt.spec \
	libevt.spec.in

EXTRA_DIST = \
	README.dll \
	README.macosx \
	README.mingw \
	README.static \
	$(DPKG_FILES) \
	$(GETTEXT_FILES) \
        $(PKGCONFIG_FILES) \
        $(SPEC_FILES)

MAINTAINERCLEANFILES = \
	Makefile.in

pkgconfigdir = $(libdir)/pkgconfig

pkgconfig_DATA = \
	libevt.pc

libtool: @LIBTOOL_DEPS@
	cd $(srcdir) && $(SHELL) ./config.status --recheck

lib: library

library:
	(cd $(srcdir)/common && $(MAKE) $(AM_MAKEFLAGS))
	(cd $(srcdir)/libcstring && $(MAKE) $(AM_MAKEFLAGS))
	(cd $(srcdir)/libcerror && $(MAKE) $(AM_MAKEFLAGS))
	(cd $(srcdir)/libclocale && $(MAKE) $(AM_MAKEFLAGS))
	(cd $(srcdir)/libcnotify && $(MAKE) $(AM_MAKEFLAGS))
	(cd $(srcdir)/libuna && $(MAKE) $(AM_MAKEFLAGS))
	(cd $(srcdir)/libcfile && $(MAKE) $(AM_MAKEFLAGS))
	(cd $(srcdir)/libcpath && $(MAKE) $(AM_MAKEFLAGS))
	(cd $(srcdir)/libbfio && $(MAKE) $(AM_MAKEFLAGS))
	(cd $(srcdir)/libfdatetime && $(MAKE) $(AM_MAKEFLAGS))
	(cd $(srcdir)/libfvalue && $(MAKE) $(AM_MAKEFLAGS))
	(cd $(srcdir)/libfwnt && $(MAKE) $(AM_MAKEFLAGS))
	(cd $(srcdir)/libevt && $(MAKE) $(AM_MAKEFLAGS))
	(cd $(srcdir)/po && $(MAKE) $(AM_MAKEFLAGS))

distclean: clean
	/bin/rm -f Makefile
	/bin/rm -f config.status
	/bin/rm -f config.cache
	/bin/rm -f config.log
	/bin/rm -f libevt.pc
	/bin/rm -f libevt.spec
	@for dir in ${subdirs}; do \
		(cd $$dir && $(MAKE) distclean) \
		|| case "$(MFLAGS)" in *k*) fail=yes;; *) exit 1;; esac; \
	done && test -z "$$fail"

splint:
	(cd $(srcdir)/libcstring && $(MAKE) splint $(AM_MAKEFLAGS))
	(cd $(srcdir)/libcerror && $(MAKE) splint $(AM_MAKEFLAGS))
	(cd $(srcdir)/libclocale && $(MAKE) splint $(AM_MAKEFLAGS))
	(cd $(srcdir)/libcnotify && $(MAKE) splint $(AM_MAKEFLAGS))
	(cd $(srcdir)/libuna && $(MAKE) splint $(AM_MAKEFLAGS))
	(cd $(srcdir)/libcfile && $(MAKE) splint $(AM_MAKEFLAGS))
	(cd $(srcdir)/libcpath && $(MAKE) splint $(AM_MAKEFLAGS))
	(cd $(srcdir)/libbfio && $(MAKE) splint $(AM_MAKEFLAGS))
	(cd $(srcdir)/libfdatetime && $(MAKE) splint $(AM_MAKEFLAGS))
	(cd $(srcdir)/libfvalue && $(MAKE) splint $(AM_MAKEFLAGS))
	(cd $(srcdir)/libfwnt && $(MAKE) splint $(AM_MAKEFLAGS))
	(cd $(srcdir)/libevt && $(MAKE) splint $(AM_MAKEFLAGS))
	(cd $(srcdir)/libfcache && $(MAKE) splint $(AM_MAKEFLAGS))
	(cd $(srcdir)/libfdata && $(MAKE) splint $(AM_MAKEFLAGS))
	(cd $(srcdir)/libexe && $(MAKE) splint $(AM_MAKEFLAGS))
	(cd $(srcdir)/libregf && $(MAKE) splint $(AM_MAKEFLAGS))
	(cd $(srcdir)/libwrc && $(MAKE) splint $(AM_MAKEFLAGS))
	(cd $(srcdir)/libcdirectory && $(MAKE) splint $(AM_MAKEFLAGS))
	(cd $(srcdir)/libcsystem && $(MAKE) splint $(AM_MAKEFLAGS))
	(cd $(srcdir)/evttools && $(MAKE) splint $(AM_MAKEFLAGS))
	(cd $(srcdir)/po && $(MAKE) splint $(AM_MAKEFLAGS))

