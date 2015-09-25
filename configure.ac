#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(Rave, 0.6.25, walten@hephy.oeaw.ac.at)
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([-Werror tar-pax subdir-objects])
AM_PROG_AR
AC_CONFIG_SRCDIR([src/RaveBase/RaveInterface/src/VertexFactory.cc])
AC_CONFIG_HEADER([config.h])

# feature: flavortagging
AC_ARG_ENABLE([flavortagging],
  [AC_HELP_STRING([--disable-flavortagging],[disable flavor-tagging support (enabled by default)])],
  [enable_flavortagging=$enableval],
  [enable_flavortagging=yes]
)

AC_ARG_ENABLE([kinematics],
  [AC_HELP_STRING([--disable-kinematics],[disable kinematic fitting (enabled by default)])],
  [enable_kinematics=$enableval],
  [enable_kinematics=yes]
)

## feature: zvres
#AC_ARG_ENABLE([zvres],
#  [AC_HELP_STRING([--enable-zvres],[enable zvres algorithm (disabled by default)])],
#  [enable_zvres=$enableval],
#  [enable_zvres=no]
#)

# feature: zvres
AC_ARG_ENABLE([zvres],
  [AC_HELP_STRING([--disable-zvres],[disable zvres algorithm (enabled by default)])],
  [enable_zvres=$enableval],
  [enable_zvres=yes]
)

# feature: ttf (tertiary track finder)
AC_ARG_ENABLE([ttf],
  [AC_HELP_STRING([--enable-ttf],[enable ttf algorithm (disabled by default)])],
  [enable_ttf=$enableval],
  [enable_ttf=no]
)

# feature: mvf (multi vertex fitter
AC_ARG_ENABLE([mvf],
  [AC_HELP_STRING([--disable-mvf],[disable mvf algorithm (enabled by default)])],
  [enable_mvf=$enableval],
  [enable_mvf=yes]
)

# feature: gsf (gaussian sum fitter)
AC_ARG_ENABLE([gsf],
  [AC_HELP_STRING([--disable-gsf],[disable gsf algorithm (enabled by default)])],
  [enable_gsf=$enableval],
  [enable_gsf=yes]
)

# feature: tkvf (trimmed kalman)
AC_ARG_ENABLE([tkvf],
  [AC_HELP_STRING([--disable-tkvf],[disable tkvf algorithm (enabled by default)])],
  [enable_tkvf=$enableval],
  [enable_tkvf=yes]
)

# feature: developers (some extra classes which are usually not used)
AC_ARG_ENABLE([developers],
  [AC_HELP_STRING([--enable-developers],[enable developers classes (e.g. special linpt finders etc.) (disabled by default)])],
  [enable_developers=$enableval],
  [enable_developers=no]
)

# feature: unit tests
AC_ARG_ENABLE([unittests],
  [AC_HELP_STRING([--enable-unittests],[enable unittests (disabled by default)])],
  [enable_unittests=$enableval],
  [enable_unittests=no]
)


# enable flavortagging?
FLAVORTAGGING_CPPFLAGS=""
FLAVORTAGGING_LIBS=""
if test "x$enable_flavortagging" = "xyes"; then
  FLAVORTAGGING_CPPFLAGS="-DWITH_FLAVORTAGGING"
  dnl FLAVORTAGGING_LIBS="-lRaveFlavorTag -lCondFormats -lMVAComputer -lMVATrainer"
  FLAVORTAGGING_LIBS="-lRaveFlavorTag"
fi

# enable kinematics?
KINEMATICS_CPPFLAGS=""
KINEMATICS_LIBS=""
if test "x$enable_kinematics" = "xyes"; then
  KINEMATICS_CPPFLAGS="-DWITH_KINEMATICS"
  KINEMATICS_LIBS="-lRaveVertexKinematics"
fi

AC_SUBST(FLAVORTAGGING_CPPFLAGS)
AC_SUBST(FLAVORTAGGING_LIBS)
AC_SUBST(KINEMATICS_CPPFLAGS)
AC_SUBST(KINEMATICS_LIBS)

