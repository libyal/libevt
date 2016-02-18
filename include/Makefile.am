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
	/bin/rm -f libevt.h
	/bin/rm -f libevt/definitions.h
	/bin/rm -f libevt/features.h
	/bin/rm -f libevt/types.h
	/bin/rm -f Makefile

