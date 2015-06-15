#include <iostream>
#include <rave/Version.h>

using namespace std;

int main( int argc, char ** argv )
{
  cout << "This is Rave Version " << rave::Version() << endl;
}
