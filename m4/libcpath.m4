dnl Functions for libcpath
dnl
dnl Version: 20120401

dnl Function to detect if libcpath is available
dnl ac_libcpath_dummy is used to prevent AC_CHECK_LIB adding unnecessary -l<library> arguments
AC_DEFUN([AX_LIBCPATH_CHECK_LIB],
 [dnl Check if parameters were provided
 AS_IF(
  [test "x$ac_cv_with_libcpath" != x && test "x$ac_cv_with_libcpath" != xno && test "x$ac_cv_with_libcpath" != xauto-detect],
  [AS_IF(
   [test -d "$ac_cv_with_libcpath"],
   [CFLAGS="$CFLAGS -I${ac_cv_with_libcpath}/include"
   LDFLAGS="$LDFLAGS -L${ac_cv_with_libcpath}/lib"],
   [AC_MSG_WARN([no such directory: $ac_cv_with_libcpath])
   ])
  ])

 AS_IF(
  [test "x$ac_cv_with_libcpath" != xno],
  [dnl Check for headers
  AC_CHECK_HEADERS([libcpath.h])
 
  AS_IF(
   [test "x$ac_cv_header_libcpath_h" = xno],
   [ac_cv_libcpath=no],
   [ac_cv_libcpath=yes
   AC_CHECK_LIB(
    fdatetime,
    libcpath_get_version,
    [ac_cv_libcpath_dummy=yes],
    [ac_cv_libcpath=no])
  
   dnl TODO add functions
   ])
  ])

 AS_IF(
  [test "x$ac_cv_libcpath" = xyes],
  [AC_DEFINE(
   [HAVE_LIBCPATH],
   [1],
   [Define to 1 if you have the `cpath' library (-lcpath).])
  LIBS="-lcpath $LIBS"
  ])

 AS_IF(
  [test "x$ac_cv_libcpath" = xyes],
  [AC_SUBST(
   [HAVE_LIBCPATH],
   [1]) ],
  [AC_SUBST(
   [HAVE_LIBCPATH],
   [0])
  ])
 ])

dnl Function to detect if mkdir is available
dnl Also checks how to use mkdir
AC_DEFUN([AX_LIBCPATH_CHECK_FUNC_MKDIR],
 [AC_CHECK_FUNCS([mkdir])

 AS_IF(
  [test "x$ac_cv_func_mkdir" = xyes],
  [AC_MSG_CHECKING(
   [how to use mkdir])

  SAVE_CFLAGS="$CFLAGS"
  CFLAGS="$CFLAGS -Wall -Werror"
  AC_LANG_PUSH(C)

  AC_LINK_IFELSE(
   [AC_LANG_PROGRAM(
    [[#include <sys/stat.h>
#include <sys/types.h>]],
    [[mkdir( "", 0 )]] )],
    [AC_MSG_RESULT(
     [with additional mode argument])
    ac_cv_cv_mkdir_mode=yes],
    [ac_cv_cv_mkdir_mode=no])

  AS_IF(
   [test "x$ac_cv_cv_mkdir_mode" = xno],
   [AC_LINK_IFELSE(
    [AC_LANG_PROGRAM(
     [[#include <io.h>]],
     [[mkdir( "" )]] )],
    [AC_MSG_RESULT(
     [with single argument])
    ac_cv_cv_mkdir=yes],
    [ac_cv_cv_mkdir=no])
   ])

  AC_LANG_POP(C)
  CFLAGS="$SAVE_CFLAGS"

  AS_IF(
   [test "x$ac_cv_cv_mkdir_mode" = xno && test "x$ac_cv_cv_mkdir" = xno],
   [AC_MSG_WARN(
    [unknown])
   ac_cv_func_mkdir=no])

  AS_IF(
   [test "x$ac_cv_func_mkdir" = xyes],
   [AC_DEFINE(
    [HAVE_MKDIR],
    [1],
    [Define to 1 if you have the mkdir function.])
   ])

  AS_IF(
   [test "x$ac_cv_cv_mkdir_mode" = xyes],
   [AC_DEFINE(
    [HAVE_MKDIR_MODE],
    [1],
    [Define to 1 if you have the mkdir function with a second mode argument.])
   ])
  ])

 AS_IF(
  [test "x$ac_cv_func_mkdir" = xno],
  [AC_MSG_FAILURE(
   [Missing function: mkdir],
   [1])
  ])
 ])

dnl Function to detect if libcpath dependencies are available
AC_DEFUN([AX_LIBCPATH_CHECK_LOCAL],
 [dnl Headers included in libcpath/libcpath_path.h
 AC_CHECK_HEADERS([sys/stat.h])

 dnl Path functions used in libcpath/libcpath_path.h
 AC_CHECK_FUNCS([chdir getcwd])
 
 AS_IF(
  [test "x$ac_cv_func_chdir" != xyes],
  [AC_MSG_FAILURE(
   [Missing functions: chdir],
   [1])
  ])
 
 AS_IF(
  [test "x$ac_cv_func_getcwd" != xyes],
  [AC_MSG_FAILURE(
   [Missing functions: getcwd],
   [1])
  ])
 
 AX_LIBCPATH_CHECK_FUNC_MKDIR
 ])

dnl Function to detect how to enable libcpath
AC_DEFUN([AX_LIBCPATH_CHECK_ENABLE],
 [AX_COMMON_ARG_WITH(
  [libcpath],
  [libcpath],
  [search for libcpath in includedir and libdir or in the specified DIR, or no if to use local version],
  [auto-detect],
  [DIR])

 AX_LIBCPATH_CHECK_LIB

 AS_IF(
  [test "x$ac_cv_libcpath" != xyes],
  [AX_LIBCPATH_CHECK_LOCAL

  AC_DEFINE(
   [HAVE_LOCAL_LIBCPATH],
   [1],
   [Define to 1 if the local version of libcpath is used.])
  AC_SUBST(
   [HAVE_LOCAL_LIBCPATH],
   [1])
  AC_SUBST(
   [LIBCPATH_CPPFLAGS],
   [-I../libcpath])
  AC_SUBST(
   [LIBCPATH_LIBADD],
   [../libcpath/libcpath.la])

  ac_cv_libcpath=local
  ])

 AM_CONDITIONAL(
  [HAVE_LOCAL_LIBCPATH],
  [test "x$ac_cv_libcpath" = xlocal])

 AS_IF(
  [test "x$ac_cv_libcpath" = xyes],
  [AC_SUBST(
   [ax_libcpath_pc_libs_private],
   [-lstring])
  ])

 AS_IF(
  [test "x$ac_cv_libcpath" = xyes],
  [AC_SUBST(
   [ax_libcpath_spec_requires],
   [libcpath])
  AC_SUBST(
   [ax_libcpath_spec_build_requires],
   [libcpath-devel])
  ])
 ])

