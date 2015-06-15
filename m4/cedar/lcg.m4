# Get the LCG platform tag (e.g. i686-slc5-gcc43-opt) for the
# current platform and set it to the $LCG_TAG variable.
# Based on the Genser platform detection script.
#AC_LCG_TAG()
#----------------------------------------
AC_DEFUN([AC_LCG_TAG], [
  AC_ARG_WITH([lcgtag], [AC_HELP_STRING([--with-lcgtag],
    [LCG platform tag, e.g. i686-slc5-gcc43-opt. Calculated automatically for Linux SLC and Mac OS X])])
  LCG_TAG="$with_lcgtag"
  if test "x$LCG_TAG" = x; then
    AC_MSG_NOTICE([Calculating LCG platform tag from system properties])
    AC_MSG_CHECKING([for OS type])
    os=UNKNOWN
    distribution=`uname`
    if `echo $distribution | grep -i "Linux" &> /dev/null`; then os=LINUX; fi
    if test -e "/etc/redhat-release"; then
      distribution="redhat"
      sltest=`cat /etc/redhat-release | grep "Scientific Linux" 2> /dev/null`
      if test -n "$sltest"; then
        os=SLC
        sl_version=`cat /etc/redhat-release | cut -f5 -d" "`
        sl_major=`echo $sl_version | cut -f1 -d.`
        distribution="slc${sl_major}"
      fi
    fi
    ## Test for Mac OS X
    SW_VERS=""
    #AC_PATH_PROG([SWVERS], [sw_vers])
    if test -x "$SWVERS"; then
      os=OSX
      osx_version=`sw_vers -productVersion | cut -f1,2 -d.`
      osx_major=`echo $osx_version | cut -f1 -d.`
      osx_minor=`echo $osx_version | cut -f2 -d.`
      distribution="mac$osx_major$osx_minor"
    fi
    ## TODO: deal with Windows ;(
    AC_MSG_RESULT([$distribution])

    ## Get architecture
    AC_MSG_CHECKING([for architecture type])
    machine=`uname -m`
    AC_MSG_RESULT([$machine])

    ## Get compiler and version
    if test "$os" != "WINDOWS"; then
      if test -z "$CXX"; then
        AC_PATH_PROG(CXX, [g++])
      fi
      if test -x "$CXX"; then
        AC_MSG_CHECKING([for gcc/g++ version])
        gcc_version=`$CXX --version | head -1 | cut -d" " -f3`
        gcc_major=`echo $gcc_version | cut -d. -f1`
        gcc_minor=`echo $gcc_version | cut -d. -f2`
        gcc_micro=`echo $gcc_version | cut -d. -f3`
        gcc_code="gcc${gcc_major}${gcc_minor}"
        AC_MSG_RESULT([$gcc_code])
        compiler_code="$gcc_code"
      else
        AC_MSG_WARN([Could not find $CXX to check for version!])
        compiler_code="gccXX"
      fi
    else
      ## Try to find VC version... somehow!
      #import distutils.msvccompiler as msvc
      #vcversions = sorted(msvc.get_devstudio_versions())
      #vcversion = vcversions[0]
      compiler_code="vc" #%s" % vcversion
    fi

    ## The "future-proof" platform tag
    LCGPLATFORM="${machine}-${distribution}-${compiler_code}-opt"

    ## Historical exceptions (confirmed as future LCG practice by Stefan Roiser)
    if test "$os" = "SLC"; then
      if test "$sl_major" -lt 5; then
        historical=yes
      fi
    fi
    if test "$os" = "OSX"; then
      if test "$osx_minor" -lt 6; then
        historical=yes
        distribution="osx$osx_major$osx_minor"
      fi
    fi

    if test "$historical" = "yes"; then
      AC_MSG_NOTICE([Using old style LCG platform tags])

      ## Different arch codes
      machine="ia32"
      if test `echo $machine | grep 64 &> /dev/null`; then machine="amd64"; fi

      ## Historical exceptions for GCC version
      if test "$compiler_code" = "gcc32" -a "$compiler_code" = "gcc40"; then
        compiler_code="$compiler_code$gcc_micro"
      fi

      LCGPLATFORM="${distribution}_${machine}_${compiler_code}"
    fi

    LCG_TAG="$LCGPLATFORM"
  fi

  AC_SUBST(LCG_TAG)
  AC_MSG_NOTICE([LCG platform tag is '${LCG_TAG}'])
])
