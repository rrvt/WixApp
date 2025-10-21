// Thread Display -- Protected display

/*
    #include <type_traits> // For std::is_same

    template <typename T>
    void process_data(T data) {
        if constexpr (std::is_same_v<T, int>) {
            // Code specific to int type
            std::cout << "Processing an integer: " << data << std::endl;
        } else if constexpr (std::is_same_v<T, double>) {
            // Code specific to double type
            std::cout << "Processing a double: " << data << std::endl;
        } else {
            // Generic code for other types
            std::cout << "Processing a generic type." << std::endl;
        }
    }
*/




#pragma once
//#include <type_traits>
#include "Lock.h"


template <typename T, int initN>
class ThDisplayT {

Lock lock;

public:

  ThDisplayT() {lock.release();}
 ~ThDisplayT() { }

  void operator() (const T* fmt, ...);

private:

  virtual void dspCStg(const T* p) { }
  };




template <typename T, int initN>
void ThDisplayT<T, initN>::operator() (const T* fmt, ...) {
NewArray(T);
int           n = initN;
va_list       ap;
unique_ptr<T> formatted;
int           noChars;

  lock.own();
    loop {

      formatted.reset(AllocArray(n));  // Wrap the plain Tchar array into the
                                                      // unique_ptr
      va_start(ap, fmt);
        if constexpr(is_same_v<T, Tchar>) noChars = _vsntprintf_s(formatted.get(), n, n-1, fmt, ap);
        else                              noChars = _vsnprintf_s( formatted.get(), n, n-1, fmt, ap);
      va_end(ap);

      if (noChars >= 0) break;

      n *= 2;   FreeArray(formatted.release());
      }

    dspCStg(formatted.get());   FreeArray(formatted.release());
  lock.release();
  }



///-------------------

#if 0
    s = formatted.get();

    if (!s.isEmpty()) {
      for (pos = s.find(_T('\n')); pos >= 0; pos = s.find(_T('\n')))
                              {t = s.substr(0, pos);   notePad << t << nCrlf;   s = s.substr(pos+1);}

      if (!s.isEmpty()) notePad << s;   invalidate();
      }
#endif

