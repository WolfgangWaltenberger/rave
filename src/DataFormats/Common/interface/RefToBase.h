#ifndef DataFormats_Common_RefToBase_h
#define DataFormats_Common_RefToBase_h
namespace edm {
  template <class T>
  class RefToBase
  {
     private:
       const T * theData;
  };
}
#endif
