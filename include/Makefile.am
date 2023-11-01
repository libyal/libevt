include_HEADERS = \
	libevt.h

pkginclude_HEADERS = \
	libevt/codepage.h \
	libevt/definitions.h \
	libevt/error.h \
	libevt/extern.h \
	libevt/features.h \
	libevt/types.h

EXTRA_DIST = \
	libevt.h.in \
	libevt/definitions.h.in \
	libevt/features.h.in \
	libevt/types.h.in

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	-rm -f libevt.h
	-rm -f libevt/definitions.h
	-rm -f libevt/features.h
	-rm -f libevt/types.h
	-rm -f Makefile

