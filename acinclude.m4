dnl Function to detect if libevt dependencies are available
AC_DEFUN([AX_LIBEVT_CHECK_LOCAL],
 [dnl Check for internationalization functions in libevt/libevt_i18n.c 
 AC_CHECK_FUNCS([bindtextdomain])
 ])

