#ifndef PROPERTYHANLDER__H_
#define PROPERTYHANLDER__H_

//  Author E. von Toerne, U. of Bonn 2010

#include <iostream>
#include <map>
#include <string>
using std::string;
#include <cstdlib>

// uncomment this line to avoid exception upon error  #define NO_EXIT_ON_ERROR

#define stringof(X) #X 
// preprocessor macro to define type-specific functions
#define MYFUNCS(VAR) \
private: std::map<std::string , VAR* >    fMap_##VAR ; \
private: std::map<std::string , VAR >    fDefault_##VAR ; \
protected:\
   bool DeclareProperty(const char* name, VAR * thepoi, VAR defValue){\
      std::string key(name);\
      if (fAllKeys.find(key)==fAllKeys.end()){\
         *thepoi=defValue;\
         fMap_##VAR[key]=thepoi;\
         fDefault_##VAR[key]=defValue;\
         fAllKeys[key]=std::string(stringof(VAR)); \
         return true;\
      }\
      else {\
         KeyAlreadyDefinedError(name);\
         return false;\
      }\
   }\
public:\
   void PrintProperties_##VAR(){\
      std::map<std::string, VAR * >::iterator it;\
      for (it = fMap_##VAR.begin();it != fMap_##VAR.end(); it++)\
         std::cout <<it->first <<"="<<*(it->second)<<std::endl;\
   }\
   void ResetProperties_##VAR(){\
      std::map<std::string, VAR * >::iterator it;\
      for (it = fMap_##VAR.begin();it != fMap_##VAR.end(); it++)\
         *(it->second)=fDefault_##VAR[it->first];\
   }\
   bool SetProperty(const char* name, VAR value){\
      std::string key(name);\
      if (fMap_##VAR.find(key)==fMap_##VAR.end()){\
         KeyNotFoundError(name);\
         return false;\
      }\
      else {\
         *(fMap_##VAR[key])=value;\
         return true;\
      }\
   } 

/*! Light-weight implementation of a propertyhandler with strong type-checking. Much simpler than a full-scale property handler. Uses a preprocessor macro to create type-specific functions. E. von Toerne, U. of Bonn 2010
 */
class PropertyHandler {

private:
   bool fIsConsistent; // //!< false if DeclarePropertyCommands are inconsistent
   std::map<std::string,std::string> fAllKeys;         //!< map of all keys, name and type
   void KeyAlreadyDefinedError(const char* name);
   void KeyNotFoundError(const char* name);

MYFUNCS(int)
MYFUNCS(double)
MYFUNCS(string)
MYFUNCS(bool)

public: 
   PropertyHandler() : fIsConsistent(true) { }
   virtual ~PropertyHandler() { }

   bool SetProperty(const char* name, const char* value){ return SetProperty(name, string(value)); }
   bool DeclareProperty(const char* name, string* var, const char* value){ return DeclareProperty(name, var, string(value)); }
   bool DeclareProperty(const string& name, string* var, const char* value){ return DeclareProperty(name.data(), var, string(value)); }
   bool DeclareProperty(const string& name, string* var, const string& value){ return DeclareProperty(name.data(), var, string(value)); }
   void PrintProperties();
   void ResetProperties();
   bool IsConsistent(){ return fIsConsistent;}

};

#endif
