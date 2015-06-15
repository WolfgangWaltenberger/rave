#include "Environment/Configuration/interface/SimpleConfigurable.h"

#include <iostream>

using namespace std;

int main( int argc, char ** argv )
{
	cout << "Setting default values.." << endl; 
	SimpleConfigurable<string> Caption("ConfigurationTest", "Caption");
	SimpleConfigurable<int> Height(600, "Height");
	SimpleConfigurable<int> Width(600, "Width");
	SimpleConfigurable<bool> Success(true, "Success");
	
	cout << "Retrieved values:" << endl
		 << "\tCaption\t" << Caption.value() << endl
		 << "\tHeight\t"  << Height.value() << endl
		 << "\tWidth\t"   << Width.value() << endl
		 << "\tSuccess\t" << Success.value() << endl;
	
	return 0;
}
