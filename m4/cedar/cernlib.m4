# Check whether cernlib is on the system (and get flags)
#AC_CHECK_CERNLIB([actionIfFound], [actionIfNotFound], [cernlibs="packlib mathlib kernlib"])
AC_DEFUN([AC_CHECK_CERNLIB],[
AC_ARG_VAR(CERNLIBPATH, path to CERNLIB root)
AC_ARG_VAR(CERNLIBLIBPATH, path to CERNLIB libraries)
AC_ARG_VAR(CERNLIBINCPATH, path to CERNLIB includes)
AC_ARG_VAR(CERNLIBBINPATH, path to CERNLIB binaries)
pkgpath=$CERNLIBPATH
pkglibpath=$CERNLIBLIBPATH
pkgincpath=$CERNLIBINCPATH
pkgbinpath=$CERNLIBBINPATH
AC_ARG_WITH([cernlib], AC_HELP_STRING(--with-cernlib, 
            path to CERNLIB), [pkgpath=$with_cernlib], [])
AC_ARG_WITH([cernlib-libpath], AC_HELP_STRING(--with-cernlib-libpath, 
            path to CERNLIB libraries), [pkglibpath=$with_cernlib_libpath], [])
AC_ARG_WITH([cernlib-incpath], AC_HELP_STRING(--with-cernlib-incpath, 
            path to CERNLIB includes), [pkgincpath=$with_cernlib_incpath], [])
AC_ARG_WITH([cernlib-binpath], AC_HELP_STRING(--with-cernlib-binpath, 
            path to CERNLIB binaries), [pkgbinpath=$with_cernlib_binpath], [])

CERNLIBFLAGS=""
CERNLIBLDFLAGS=""
CERNLIBLDADD=""
if test x$with_cernlib != xno; then
  CERNLIB=""
  ## If a bin path is specified and the cernlib script exists in it then use it
  if test "$pkgbinpath"; then CERNLIB="$pkgbinpath/cernlib"; fi
  if test ! -x "$CERNLIB"; then
    AC_CHECK_PROG(CERNLIB, [cernlib], [cernlib], [no])
  fi
  if test "$CERNLIB"; then
    cernlibs="$3"
    test x$cernlibs = x && cernlibs="packlib mathlib kernlib"
    AC_MSG_CHECKING([cernlib flags for "$cernlibs"])
    CERNLIBFLAGS=`$CERNLIB $cernlibs`
    ## Try to break down the flags into LDFLAGS and LDADD components
    for i in $CERNLIBFLAGS; do
      if test `echo $i | egrep -- "-L.*"`; then
        CERNLIBLDFLAGS="$CERNLIBLDFLAGS $i"
      else
        CERNLIBLDADD="$CERNLIBLDADD $i"
      fi
    done
    CERNLIBLDFLAGS=`echo "$CERNLIBLDFLAGS" | sed -e s/'^\ *'//`
    CERNLIBLDADD=`echo "$CERNLIBLDADD" | sed -e s/'^\ *'//`
    AC_MSG_RESULT(["$CERNLIBFLAGS"])
    $1
  else
    true
    $2
  fi
else
  AC_MSG_NOTICE([Not using CERNLIB])
fi

AM_CONDITIONAL([WITH_CERNLIB], test -n "$CERNLIBFLAGS")
AM_CONDITIONAL([WITHOUT_CERNLIB], test -z "$CERNLIBFLAGS")
CERNLIBPATH=$pkgpath
CERNLIBLIBPATH=$pkglibpath
CERNLIBINCPATH=$pkgincpath
AC_SUBST(CERNLIBFLAGS)
AC_SUBST(CERNLIBLDFLAGS)
AC_SUBST(CERNLIBLDADD)
AC_SUBST(CERNLIBPATH)
AC_SUBST(CERNLIBLIBPATH)
AC_SUBST(CERNLIBINCPATH)
])

# Check whether cernlib can actually be used
#AC_TEST_CERNLIB(actionIfWorks, actionIfFails)
AC_DEFUN([AC_TEST_CERNLIB],[
## Should add one of these
#AC_TRY_LINK()
])
