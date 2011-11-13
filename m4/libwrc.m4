dnl Functions for libwrc
dnl
dnl Version: 20111113

dnl Function to detect if libwrc is available
dnl ac_libwrc_dummy is used to prevent AC_CHECK_LIB adding unnecessary -l<library> arguments
AC_DEFUN([AX_LIBWRC_CHECK_LIB],
 [dnl Check if parameters were provided
 AS_IF(
  [test "x$ac_cv_with_libwrc" != x && test "x$ac_cv_with_libwrc" != xno && test "x$ac_cv_with_libwrc" != xauto-detect],
  [AS_IF(
   [test -d "$ac_cv_with_libwrc"],
   [CFLAGS="$CFLAGS -I${ac_cv_with_libwrc}/include"
   LDFLAGS="$LDFLAGS -L${ac_cv_with_libwrc}/lib"],
   [AC_MSG_WARN([no such directory: $ac_cv_with_libwrc])
   ])
  ])

 AS_IF(
  [test "x$ac_cv_with_libwrc" = xno],
  [ac_cv_libwrc=no],
  [dnl Check for headers
  AC_CHECK_HEADERS([libwrc.h])
 
  AS_IF(
   [test "x$ac_cv_header_libwrc_h" = xno],
   [ac_cv_libwrc=no],
   [ac_cv_libwrc=yes
   AC_CHECK_LIB(
    wrc,
    libwrc_get_version,
    [ac_cv_libwrc_dummy=yes],
    [ac_cv_libwrc=no])
  
   dnl TODO add functions
   ])
  ])

 AS_IF(
  [test "x$ac_cv_libwrc" = xyes],
  [AC_DEFINE(
   [HAVE_LIBWRC],
   [1],
   [Define to 1 if you have the `wrc' library (-lwrc).])
  LIBS="-lwrc $LIBS"
  ])

 AS_IF(
  [test "x$ac_cv_libwrc" = xyes],
  [AC_SUBST(
   [HAVE_LIBWRC],
   [1]) ],
  [AC_SUBST(
   [HAVE_LIBWRC],
   [0])
  ])
 ])

dnl Function to detect if libwrc dependencies are available
AC_DEFUN([AX_LIBWRC_CHECK_LOCAL],
 [dnl Types used in libwrc/libwrc_date_time.h
 AC_STRUCT_TM

 dnl Headers included in libwrc/libwrc_date_time.h
 AC_HEADER_TIME

 dnl Date and time functions used in libwrc/libwrc_date_time.h
 AC_CHECK_FUNCS(
  [time],
  [],
  [AC_MSG_FAILURE(
   [Missing function: time],
   [1])
  ])
 ])

dnl Function to detect how to enable libwrc
AC_DEFUN([AX_LIBWRC_CHECK_ENABLE],
 [AX_COMMON_ARG_WITH(
  [libwrc],
  [libwrc],
  [search for libwrc in includedir and libdir or in the specified DIR, or no if to use local version],
  [auto-detect],
  [DIR])

 AX_LIBWRC_CHECK_LIB

 AS_IF(
  [test "x$ac_cv_libwrc" != xyes],
  [AX_LIBWRC_CHECK_LOCAL

  AC_DEFINE(
   [HAVE_LOCAL_LIBWRC],
   [1],
   [Define to 1 if the local version of libwrc is used.])
  AC_SUBST(
   [HAVE_LOCAL_LIBWRC],
   [1])
  AC_SUBST(
   [LIBWRC_CPPFLAGS],
   [-I../libwrc])
  AC_SUBST(
   [LIBWRC_LIBADD],
   [../libwrc/libwrc.la])
  ac_cv_libwrc=local
  ])

 AM_CONDITIONAL(
  [HAVE_LOCAL_LIBWRC],
  [test "x$ac_cv_libwrc" = xlocal])

 AS_IF(
  [test "x$ac_cv_libwrc" = xyes],
  [AC_SUBST(
   [ax_libwrc_pc_libs_private],
   [-lwrc])
  ])

 AS_IF(
  [test "x$ac_cv_libwrc" = xyes],
  [AC_SUBST(
   [ax_libwrc_spec_requires],
   [libwrc])
  AC_SUBST(
   [ax_libwrc_spec_build_requires],
   [libwrc-devel])
  ])
 ])
