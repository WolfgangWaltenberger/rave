/*
 * @file Utilities.cpp
 * @date 2006-03-28
 *
 * This short program tests the funtionality of the Utilites components
 * Please expand on demand.
 *
 */

#define HaveException
#ifdef  HaveException
#include "FWCore/Utilities/interface/Exception.h"
#endif

#include <string>
#include <iostream>

int main( int argc, char ** argv )
{
#ifdef HaveException
    try
    {
        throw cms::Exception( "test", "string message" );
    }
    catch ( cms::Exception ex )
    {
        std::cout << ex.what() << std::endl;
    };

    try
    {
        throw cms::Exception( "stream test" ) << "stream message at line " << 32;
    }
    catch ( cms::Exception ex )
    {
        std::cout << ex << std::endl;
    };
#endif

    std::cout << std::endl;
};
