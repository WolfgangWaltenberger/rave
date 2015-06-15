//
//  Author E. von Toerne, U. of Bonn 2011
//
//  Boost-based unit tests, see www.boost.org for details
#include <boost/test/unit_test.hpp>
#include "RaveTest.h"
#include <rave/Version.h>
#include <cstdio>
#include <iostream>

BOOST_AUTO_TEST_CASE(ZeroBFields)
{
   std::cout << "ZeroB"<<std::endl;
   RaveTest* test = RaveTest::Get();
   test->ResetProperties();
   // this test fails in 0.3.91 and works in 0.5.4test
   test->SetProperty("NVtx",2);
   test->SetProperty("BField",0.0);
   for (int ialg=0;ialg<2;ialg++){
      switch (ialg) {
      case 0: test->SetProperty("Algorithm","avr"); break;
      case 1: test->SetProperty("Algorithm","mvf-ini:(finder:avr)"); break;
      default: break;
      }
      BOOST_CHECK(test->run()); 
   }
}
