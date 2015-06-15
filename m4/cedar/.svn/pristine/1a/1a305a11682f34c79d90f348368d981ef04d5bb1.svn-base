
# Check whether we can link against Rivet
#AC_TEST_RIVET(actionIfWorks, actionIfFails)
AC_DEFUN([AC_TEST_RIVET],[
oldLIBS="$LIBS"
oldLDFLAGS="$LDFLAGS"
oldCPPFLAGS="$CPPFLAGS"
LIBS="$LIBS -l$RIVETLIBNAME -lHepMC"
LDFLAGS="$LDFLAGS -L$RIVETLIBPATH"
CPPFLAGS="$CPPFLAGS -I$RIVETINCPATH"

AC_CHECK_HEADERS([Rivet/AnalysisHandler.hh Rivet/Analyses/TestAnalysis.hh])
AC_MSG_CHECKING([that Rivet works])
AC_TRY_LINK([#include "Rivet/AnalysisHandler.hh"
#include "Rivet/Analyses/TestAnalysis.hh"
namespace Rivet {}], 
[using namespace Rivet; 
AnalysisHandler rivet; 
rivet.addAnalysis(TestAnalysis());
rivet.init(); ],
[AC_MSG_RESULT(yes)
$1],
[AC_MSG_RESULT([no]) 
AC_MSG_NOTICE([Rivet does not work])
$2])

LIBS="$oldLIBS"
LDFLAGS="$oldLDFLAGS"
CPPFLAGS="$oldCPPFLAGS"
])
