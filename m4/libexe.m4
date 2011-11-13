dnl Functions for libexe
dnl
dnl Version: 20111025

dnl Function to detect if libexe is available
dnl ac_libexe_dummy is used to prevent AC_CHECK_LIB adding unnecessary -l<library> arguments
AC_DEFUN([AX_LIBEXE_CHECK_LIB],
 [dnl Check if parameters were provided
 AS_IF(
  [test "x$ac_cv_with_libexe" != x && test "x$ac_cv_with_libexe" != xno && test "x$ac_cv_with_libexe" != xauto-detect],
  [AS_IF(
   [test -d "$ac_cv_with_libexe"],
   [CFLAGS="$CFLAGS -I${ac_cv_with_libexe}/include"
   LDFLAGS="$LDFLAGS -L${ac_cv_with_libexe}/lib"],
   [AC_MSG_WARN([no such directory: $ac_cv_with_libexe])
   ])
  ])

 AS_IF(
  [test "x$ac_cv_with_libexe" = xno],
  [ac_cv_libexe=no],
  [dnl Check for headers
  AC_CHECK_HEADERS([libexe.h])
 
  AS_IF(
   [test "x$ac_cv_header_libexe_h" = xno],
   [ac_cv_libexe=no],
   [ac_cv_libexe=yes
   AC_CHECK_LIB(
    exe,
    libexe_get_version,
    [ac_cv_libexe_dummy=yes],
    [ac_cv_libexe=no])
  
   dnl TODO add functions
   ])
  ])

 AS_IF(
  [test "x$ac_cv_libexe" = xyes],
  [AC_DEFINE(
   [HAVE_LIBEXE],
   [1],
   [Define to 1 if you have the `exe' library (-lexe).])
  LIBS="-lexe $LIBS"
  ])

 AS_IF(
  [test "x$ac_cv_libexe" = xyes],
  [AC_SUBST(
   [HAVE_LIBEXE],
   [1]) ],
  [AC_SUBST(
   [HAVE_LIBEXE],
   [0])
  ])
 ])

dnl Function to detect if libexe dependencies are available
AC_DEFUN([AX_LIBEXE_CHECK_LOCAL],
 [dnl Types used in libexe/libexe_date_time.h
 AC_STRUCT_TM

 dnl Headers included in libexe/libexe_date_time.h
 AC_HEADER_TIME

 dnl Date and time functions used in libexe/libexe_date_time.h
 AC_CHECK_FUNCS(
  [time],
  [],
  [AC_MSG_FAILURE(
   [Missing function: time],
   [1])
  ])
 ])

dnl Function to detect how to enable libexe
AC_DEFUN([AX_LIBEXE_CHECK_ENABLE],
 [AX_COMMON_ARG_WITH(
  [libexe],
  [libexe],
  [search for libexe in includedir and libdir or in the specified DIR, or no if to use local version],
  [auto-detect],
  [DIR])

 AX_LIBEXE_CHECK_LIB

 AS_IF(
  [test "x$ac_cv_libexe" != xyes],
  [AX_LIBEXE_CHECK_LOCAL

  AC_DEFINE(
   [HAVE_LOCAL_LIBEXE],
   [1],
   [Define to 1 if the local version of libexe is used.])
  AC_SUBST(
   [HAVE_LOCAL_LIBEXE],
   [1])
  AC_SUBST(
   [LIBEXE_CPPFLAGS],
   [-I../libexe])
  AC_SUBST(
   [LIBEXE_LIBADD],
   [../libexe/libexe.la])
  ac_cv_libexe=local
  ])

 AM_CONDITIONAL(
  [HAVE_LOCAL_LIBEXE],
  [test "x$ac_cv_libexe" = xlocal])

 AS_IF(
  [test "x$ac_cv_libexe" = xyes],
  [AC_SUBST(
   [ax_libexe_pc_libs_private],
   [-lexe])
  ])

 AS_IF(
  [test "x$ac_cv_libexe" = xyes],
  [AC_SUBST(
   [ax_libexe_spec_requires],
   [libexe])
  AC_SUBST(
   [ax_libexe_spec_build_requires],
   [libexe-devel])
  ])
 ])
