# AC_SEARCH_ROOT(actionIfFound, actionIfNotFound)
AC_DEFUN([AC_SEARCH_ROOT], [
ROOTSEARCHPATH=$PATH
if test -n "$ROOTSYS"; then
  AC_MSG_NOTICE([Adding ROOTSYS=$ROOTSYS to ROOT search path])
  ROOTSEARCHPATH=$ROOTSYS/bin:$PATH
fi
AC_PATH_PROG(ROOTCONFIG, [root-config], [no], [$ROOTSEARCHPATH])
if test x$ROOTCONFIG != xno; then
  AC_MSG_NOTICE([Using root-config to get ROOT paths])
  ROOTCPPFLAGS="`$ROOTCONFIG --cflags`"
  ROOTLIBS="`$ROOTCONFIG --libs`"
  ROOTLDFLAGS="`$ROOTCONFIG --ldflags`"
  AC_SUBST([ROOTCPPFLAGS])
  AC_SUBST([ROOTLIBS])
  AC_SUBST([ROOTLDFLAGS])
  AC_MSG_NOTICE([ROOTCPPFLAGS=$ROOTCPPFLAGS])
  AC_MSG_NOTICE([ROOTLIBS=$ROOTLIBS])
  AC_MSG_NOTICE([ROOTLDFLAGS=$ROOTLDFLAGS])
  $1
else
  $2
fi
])


# AC_TEST_ROOT(actionIfWorks, actionIfFails)
AC_DEFUN([AC_TEST_ROOT], [
oldLIBS="$LIBS"
oldLDFLAGS="$LDFLAGS"
oldCPPFLAGS="$CPPFLAGS"
oldCXXFLAGS="$CXXFLAGS"
LIBS="$LIBS $ROOTLIBS"
LDFLAGS="$LDFLAGS $ROOTLDFLAGS"
CPPFLAGS="$CPPFLAGS $ROOTCPPFLAGS"
CXXFLAGS="$CXXFLAGS -Wno-long-long"
## Check ROOT
AC_MSG_CHECKING([that ROOT works])
AC_TRY_LINK([#include "TROOT.h"
#include "TH1F.h"
#include "TRandom.h"],
[using namespace ROOT; 
TH1* a = new TH1F("a", "Caption", 100, -10, 10); gRandom->SetSeed();
for (unsigned int i = 0; i < 10; ++i) { a->Fill(gRandom->Gaus(0.0, 2.0)); }],
[AC_MSG_RESULT(yes)
$1],
[AC_MSG_RESULT(no) 
LIBS="$oldLIBS"
LDFLAGS="$oldLDFLAGS"
CPPFLAGS="$oldCPPFLAGS"
CXXFLAGS="$oldCXXFLAGS"
$2])
])


#AC_CEDAR_ROOTSYS()
AC_DEFUN([AC_CEDAR_ROOTSYS], [
  # ROOTSYS needs to be specified as environment variable 
  AC_ARG_VAR( ROOTSYS, [absolute path to Root installation directory])
  if test x$enable_root = xyes; then
    if test x$ROOTSYS != x; then
      AC_MSG_NOTICE([ROOTSYS=$ROOTSYS])
      if test -e $ROOTSYS/lib/libCore.so; then 
        AC_MSG_NOTICE([Found Root library libCore.so at $ROOTSYS/lib])
      else
        AC_MSG_ERROR([Root library libCore.so not fond at $ROOTSYS/lib]) 
      fi	
    else
      AC_MSG_ERROR([ROOTSYS path not specified!])
    fi
  fi
])


