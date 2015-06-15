#AC_CEDAR_SHERPA()
#----------------------------------------
AC_DEFUN([AC_CEDAR_SHERPA], [
  AC_PATH_PROG(SHERPACONFIG, [Sherpa-config], [], [$with_sherpa/bin:$PATH])
  if test x$SHERPACONFIG != x; then
    SHERPAPATH=`$SHERPACONFIG --prefix`
    SHERPAINCPATH=`$SHERPACONFIG --incdir`
    SHERPALIBS=`$SHERPACONFIG --libs`
    SHERPALDFLAGS=`$SHERPACONFIG --ldflags`
    if test -z "`echo $SHERPALDFLAGS | grep '-L' 2> /dev/null`"; then
      SHERPALDFLAGS="$SHERPALDFLAGS -L$SHERPAPATH/lib/SHERPA-MC"
    fi
    SHERPACPPFLAGS="-I$SHERPAINCPATH"
    AC_SUBST(SHERPAPATH)
    AC_SUBST(SHERPAINCPATH)
    AC_SUBST(SHERPALIBS)
    AC_SUBST(SHERPALDFLAGS)
    AC_SUBST(SHERPACPPFLAGS)
    $1
  else
    AC_MSG_WARN([Could not find Sherpa-config script])
    $2
  fi
])
