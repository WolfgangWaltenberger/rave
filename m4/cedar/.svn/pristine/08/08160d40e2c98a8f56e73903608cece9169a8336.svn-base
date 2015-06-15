#AC_CEDAR_CCHERWIG()
#----------------------------------------
AC_DEFUN([AC_CEDAR_CCHERWIG], [
  AC_ARG_VAR([THEPEGPATH], [path to ThePEG installation])
  AC_ARG_WITH([thepeg], AC_HELP_STRING([--with-thepeg], [path to ThePEG installation]))
  if test "x$with_thepeg" != "xno"; then THEPEGPATH="$with_thepeg"; fi
  if test "$THEPEGPATH"; then
    THEPEGCPPFLAGS="-I$THEPEGPATH/include"
    THEPEGLDFLAGS="-L$THEPEGPATH/lib/ThePEG"
    THEPEGLDLIBS="-lThePEG"
  fi
  AC_PATH_PROG(HERWIGCONFIG, [herwig-config], [], [$with_ccherwig/bin:$PATH])
  if test x$HERWIGCONFIG != x; then
    CCHERWIGPATH=`$HERWIGCONFIG --prefix`
    CCHERWIGINCPATH=`$HERWIGCONFIG --includedir`
    CCHERWIGLIBPATH=`$HERWIGCONFIG --libdir`
    CCHERWIGDATAPATH=`$HERWIGCONFIG --datadir`
    CCHERWIGLIBNAME=`$HERWIGCONFIG --libname`
    ## For some reason, Genser's H++ install doesn't report the ThePEG paths in herwig-config...
    CCHERWIGCPPFLAGS="`$HERWIGCONFIG --cppflags` $THEPEGCPPFLAGS"
    CCHERWIGLDFLAGS="`$HERWIGCONFIG --ldflags` $THEPEGLDFLAGS"
    CCHERWIGLDLIBS="`$HERWIGCONFIG --ldlibs` $THEPEGLDLIBS"
    AC_SUBST(CCHERWIGPATH)
    AC_SUBST(CCHERWIGINCPATH)
    AC_SUBST(CCHERWIGLIBPATH)
    AC_SUBST(CCHERWIGDATAPATH)
    AC_SUBST(CCHERWIGLIBNAME)
    AC_SUBST(CCHERWIGCPPFLAGS)
    AC_SUBST(CCHERWIGLDFLAGS)
    AC_SUBST(CCHERWIGLDLIBS)
    $1
  else
    AC_MSG_WARN([Could not find herwig-config script])
    $2
  fi
])