AM_CONDITIONAL([HAS_FLAVORTAGGING],[test "x$enable_flavortagging" = "xyes"])
AM_CONDITIONAL([HAS_KINEMATICS],[test "x$enable_kinematics" = "xyes"])
AM_CONDITIONAL([HAS_ZVRES],[test "x$enable_zvres" = "xyes"])
AM_CONDITIONAL([HAS_TTF],[test "x$enable_ttf" = "xyes"])
AM_CONDITIONAL([HAS_GSF],[test "x$enable_gsf" = "xyes"])
AM_CONDITIONAL([HAS_MVF],[test "x$enable_mvf" = "xyes"])
AM_CONDITIONAL([HAS_TKVF],[test "x$enable_tkvf" = "xyes"])
AM_CONDITIONAL([HAS_DEVELOPERS],[test "x$enable_developers" = "xyes"])
AM_CONDITIONAL([HAS_UNITTESTS],[test "x$enable_unittests" = "xyes"])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_LIBTOOL_WIN32_DLL
AC_PROG_LIBTOOL
AC_CHECK_PROGS([SWIG], [swig])
AC_CHECK_PROGS([JAVAC], [javac gcj])
AC_CHECK_PROGS([JAVA], [java])
AC_CHECK_PROGS([JAR], [fastjar jar])
AC_PROG_INSTALL
AC_PROG_MAKE_SET

# Checks for libraries.
AX_BOOST_BASE([1.32.0])

AC_SEARCH_CLHEP([], [AC_MSG_ERROR([required clhep not found])])
# AC_CEDAR_LIBRARY([CLHEP], [?.?.?.?],[],[AC_MSG_ERROR([CLHEP not found])])
# AC_CEDAR_LIBRARY([CLHEP-Matrix], [?.?.?.?],[],[AC_MSG_ERROR([CLHEP-Matrix not found])])

JAVALIBRARY="JavaRave"

PLATFORMLDFLAGS=
PLATFORMCPPFLAGS=-DRaveDllExport=""
case "${host}" in
    *-*-cygwin | *-*-pw32* )
# PLATFORMLDFLAGS="-stdlib -no-undefined -shared -Wl,--enable-auto-import -Wl,--export-all-symbols -Wl,--allow-shlib-undefined -Wl,--allow-multiple-definition -Wl,-u__pei386_runtime_relocator";
        PLATFORMLDFLAGS="-mno-cygwin -Wl,--enable-auto-import -Wl,--export-all-symbols";
        PLATFORMCPPFLAGS="-mno-cygwin";;
    *-*-mingw* )
        PLATFORMLDFLAGS="-mno-cygwin -Wl,--enable-auto-import -Wl,--export-all-symbols -Wl,--allow-multiple-definition";
        PLATFORMCPPFLAGS="-mno-cygwin";;
esac
AC_SUBST(PLATFORMLDFLAGS)
AC_SUBST(PLATFORMCPPFLAGS)

# get correct include path for java

create_dlls="no"

HOST_INCLUDE="genunix"
case "${host}" in
    *-*-cygwin | *-*-pw32* )
        HOST_INCLUDE="win32"; create_dlls="yes";;
    *-*-mingw* )
        HOST_INCLUDE="linux"; create_dlls="yes";;
    *linux*)
        HOST_INCLUDE="linux";;
    *solaris*)
        HOST_INCLUDE="solaris";;
    *bsdi*)
        HOST_INCLUDE="bsdos";;
    *osf*)
        HOST_INCLUDE="alpha";;
esac

AM_CONDITIONAL([IS_WIN32],[test "x$create_dlls" = "xyes"])

# JAVA
AC_ARG_ENABLE(java, AC_HELP_STRING([--disable-java],[disable the Java binding]),,
              enable_java="yes")
AC_ARG_WITH(java-home,
            [  --with-java-home=(JAVA_HOME)  set the full path to the java home directory.])
JAVAC_FLAGS=
JDK_INCLUDE=
# ${JAVAROOT} ${JAVA_HOME}

if test "X$with_java_home" != "X"; then
  JDK_INCLUDE="$with_java_home"
fi

