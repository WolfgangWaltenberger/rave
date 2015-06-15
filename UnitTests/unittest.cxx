//
//  Author E. von Toerne, U. of Bonn 2011
//
//  Boost-based unit tests, see www.boost.org for details
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE MultipleVertexFits
#include <boost/test/unit_test.hpp>
#include "RaveTest.h"
#include <cstdio>
#include <iostream>

BOOST_AUTO_TEST_CASE(FirstRavePerformanceTest)
{
   std::cout << "FirstRavePerformanceTest"<<std::endl;
   RaveTest* test = RaveTest::Get();
   test->ResetProperties();

   const int nvtxmax=5;
   BOOST_CHECK(test->run());
   test->SetProperty("Seed",3);
   BOOST_CHECK(test->run()); 

   test->SetProperty("AvNtrk", 10.);
   for (int ialg=0;ialg<2;ialg++){
      switch (ialg) {
      case 0: test->SetProperty("Algorithm","avr"); break;
      case 1: test->SetProperty("Algorithm","mvf-ini:(finder:avr)"); break;
      default: break;
      }
      for (int i=5; i<=nvtxmax; i+=5){
         test->SetProperty("Seed",100+i);
         test->SetProperty("NVtx",i);
         BOOST_CHECK(test->run()); 
      }
   }

}
