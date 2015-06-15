#include <iostream>
#include "CommonTools/Statistics/interface/ChiSquaredProbability.h"

using namespace std;

int main()
{
  cout << "test" << endl;
  cout << "P(3.5,2.2)=" << ChiSquaredProbability(3.5,2.) << endl;
}