if test "x$JAVA" = "xjava" && test "x$JAVAC" = "xjavac"; then                                                                         JAVA_VERSION=`$JAVA -version 2>&1 | $SED -ne 's/java version "\([^"]*\)".*/\1/p' 2>/dev/null`
  if test "x$enable_java" = "xyes"; then

    OLD_CPPFLAGS="$CPPFLAGS"
    CPPFLAGS="-I$JDK_INCLUDE/include -I$JDK_INCLUDE/include/$HOST_INCLUDE -I$JAVAROOT/include -I$JAVAROOT/include/$HOST_INCLUDE -I$JAVA_HOME/include -I$JAVA_HOME/include/$HOST_INCLUDE"
    AC_CHECK_HEADER(jni.h, [enable_java=yes],[enable_java=no])
    CPPFLAGS="$OLD_CPPFLAGS"

  fi
  if test "x$enable_java" = "xyes"; then
    JAVAC_FLAGS="-fno-strict-aliasing" # needed, otherwise optimised code doesnt work!
  fi
fi

if test "x$enable_java" = "xyes"; then
  if test "x$JAVA" = "x"; then
    AC_MSG_ERROR([ *** java not found, but java-support requested. (you may want to try to configure with --disable-java) *** ])
	fi
  if test "x$JAVAC" = "x"; then
    AC_MSG_ERROR([ *** javac not found, but java-support requested. (you may want to try to configure with --disable-java) *** ])
	fi
  if test "x$JAR" = "x"; then
    AC_MSG_ERROR([ *** jar not found, but java-support requested. (you may want to try to configure with --disable-java) *** ])
	fi
fi

dnl If we found a java version information, we have java compilation environment.
#if test "X$JAVA_VERSION" != "X"; then
#else
#  enable_java=no
#  JAVA_VERSION=""
#fi

dnl Conditional java sub dir test.
AM_CONDITIONAL([JAVA_ENABLED],[test "x$enable_java" = "xyes"])

AC_SUBST(JAVA_VERSION)
AC_SUBST(JAVAC_FLAGS)
AC_SUBST(HOST_INCLUDE)
AC_SUBST(JDK_INCLUDE)
AC_SUBST(JAVALIBRARY)

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([float.h limits.h stdlib.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T
AC_STRUCT_TM
AC_CHECK_TYPES([ptrdiff_t])

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_FUNC_MALLOC
AC_FUNC_STRTOD
AC_CHECK_FUNCS([floor memset pow sqrt strstr strtol strtoul])

AC_PATH_PROG(PKG_CONFIG, [pkg-config], [no])
if test x$PKG_CONFIG = xno ; then
  AC_MSG_ERROR([*** pkg-config not found. See http://www.freedesktop.org/software/pkgconfig/])
fi

if $PKG_CONFIG --atleast-pkgconfig-version 0.14 ; then
  :
else
  AC_MSG_ERROR([*** pkg-config too old; version 0.14 or better required.])
fi

AC_CONFIG_FILES([
rave.pc
rave-impl.pc
rave.spec
rave_autoconf.i
RaveConfig.cmake
Makefile
src/Makefile
src/RecoVertex/Makefile
src/RecoVertex/KinematicFit/Makefile
src/RecoBTag/Makefile
src/RaveBase/Makefile
tests/Makefile
UnitTests/Makefile
java/Makefile
java/tests/Makefile
])

AC_OUTPUT

AC_MSG_RESULT(

=============
Configuration
=============
Compiler:               ${CXX}
CXXFLAGS:               ${CXXFLAGS}
LDFLAGS:                ${LDFLAGS}
Install prefix:         ${prefix}
Java binding:           ${enable_java}
JDK_INCLUDE:            ${JDK_INCLUDE}
Create windows dlls:    ${create_dlls}
flavortagging:          ${enable_flavortagging}     (enable flavor tagging code)
kinematics:             ${enable_kinematics}     (enable kinematic fitter)
Zvres:                  ${enable_zvres}     (Zvres vertex finder)
Ttf:                    ${enable_ttf}     (tertiary tracks vertex finder)
Gsf:                    ${enable_gsf}     (Gaussian Sum Filter)
Mvf:                    ${enable_mvf}     (Multi Vertex Fitter)
Tkvf:                   ${enable_tkvf}     (trimmed kalman vertex finder)
Developers:             ${enable_developers}     (Special developers classes enabled)
Unit tests:             ${enable_unittests}     (Build also unit tests? Disabled in the default because of dependency issues.)
)
