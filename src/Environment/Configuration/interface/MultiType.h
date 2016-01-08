#ifndef __MultiType__H
#define __MultiType__H

#include <string>
#include <vector>
#include <iostream>

namespace dataharvester {
class MultiType {
public:
  /**
   *  \class MultiType
   *  Weak typing in a strongly typed programming language
   */
  MultiType ( const std::string & data );
  MultiType ( const char * );
  MultiType ( long signed data );
  MultiType ( int data );
  MultiType ( double data );
  MultiType ( float data );
  MultiType ( bool data );
  MultiType ( const std::vector < double > & data );
  MultiType ( const std::vector < long > & data );

  MultiType ();
  operator double() const;
  /* operator std::vector < double > () const;
  operator std::vector < long > () const; */
  operator float() const;
  operator std::string() const;
  operator long() const;
  operator int() const;
  operator unsigned() const;
  operator bool() const;

  /// In an array, return the nth element,
  /// In a non-array like object, return
  /// this (for n=0) or throw (for n>0).
  MultiType getElement ( int n ) const;
  MultiType operator[] ( int n ) const;

  /// Add the data to an array ( vector )
  void addToVector ( const MultiType & );
  void addToVector ( double data );
  void addToVector ( long data );

  std::string asString() const;
  double asDouble() const;
  float asFloat() const;
  long asLong() const;
  int asInt() const;
  bool asBool() const;
  std::vector < double > asDoubleVector() const;
  std::vector < long > asIntVector() const;

  //* try to rethink the assigned type
  void trimType();

  MultiType & operator= ( const std::string & );
  MultiType & operator= ( long signed );
  MultiType & operator= ( int );
  MultiType & operator= ( size_t );
  MultiType & operator= ( double );
  // unimplemented by now, using standard C++ cast currently
  //MultiType & operator= ( float );
  MultiType & operator= ( bool );
  MultiType & operator= ( const char * );
  MultiType & operator= ( const std::vector < double > & );
  MultiType & operator= ( const std::vector < long > & );

  bool operator== ( const char * ) const;
  bool operator== ( const MultiType &) const;
  bool operator== ( const std::vector < double > & ) const;
  bool operator!= ( const char * ) const;

  /**
   *  returns "double", "int", "string", or "bool". Note that isType gives more
   *  detailed info.
   */
  std::string isA() const;

  enum Type { kString, kInt32, kInt64, kDouble32, kDouble64,
              kBool, kDouble32Vec, kDouble64Vec, 
              kInt32Vec, kInt64Vec, kNone };

  // returns the typename of a given type
  static std::string typeName ( Type );

  Type isType() const; //< returns the exact type
  bool hasValue() const;
  void toDefault();
  void defineAs( Type ); //< fix the type
private:
  void clear(); //< clear all data, but dont touch type info 
  void hasNothing(); //< set all has* to false.
  Type stringIs() const; //< get the most restrictive type for the string
  int goodPrecision() const; //< work out a good precision for floats and doubles
private:
  /**
   *  make all private members mutable.  That way the MultiType can be treated
   *  like any other type (const correctness ...)
   *  Note that conversions are "kept"
   */
  mutable bool theTrimmable; // can it be trimmed?
  mutable double theDouble;
  mutable std::string theString;
  mutable long signed theInt;
  mutable std::vector < double > theDoubleVec;
  mutable std::vector < long > theIntVec;

  /**
   *  is it a double or has it been converted
   *  into a double before?
   */
  mutable bool hasDouble;
  /** 
   *  is it a string or has it been converted
   *  to a string before?
   */
  mutable bool hasString;
  /**
   *  Is it an int or has it been converted
   *  to an int before?
   */
  mutable bool hasInt;
  /**
   *  Is it a double vector or has it 
   *  been converted to a double vector before?
   */
  mutable bool hasDoubleVec;
  /**
   *  Is it a double vector or has it 
   *  been converted to a int vector before?
   */
  mutable bool hasIntVec;
  Type theType; //< What _is_ it?
};
}

std::ostream & operator << ( std::ostream & s, dataharvester::MultiType & );

#endif
