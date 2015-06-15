#ifndef NEURALNETCONFIG_H
#define NEURALNETCONFIG_H
#ifdef _WIN32
# pragma once
# ifdef DLL
#  ifdef BUILDNEURALNET
#   ifndef NEURALNETDLL
#    define NEURALNETDLL __declspec(dllexport)
#   endif
#   ifndef NEURALNETDLLTMPL
#    define NEURALNETDLLTMPL
#   endif
#  else
#   ifndef NEURALNETDLL
#    define NEURALNETDLL __declspec(dllimport)
#   endif
#   ifndef NEURALNETDLLTMPL
#    define NEURALNETDLLTMPL extern
#   endif
#  endif
# endif
# ifdef __CINT__
#  ifdef NEURALNETDLL
#   undef NEURALNETDLL
#  endif
#  ifdef NEURALNETDLLTMPL
#   undef NEURALNETDLLTMPL
#  endif
# endif
#endif
#ifndef NEURALNETDLL
# define NEURALNETDLL
#endif
#ifndef NEURALNETDLLTMPL
# define NEURALNETDLLTMPL
#endif

#endif
