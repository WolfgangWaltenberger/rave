#AC_CEDAR_CCPYTHIA()
#----------------------------------------
AC_DEFUN([AC_CEDAR_CCPYTHIA], [
  AC_CEDAR_LIBRARYANDHEADERS([pythia8],[],
    [$1
    found_ccpythia=yes],
    [$2
    found_ccpythia=no
    AC_MSG_WARN([Pythia8 not found: Pythia8 generator will not work])])
  # PYTHIA8DATA needs to be specified either as environment variable 
  # or as configure option if Pythia8 is being used
  AC_ARG_VAR(PYTHIA8DATA, [absolute path to the Pythia8 xmldoc directory])
  if test x$PYTHIA8DATA = x; then
    PYTHIA8DATA=$PYTHIA8PATH/xmldoc
    if test ! -e "$PYTHIA8DATA"; then
      PYTHIA8DATA=""
    fi
  fi
  if test x$found_ccpythia != xno; then
    if test x$PYTHIA8DATA != x; then
      AC_MSG_NOTICE([Pythia 8 data directory is $PYTHIA8DATA])
    else
      AC_MSG_ERROR([Pythia 8 data directory is not specified!])
    fi
  fi
])
