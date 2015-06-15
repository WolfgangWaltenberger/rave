#AC_CEDAR_CHECK_THEPEG()
#--------------------------------------
AC_DEFUN([AC_CEDAR_CHECK_THEPEG], [
  #m4_define([cedar_prettyname], [$1])dnl
  #m4_define([cedar_PKGNAME], [translit([translit([$1], [a-z], [A-Z])], [.])])dnl
  #m4_define([cedar_pkgname], [translit([translit([$1], [A-Z], [a-z])], [.])])dnl
  #m4_define([cedar_libname], [lib@&t@cedar_pkgname@&t@.a])dnl
  #m4_define([cedar_incname], [cedar_pkgname])dnl

  AC_MSG_RESULT([Checking for ThePEG:])
  # Don't know why this isn't working by default:
  test x${prefix} = xNONE && prefix=${ac_default_prefix}
  AC_ARG_WITH(thepeg,
              AC_HELP_STRING(--with-thepeg, path to ThePEG),
              [pkgpath=$with_thepeg], 
              [pkgpath=${prefix}])
  AC_ARG_WITH(thepeg-lib,
              AC_HELP_STRING(--with-thepeg-lib, path to ThePEG library),
              [pkglibpath=$with_thepeg_lib], 
              [pkglibpath=${prefix}/lib])
  AC_ARG_WITH(thepeg-include,
              AC_HELP_STRING(--with-thepeg-include, path to ThePEG headers),
              [pkgincpath=$with_thepeg_include], 
              [pkgincpath=${prefix}/include])
  pkglib=yes; pkginc=yes; pkggood=yes

  if test x$pkgpath != xno; then
    AC_CHECK_FILE([${pkgpath}/lib/ThePEG/libThePEG.so], [],
                  [pkglib=no; AC_MSG_ERROR([ThePEG library "libThePEG.so" is not in a standard location])])
    AC_MSG_NOTICE([Found ThePEG libraries])
    AC_CHECK_FILE([${pkgpath}/include/ThePEG], [],
                  [pkginc=no; AC_MSG_ERROR([cedar_prettyname header directory is not in a standard location])])
    AC_MSG_NOTICE([Found ThePEG header directory])
  else
    AC_MSG_ERROR([You've specified "--without-thepeg", but ThePEG is required to build ${PACKAGE_NAME}])
  fi
  AC_MSG_RESULT([ThePEG paths verified])
  THEPEGPATH=${pkgpath}
  AC_SUBST(THEPEGPATH)
])
