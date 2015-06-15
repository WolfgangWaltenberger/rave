#include "PropertyHandler.h"

void PropertyHandler::PrintProperties()
{ 
   std::cout <<"Properties of object:"<<std::endl; 
   PrintProperties_int();
   PrintProperties_double();
   PrintProperties_string();
   PrintProperties_bool();
   std::cout <<std::endl;
}

void PropertyHandler::ResetProperties()
{ 
   ResetProperties_int();
   ResetProperties_double();
   ResetProperties_string();
   ResetProperties_bool();
}

void PropertyHandler::KeyAlreadyDefinedError(const char* name)
{
   std::string key(name);
   std::cout << "PropertyHandler ERROR key="<<name<<" of type="<<fAllKeys[key]<<" is already defined, exiting..."<<std::endl;
   fIsConsistent = false;
#ifndef NO_EXIT_ON_ERROR
   exit(1);
#endif
}

void PropertyHandler::KeyNotFoundError(const char* name)
{
   std::string key(name);
   if (fAllKeys.find(key)==fAllKeys.end()){
      std::cout << "PropertyHandler could not find key="<<name<<", exiting..."<<std::endl;
      PrintProperties();
#ifndef NO_EXIT_ON_ERROR
      exit(1);
#endif
   }
   else{
      std::cout << "PropertyHandler: wrong data type used with SetProperty(). Key="<<name<<" must have data type="<<fAllKeys[key]<<", exiting..."<<std::endl;  
      //PrintProperties();
#ifndef NO_EXIT_ON_ERROR
      exit(1);
#endif
   }
}

