dnl Functions for libregf
dnl
dnl Version: 20111113

dnl Function to detect if libregf is available
dnl ac_libregf_dummy is used to prevent AC_CHECK_LIB adding unnecessary -l<library> arguments
AC_DEFUN([AX_LIBREGF_CHECK_LIB],
 [dnl Check if parameters were provided
 AS_IF(
  [test "x$ac_cv_with_libregf" != x && test "x$ac_cv_with_libregf" != xno && test "x$ac_cv_with_libregf" != xauto-detect],
  [AS_IF(
   [test -d "$ac_cv_with_libregf"],
   [CFLAGS="$CFLAGS -I${ac_cv_with_libregf}/include"
   LDFLAGS="$LDFLAGS -L${ac_cv_with_libregf}/lib"],
   [AC_MSG_WARN([no such directory: $ac_cv_with_libregf])
   ])
  ])

 AS_IF(
  [test "x$ac_cv_with_libregf" = xno],
  [ac_cv_libregf=no],
  [dnl Check for headers
  AC_CHECK_HEADERS([libregf.h])
 
  AS_IF(
   [test "x$ac_cv_header_libregf_h" = xno],
   [ac_cv_libregf=no],
   [ac_cv_libregf=yes
   AC_CHECK_LIB(
    regf,
    libregf_get_version,
    [ac_cv_libregf_dummy=yes],
    [ac_cv_libregf=no])
  
   dnl TODO add functions
   ])
  ])

 AS_IF(
  [test "x$ac_cv_libregf" = xyes],
  [AC_DEFINE(
   [HAVE_LIBREGF],
   [1],
   [Define to 1 if you have the `regf' library (-lregf).])
  LIBS="-lregf $LIBS"
  ])

 AS_IF(
  [test "x$ac_cv_libregf" = xyes],
  [AC_SUBST(
   [HAVE_LIBREGF],
   [1]) ],
  [AC_SUBST(
   [HAVE_LIBREGF],
   [0])
  ])
 ])

dnl Function to detect if libregf dependencies are available
AC_DEFUN([AX_LIBREGF_CHECK_LOCAL],
 [dnl Types used in libregf/libregf_date_time.h
 AC_STRUCT_TM

 dnl Headers included in libregf/libregf_date_time.h
 AC_HEADER_TIME

 dnl Date and time functions used in libregf/libregf_date_time.h
 AC_CHECK_FUNCS(
  [time],
  [],
  [AC_MSG_FAILURE(
   [Missing function: time],
   [1])
  ])
 ])

dnl Function to detect how to enable libregf
AC_DEFUN([AX_LIBREGF_CHECK_ENABLE],
 [AX_COMMON_ARG_WITH(
  [libregf],
  [libregf],
  [search for libregf in includedir and libdir or in the specified DIR, or no if to use local version],
  [auto-detect],
  [DIR])

 AX_LIBREGF_CHECK_LIB

 AS_IF(
  [test "x$ac_cv_libregf" != xyes],
  [AX_LIBREGF_CHECK_LOCAL

  AC_DEFINE(
   [HAVE_LOCAL_LIBREGF],
   [1],
   [Define to 1 if the local version of libregf is used.])
  AC_SUBST(
   [HAVE_LOCAL_LIBREGF],
   [1])
  AC_SUBST(
   [LIBREGF_CPPFLAGS],
   [-I../libregf])
  AC_SUBST(
   [LIBREGF_LIBADD],
   [../libregf/libregf.la])
  ac_cv_libregf=local
  ])

 AM_CONDITIONAL(
  [HAVE_LOCAL_LIBREGF],
  [test "x$ac_cv_libregf" = xlocal])

 AS_IF(
  [test "x$ac_cv_libregf" = xyes],
  [AC_SUBST(
   [ax_libregf_pc_libs_private],
   [-lregf])
  ])

 AS_IF(
  [test "x$ac_cv_libregf" = xyes],
  [AC_SUBST(
   [ax_libregf_spec_requires],
   [libregf])
  AC_SUBST(
   [ax_libregf_spec_build_requires],
   [libregf-devel])
  ])
 ])
