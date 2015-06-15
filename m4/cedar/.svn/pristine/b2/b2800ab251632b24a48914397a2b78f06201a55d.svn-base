## Determine whether a compiler flag is accepted
#AC_CEDAR_CHECKFCFLAG(flag, action-if-true, action-if-false)
AC_DEFUN([AC_CEDAR_CHECKFCFLAG], [
  AC_LANG_PUSH(Fortran)
  AC_MSG_CHECKING([if the $FC compiler accepts the $1 flag])
  AC_LANG_CONFTEST([AC_LANG_PROGRAM([],[return])])
  flag_ok=no
  conftestsrc=`ls conftest.f*`
  #$FC $1 $conftestsrc >&5 2>/dev/null && flag_ok=yes
  stat_string=`$FC $1 conftest.f 2>&1 1>&5` ; test -z "$stat_string" && flag_ok=yes
  AC_MSG_RESULT([$flag_ok])
  if test x$flag_ok = xyes; then 
    true
    $2
  else
    true
    $3
  fi
  AC_LANG_POP(Fortran)
])
