// Python & Java convenience

%{
#include "vcconfig.h"
#include <sstream>
%}

%include "vcconfig.h"

%define STRINGHELPER(classname)
%extend classname {
    char * toString() {
		static char temp[512];
        std::ostringstream os;
		os << (*self);
		strcpy_s(temp, os.str().c_str());
        return &temp[0];
    }
    
    std::string __str__() {
        static char temp[512];
        std::ostringstream os;
		os << (*self);
		strcpy_s(temp, os.str().c_str());
        return &temp[0];
    }
};
%enddef // STRINGHELPER

%include "rave.i"

%pragma(java) jniclasscode = %{
	static {
		// String libname = System.mapLibraryName( "rave" );
		String libname = "Rave";
		System.out.println( "Trying to load " + libname + " ..." );

		try {
			System.loadLibrary ( libname );
			System.out.println( libname + " successfully loaded!" );
		} catch( SecurityException e ) {
			System.out.println( libname + " not loaded!" );
			e.printStackTrace();
		} catch(UnsatisfiedLinkError e) {
			System.out.println( libname + " not loaded!" );
			e.printStackTrace();
		}
	}
%}
