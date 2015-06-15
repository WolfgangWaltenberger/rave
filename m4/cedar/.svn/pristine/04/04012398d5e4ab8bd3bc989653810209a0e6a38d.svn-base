#AC_SEARCH_CLHEP(actionIfFound, actionIfNotFound)
AC_DEFUN([AC_SEARCH_CLHEP],[
find_manually=no
if test x$with_clhep = x; then
  AC_CHECK_PROG(CLHEPCONFIG, [clhep-config], [clhep-config], [no])
  if test x$CLHEPCONFIG != xno; then
    AC_MSG_NOTICE([Using clhep-config to find CLHEP paths])
    libstr=`$CLHEPCONFIG --libs`
    incstr=`$CLHEPCONFIG --include`
    CLHEPLIB="$libstr"
    CLHEPLIBPATH=`echo $libstr | sed -e s/'^-L\(@<:@^\ @:>@*\).*$'/'\1'/`
    CLHEPLIBNAME=`echo $libstr | sed -e s/'.*-l\(@<:@^\ @:>@*\).*$'/'\1'/`
    CLHEPINCPATH=`echo $incstr | sed -e s/'^-I\(@<:@^\ @:>@*\).*$'/'\1'/`
    CLHEPINCNAME="CLHEP"
    AC_SUBST([CLHEPLIB])
    AC_SUBST([CLHEPLIBPATH])
    AC_SUBST([CLHEPLIBNAME])
    AC_SUBST([CLHEPINCPATH])
    AC_SUBST([CLHEPINCNAME])
    AM_CONDITIONAL([WITH_CLHEP], true)
    AM_CONDITIONAL([WITH_CLHEPLIB], true)
    AM_CONDITIONAL([WITH_CLHEPINC], true)
    AM_CONDITIONAL([WITHOUT_CLHEP], false)
    AM_CONDITIONAL([WITHOUT_CLHEPLIB], false)
    AM_CONDITIONAL([WITHOUT_CLHEPINC], false)
    AC_MSG_NOTICE([CLHEP libs path is $CLHEPLIBPATH])
    AC_MSG_NOTICE([CLHEP include path is $CLHEPINCPATH])
    $1
  else
    find_manually=yes
  fi
elif test x$with_clhep = xno; then
  AC_MSG_NOTICE([Not using CLHEP])
  AM_CONDITIONAL([WITH_CLHEP], false)
  AM_CONDITIONAL([WITH_CLHEPLIB], false)
  AM_CONDITIONAL([WITH_CLHEPINC], false)
  AM_CONDITIONAL([WITHOUT_CLHEP], true)
  AM_CONDITIONAL([WITHOUT_CLHEPLIB], true)
  AM_CONDITIONAL([WITHOUT_CLHEPINC], true)
else
  find_manually=yes
fi

if test x$find_manually != xno; then
    AC_MSG_NOTICE([Trying to find CLHEP manually])
    AC_CEDAR_LIBRARYANDHEADERS([CLHEP], [?.?.?.?], [$1], [$2])
fi
])


# Check what version of CLHEP we've got
#AC_TEST_CLHEP_VERSION(actionIfVersion1, actionIfVersion2)
AC_DEFUN([AC_TEST_CLHEP_VERSION],[
AC_CHECK_PROG(CLHEPCONFIG, [clhep-config], [clhep-config], [no])
if test x$CLHEPCONFIG != xno; then
  AC_MSG_NOTICE([Using clhep-config to find CLHEP version])
  verstr=`$CLHEPCONFIG --version`
  CLHEPVERSION=`echo $verstr | sed -e s/'^CLHEP\ *\(@<:@0-9\.@:>@*\)$'/'\1'/`

  if test $( echo $CLHEPVERSION | egrep "^1\..*" ); then
    AC_MSG_NOTICE([CLHEP version < 2.0 detected.])
    $1
  else
    AC_MSG_NOTICE([CLHEP version >= 2.0 detected.])
    $2
  fi

  AM_CONDITIONAL([CLHEP1], test $( echo $CLHEPVERSION | egrep "^1\..*") )
  AM_CONDITIONAL([CLHEP2], test $( echo $CLHEPVERSION | egrep "^2\..*") )
else
  AC_CHECK_FILE([$CLHEPINCPATH]/config/CLHEP.h, [clhep_config=yes], [clhep_config=no])
  if test x$clhep_config = xyes; then
    AC_MSG_NOTICE([CLHEP version < 2.0 detected.])
    $1
  else
    AC_MSG_NOTICE([CLHEP version >= 2.0 detected.])
    $2
  fi
  AM_CONDITIONAL(CLHEP1, test "x$clhep_config" = "xyes")
  AM_CONDITIONAL(CLHEP2, test "x$clhep_config" = "xno")
fi
])


# Check whether we can link against CLHEP
#AC_TEST_CLHEP(actionIfWorks, actionIfFails)
AC_DEFUN([AC_TEST_CLHEP],[
## Now lets see if the libraries work properly
oldLIB="$LIBS"
oldLDFLAGS="$LDFLAGS"
oldCPPFLAGS="$CPPFLAGS"
LIBS="$LIBS $CLHEPLIB"
LDFLAGS="$LDFLAGS -L$CLHEPPATH/lib -L$CLHEPLIBPATH -l$CLHEPLIBNAME"
CPPFLAGS="$CPPFLAGS -I$CLHEPINCPATH -I$CLHEPINCPATH/$CLHEPINCNAME $CLHEPINCLUDE"
#echo libs: $LIBS
#echo ldflags: $LDFLAGS
#echo cppflags: $CPPFLAGS

## Check CLHEP
AC_MSG_CHECKING([that CLHEP works])
AC_LINK_IFELSE([AC_LANG_PROGRAM([[#include <CLHEP/Random/Random.h>
namespace CLHEP {}]], [[using namespace CLHEP; HepRandom r; r.flat();]])],
[AC_MSG_RESULT(yes)
$1],
[AC_MSG_RESULT(no) 
AC_MSG_NOTICE([CLHEP does not work])
$2])
LIBS="$oldLIB"
LDFLAGS="$oldLDFLAGS"
CPPFLAGS="$oldCPPFLAGS"
])
