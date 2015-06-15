#ifndef _Utilities_Exception_H_
#define _Utilities_Exception_H_

#include <string>
#include <list>
#include <sstream>

#include "boost/type_traits/is_base_and_derived.hpp"
#include "boost/type_traits/is_polymorphic.hpp"

namespace cms
{ 
   
  namespace detail
  {
    // The struct template Desired exists in order to allow us to use
    // SFINAE to control the instantiation of the stream insertion
    // member template needed to support streaming output to an object
    // of type cms::Exception, or a subclass of cms::Exception.

    template <class T, bool b> struct Desired;
    template <class T> struct Desired<T, true> { typedef T type; };


    // The following struct template is a metafunction which combines
    // two of the boost type_traits metafunctions.    

    template <class BASE, class DERIVED>
    struct is_derived_or_same
    {
      static const bool value = 
	boost::is_base_and_derived<BASE,DERIVED>::value || 
        boost::is_same<BASE,DERIVED>::value;
    };

  }

  class Exception
  {
  public:
    typedef std::string Category;
	typedef std::string Foostr;
    typedef std::list<Category> CategoryList;

    explicit Exception(const Category& category);
    Exception(const Category& category,
	      const std::string& message);
    Exception(const Category& category,
	      const std::string& message,
	      const Exception& another);
    Exception(const Exception& other);
    virtual ~Exception();

    std::string what() const;
    std::string explainSelf() const;
    std::string category() const;

    const CategoryList& history() const;
    std::string rootCause() const;

    void append(const Exception& another);
    void append(const std::string& more_information);
    void append(const char* more_information);

    // In the following templates, class E is our Exception class or
    // any subclass thereof. The complicated return type exists to
    // make sure the template can only be instantiated for such
    // classes.
    //

    // We have provided versions of these templates which accept, and
    // return, reference-to-const objects of type E. These are needed
    // so that the expression:
    //
    //     throw Exception("category") << "some message";
    //
    // shall compile. The technical reason is that the un-named
    // temporary created by the explicit call to the constructor
    // allows only const access, except by member functions. It seems
    // extremely unlikely that any Exception object will be created in
    // read-only memory; thus it seems unlikely that allowing the
    // stream operators to write into a nominally 'const' Exception
    // object is a real danger.

    template <class E, class T>
    friend
    typename detail::Desired<E, detail::is_derived_or_same<Exception,E>::value>::type &
    operator<<(E& e, const T& stuff);

    template <class E, class T>
    friend
    typename detail::Desired<E, detail::is_derived_or_same<Exception,E>::value>::type const&
    operator<<(E const& e, const T& stuff);

    template <class E>
    friend
    typename detail::Desired<E, detail::is_derived_or_same<Exception,E>::value>::type &
    operator<<(E& e, std::ostream&(*f)(std::ostream&));

    template <class E>
    friend
    typename detail::Desired<E, detail::is_derived_or_same<Exception,E>::value>::type const&
    operator<<(E const& e, std::ostream&(*f)(std::ostream&));
  
    template <class E>
    friend
    typename detail::Desired<E, detail::is_derived_or_same<Exception,E>::value>::type &
    operator<<(E& e, std::ios_base&(*f)(std::ios_base&));

    template <class E>
    friend
    typename detail::Desired<E, detail::is_derived_or_same<Exception,E>::value>::type const&
    operator<<(E const& e, std::ios_base&(*f)(std::ios_base&));

  private:
    // data members
    std::ostringstream ost_;
    CategoryList category_;
  };


  inline 
  std::ostream& 
  operator<<(std::ostream& ost, const Exception& e)
  {
    ost << e.what();
    return ost;
  }

  // -------- implementation ---------

  template <class E, class T>
  inline
  typename detail::Desired<E, detail::is_derived_or_same<Exception,E>::value>::type &
  operator<<(E& e, const T& stuff)
  {
    e.ost_ << stuff;
    return e;
  }

  template <class E, class T>
  inline
  typename detail::Desired<E, detail::is_derived_or_same<Exception,E>::value>::type const&
  operator<<(E const& e, const T& stuff)
  {
    E& ref = const_cast<E&>(e);
    ref.ost_ << stuff;
    return e;
  }

  template <class E>
  inline 
  typename detail::Desired<E, detail::is_derived_or_same<Exception,E>::value>::type &
  operator<<(E& e, std::ostream&(*f)(std::ostream&))
  {
    f(e.ost_);
    return e;
  }

  template <class E>
  inline 
  typename detail::Desired<E, detail::is_derived_or_same<Exception,E>::value>::type const&
  operator<<(E const& e, std::ostream&(*f)(std::ostream&))
  {
    E& ref = const_cast<E&>(e);
    f(ref.ost_);
    return e;
  }
  
  template <class E>
  inline
  typename detail::Desired<E, detail::is_derived_or_same<Exception,E>::value>::type & 
  operator<<(E& e, std::ios_base&(*f)(std::ios_base&))
  {
    f(e.ost_);
    return e;
  }

  template <class E>
  inline
  typename detail::Desired<E, detail::is_derived_or_same<Exception,E>::value>::type const& 
  operator<<(E const& e, std::ios_base&(*f)(std::ios_base&))
  {
    E& ref = const_cast<E&>(e);
    f(ref.ost_);
    return e;
  }

}

#endif
