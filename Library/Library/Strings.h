// Several String Classes

// String -- A wrapper of std::tstring
// Should handle automatic conversions from/to:
//  - tstring
//  - Cstring
//  - bstr_t
//  - variant_t
//  - const Tchar* (TCchar*)
//  - int
//  - long
//  - ulong
//  - double
// rrvt 08/02/16


#pragma once


#include "ToString.h"


class String;


class Cstring : public CString {
public:

  Cstring()          : CString() {}
  Cstring(TCchar* s) : CString(s) {}
  Cstring(String& s);

  int      stoi( uint& i, int base=10);

  void     clear()   {Empty();}
  bool     isEmpty() {return IsEmpty();}

  operator TCchar*() const {return GetString();}
  TCchar*      str() const {return GetString();}

  bool     loadRes(uint id) {return LoadString(id);}

  // Left Hand side of TCchar*, Cstring, bstr & variant_t

  Cstring& operator= (      Tchar     ch)  {CString& cs = *this; cs = ch; return *this;}
  Cstring& operator= (      TCchar*    s)  {CString& cs = *this; cs = s; return *this;}
  Cstring& operator= (      CString&   s)  {CString& cs = *this; cs = s; return *this;}
  Cstring& operator= (      String&    s);
  Cstring& operator= (      tstring&   s)  {CString& cs = *this; cs = s.c_str(); return *this;}
  Cstring& operator= (const tstring&   s)  {CString& cs = *this; cs = s.c_str(); return *this;}
  Cstring& operator= (      bstr_t    bs)  {CString& cs = *this; cs = (Tchar*) bs; return *this;}


  Cstring& operator= (variant_t& v);
  Cstring& operator= (int        v);
  Cstring& operator= (long       v);
  Cstring& operator= (ulong      v);
  Cstring& operator= (double     v);

  bool     operator== (Cstring& t) {return (CString) *this == (CString) t;}
  bool     operator!= (Cstring& t) {return (CString) *this != (CString) t;}
  bool     operator>  (Cstring& t) {return (CString) *this >  (CString) t;}
  bool     operator>= (Cstring& t) {return (CString) *this >= (CString) t;}
  bool     operator<  (Cstring& t) {return (CString) *this <  (CString) t;}
  bool     operator<= (Cstring& t) {return (CString) *this <= (CString) t;}
  };


inline bool operator== (Cstring& s, TCchar* t) {return s.Compare(t) == 0;}
inline bool operator!= (Cstring& s, TCchar* t) {return s.Compare(t) != 0;}
inline bool operator>  (Cstring& s, TCchar* t) {return s.Compare(t) >  0;}
inline bool operator>= (Cstring& s, TCchar* t) {return s.Compare(t) >= 0;}
inline bool operator<  (Cstring& s, TCchar* t) {return s.Compare(t) <  0;}
inline bool operator<= (Cstring& s, TCchar* t) {return s.Compare(t) <= 0;}


class String : public tstring {
public:

static const int npos = -1;

// Initialization

  String()                        : tstring() {}
  String(String&    v)            : tstring(v.str()) {}
  String(tstring    t)            : tstring(t) {}
  String(TCchar*    stg)          : tstring(stg) {}
  String(TCchar*    stg, int cnt) : tstring(stg,cnt) {}
  String(Cstring&   stg)          : tstring(stg) {}
  String(CString&   stg)          : tstring(stg) {}
  String(bstr_t     bs)           : tstring(bs) {}
  String(variant_t& v)            {if (v.vt == VT_BSTR) *this = bstr_t(v);}
  String(Tchar      ch)           : tstring(1, ch) {}
  String(int count, Tchar ch)     : tstring(count, ch) {}
  String(int        v)            {String& s = *this; s.format(_T("%i"),  v);}
  String(long       v)            {String& s = *this; s.format(_T("%li"), v);}
  String(ulong      v)            {String& s = *this; s.format(_T("%lu"), v);}
  String(double     v)            {String& s = *this; s.format(_T("%lg"), v);}

// String Attributes

  int   length() {return (int) ((tstring*)this)->size();}      // size() also works on String
  int   size()   {return (int) ((tstring*)this)->size();}
  bool  isEmpty() {return  ((tstring*)this)->empty();}
  Tchar& operator[](int i) {tstring& s = *this; return s[i];}  // returns reference to character at i
                                                               // May be used on both sides of '='.
// Manipulating Functions

  void    clear() {((tstring*)this)->clear();}               // Erases all elements of a tstring

  String& trim();                                           // Remove spaces/tabs/etc from both ends
  String& trimLeft();                                       // Remove spaces/tabs/etc from left end
  String& trimRight();                                      // Remove spaces/tabs/etc from right end
  String& upperCase();                                      // Convert lower case to upper case
  String& lowerCase();                                      // ditto in reverse
  String& format(TCchar* fmt_str, ...);                     // Apply traditional printf formatting
                                                            // Note:  Strings are not recognized as input
// Assignment

  String& operator= (      Tchar    v) {tstring& s = *this; s = v;              return *this;}
  String& operator= (      Tchar*   v) {tstring& s = *this; s = v;              return *this;}
  String& operator= (      TCchar*  v) {tstring& s = *this; s = v;              return *this;}
  String& operator= (      CString& v) {tstring& s = *this; s = v;              return *this;}
  String& operator= (const CString& v) {tstring& s = *this; s = v;              return *this;}
  String& operator= (      Cstring& v) {tstring& s = *this; s = v;              return *this;}
  String& operator= (const Cstring& v) {tstring& s = *this; s = v;              return *this;}
  String& operator= (      bstr_t&  v) {tstring& s = *this; s = v;              return *this;}
  String& operator= (const bstr_t&  v) {tstring& s = *this; s = v;              return *this;}
  String& operator= (      int      v) {String&  s = *this; s.format(_T("%i"),  v); return *this;}
  String& operator= (      long     v) {String&  s = *this; s.format(_T("%li"), v); return *this;}
  String& operator= (      ulong    v) {String&  s = *this; s.format(_T("%lu"), v); return *this;}
  String& operator= (      double   v) {String&  s = *this; s.format(_T("%lg"), v); return *this;}
  String& operator= (const String&  v) {tstring& s = *this; s = v.str();        return *this;}
//  String& operator= (      String&  v) {tstring& s = *this; s = v.str();        return *this;}

  String& operator= (variant_t& t)
                           {if (t.vt == VT_BSTR) *this = (tstring) bstr_t(t); else clear(); return *this;}


// Interesting notion:  one cannot define an operator= that results in a non-String value.  Instead one
// must define casts to the types needed.  An explicit TCchar* is also included for format.

//  operator tstring&()       {return s;}
  operator TCchar*() const {return str();}    // Left Hand side of TCchar*, Cstring, bstr & variant_t
  TCchar*      str() const {return c_str();}

#ifdef _UNICODE
  operator LPWSTR() const {return (LPWSTR) str();}
#endif

// Concatenation operators
//  s += t;  where s is a String and t is a String, tstring, Cstring, bstr_t, variant_t,
//  const Tchar* (TCchar*), TCchar, int, long, ulong, double.

         String  operator+  (TCchar         c) {tstring x = *this; x += c;    return x;}
         String  operator+  (Tchar*         t) {tstring x = *this; x += t;    return x;}
         String  operator+  (TCchar*        t) {tstring x = *this; x += t;    return x;}
         String  operator+  (      tstring& t) {tstring x = *this; x += t;    return x;}
         String  operator+  (const tstring& t) {tstring x = *this; x += t;    return x;}
         String  operator+  (      String&  t) {tstring x = *this; x += t;    return x;}
         String  operator+  (const String&  t) {tstring x = *this; x += t;    return x;}
         String  operator+  (      Cstring& t) {tstring x = *this; x += t;    return x;}
         String  operator+  (const Cstring& t) {tstring x = *this; x += t;    return x;}
         String  operator+  (      CString& t) {tstring x = *this; x += t;    return x;}
         String  operator+  (const CString& t) {tstring x = *this; x += t;    return x;}
//       String  operator+  (variant_t&     t) {tstring x = *this; x += t;    return x;}
         String  operator+  (bstr_t&        t) {tstring x = *this; x += t;    return x;}
         String  operator+  (const bstr_t&  t) {tstring x = *this; x += t;    return x;}


         // Some simple numbers, more complex formatting is up to the engineer

         String  operator+  (int v)
                                {String s = *this; String t; t.format(_T("%li"), v); s += t; return s;}
         String  operator+  (long   v)
                                {String s = *this; String t; t.format(_T("%li"), v); s += t; return s;}
         String  operator+  (ulong  v)
                                {String s = *this; String t; t.format(_T("%lu"), v); s += t; return s;}
         String  operator+  (double v)
                                {String s = *this; String t; t.format(_T("%lg"), v); s += t; return s;}

         String& operator+= (Tchar          t) {tstring& s = *this; s += t; return *this;}
         String& operator+= (TCchar*        t) {tstring& s = *this; s += t; return *this;}
         String& operator+= (      tstring& t) {tstring& s = *this; s += t; return *this;}
         String& operator+= (const tstring& t) {tstring& s = *this; s += t; return *this;}
         String& operator+= (      String&  t) {tstring& s = *this; s += t; return *this;}
         String& operator+= (      Cstring& t) {tstring& s = *this; s += t; return *this;}
         String& operator+= (      bstr_t&  t) {tstring& s = *this; s += t; return *this;}
         String& operator+= (variant_t&     v) {if (v.vt == VT_BSTR) *this += bstr_t(v); return *this;}
         String& operator+= (int        v)
                          {tstring& s = *this; String t; t.format(_T("%i"),  v); s += t; return *this;}
         String& operator+= (long       v)
                          {tstring& s = *this; String t; t.format(_T("%li"), v); s += t; return *this;}
         String& operator+= (ulong      v)
                          {tstring& s = *this; String t; t.format(_T("%lu"), v); s += t; return *this;}
         String& operator+= (double     v)
                          {tstring& s = *this; String t; t.format(_T("%lg"), v); s += t; return *this;}
// Loop logic
// String           s;
// String::iterator p;
//    for (p = s.begin();  p != s.end(); p++) { ch = *p;  *p = ch; ... }
// or for (p = s.rbegin(); p != s.rend(); p++) {ch = *p;  *p = ch; ... }

typedef tstring::iterator         iterator;
typedef tstring::reverse_iterator reverseIterator;

  int find(Tchar   ch,  int offset=0) {tstring& s = *this; return (int) s.find(ch,  offset);}
  int find(TCchar* stg, int offset=0) {tstring& s = *this; return (int) s.find(stg, offset);}
  int find(TCchar* stg, int offset, int count)
                                          {tstring& s = *this; return (int) s.find(stg,  offset, count);}
  int find(const String& stg, int offset=0)
                                          {tstring& s = *this; return (int) s.find(stg, offset);}

  // Find last character in the string given a single character or a group of characters
  // offset -- Index at which the search is to finish

  int findLastOf(Tchar          ch,  int offset=npos)
                                  {tstring& s = *this; return (int) s.find_last_of(ch,  offset);}
  int findLastOf(TCchar*       stg, int offset=npos)
                                  {tstring& s = *this; return (int) s.find_last_of(stg, offset);}
  int findLastOf(TCchar*       stg, int offset, int count)
                                  {tstring& s = *this; return (int) s.find_last_of(stg,  offset, count);}
  int findLastOf(const String& stg, int offset=npos)
                                  {tstring& s = *this; return (int) s.find_last_of(stg, offset);}

// Conversion to numbers, integer, unsigned long integer and double
// returns the value, i contains the index of the first unconverted character

  int    stoi( uint& i, int base=10);
  ulong  stoul(uint& i, int base=10);
  double stod( uint& i);

  String substr(int off, int count = npos) {
    if (off >= length()) return _T("");                       // Patch for bug in s.substr
    tstring s = *this; return s.substr(off,count);
    }

 ~String() { }


  // Boolean operators

  bool operator== (      Tchar*   t) {return !compare(t);}
  bool operator== (      TCchar*  t) {return !compare(t);}
  bool operator== (      tstring& t) {return !compare(t);}
  bool operator== (const tstring& t) {return !compare(t);}
  bool operator== (      Cstring& t) {return !compare(t);}
  bool operator== (const Cstring& t) {return !compare(t);}
  bool operator== (      CString& t) {return !compare(t);}
  bool operator== (const CString& t) {return !compare(t);}
  bool operator== (      bstr_t&  t) {return !compare(t);}
  bool operator== (const bstr_t&  t) {return !compare(t);}
  bool operator== (variant_t&     t) {return !compare((String) t);}


  bool operator== (      String&  t) {return !compare(t);}
  bool operator== (const String&  t) {return !compare(t);}

  bool operator!= (      Tchar*   t) {return compare(t)         != 0;}
  bool operator!= (      TCchar*  t) {return compare(t)         != 0;}
  bool operator!= (      tstring& t) {return compare(t)         != 0;}
  bool operator!= (const tstring& t) {return compare(t)         != 0;}
  bool operator!= (      Cstring& t) {return compare(t)         != 0;}
  bool operator!= (const Cstring& t) {return compare(t)         != 0;}
  bool operator!= (      CString& t) {return compare(t)         != 0;}
  bool operator!= (const CString& t) {return compare(t)         != 0;}
  bool operator!= (      bstr_t&  t) {return compare(t)         != 0;}
  bool operator!= (const bstr_t&  t) {return compare(t)         != 0;}
  bool operator!= (variant_t&     t) {return compare(String(t)) != 0;}
  bool operator!= (      String&  t) {return compare(t)         != 0;}
  bool operator!= (const String&  t) {return compare(t)         != 0;}

  bool operator>  (      Tchar*   t) {return compare(t)          > 0;}
  bool operator>  (      TCchar*  t) {return compare(t)          > 0;}
  bool operator>  (      tstring& t) {return compare(t)          > 0;}
  bool operator>  (const tstring& t) {return compare(t)          > 0;}
  bool operator>  (      Cstring& t) {return compare(t)          > 0;}
  bool operator>  (const Cstring& t) {return compare(t)          > 0;}
  bool operator>  (      CString& t) {return compare(t)          > 0;}
  bool operator>  (const CString& t) {return compare(t)          > 0;}
  bool operator>  (      bstr_t&  t) {return compare(t)          > 0;}
  bool operator>  (const bstr_t&  t) {return compare(t)          > 0;}
  bool operator>  (variant_t&     t) {return compare(String(t))  > 0;}
  bool operator>  (      String&  t) {return compare(t)          > 0;}
  bool operator>  (const String&  t) {return compare(t)          > 0;}

  bool operator>= (      Tchar*   t) {return compare(t)         >= 0;}
  bool operator>= (      TCchar*  t) {return compare(t)         >= 0;}
  bool operator>= (      tstring& t) {return compare(t)         >= 0;}
  bool operator>= (const tstring& t) {return compare(t)         >= 0;}
  bool operator>= (      Cstring& t) {return compare(t)         >= 0;}
  bool operator>= (const Cstring& t) {return compare(t)         >= 0;}
  bool operator>= (      CString& t) {return compare(t)         >= 0;}
  bool operator>= (const CString& t) {return compare(t)         >= 0;}
  bool operator>= (      bstr_t&  t) {return compare(t)         >= 0;}
  bool operator>= (const bstr_t&  t) {return compare(t)         >= 0;}
  bool operator>= (variant_t&     t) {return compare(String(t)) >= 0;}
  bool operator>= (      String&  t) {return compare(t)         >= 0;}
  bool operator>= (const String&  t) {return compare(t)         >= 0;}

  bool operator<  (      Tchar*   t) {return compare(t)          < 0;}
  bool operator<  (      TCchar*  t) {return compare(t)          < 0;}
  bool operator<  (      tstring& t) {return compare(t)          < 0;}
  bool operator<  (const tstring& t) {return compare(t)          < 0;}
  bool operator<  (      Cstring& t) {return compare(t)          < 0;}
  bool operator<  (const Cstring& t) {return compare(t)          < 0;}
  bool operator<  (      CString& t) {return compare(t)          < 0;}
  bool operator<  (const CString& t) {return compare(t)          < 0;}
  bool operator<  (      bstr_t&  t) {return compare(t)          < 0;}
  bool operator<  (const bstr_t&  t) {return compare(t)          < 0;}
  bool operator<  (variant_t&     t) {return compare(String(t))  < 0;}
  bool operator<  (      String&  t) {return compare(t)          < 0;}
  bool operator<  (const String&  t) {return compare(t)          < 0;}

  bool operator<= (      Tchar*   t) {return compare(t)         <= 0;}
  bool operator<= (      TCchar*  t) {return compare(t)         <= 0;}
  bool operator<= (      tstring& t) {return compare(t)         <= 0;}
  bool operator<= (const tstring& t) {return compare(t)         <= 0;}
  bool operator<= (      Cstring& t) {return compare(t)         <= 0;}
  bool operator<= (const Cstring& t) {return compare(t)         <= 0;}
  bool operator<= (      CString& t) {return compare(t)         <= 0;}
  bool operator<= (const CString& t) {return compare(t)         <= 0;}
  bool operator<= (      bstr_t&  t) {return compare(t)         <= 0;}
  bool operator<= (const bstr_t&  t) {return compare(t)         <= 0;}
  bool operator<= (variant_t&     t) {return compare(String(t)) <= 0;}
  bool operator<= (      String&  t) {return compare(t)         <= 0;}
  bool operator<= (const String&  t) {return compare(t)         <= 0;}


private:

  friend class TokenString;
  };


inline bool operator== (      TCchar*  s, const String&  t) {return !t.compare(s);}
inline bool operator== (const tstring& s, const String&  t) {return !t.compare(s);}
inline bool operator== (const Cstring& s, const String&  t) {return !t.compare(s);}
inline bool operator== (      Cstring& s, const String&  t) {return !t.compare(s);}
inline bool operator== (const CString& s, const String&  t) {return !t.compare(s);}
inline bool operator== (      CString& s, const String&  t) {return !t.compare(s);}
inline bool operator== (const bstr_t&  s, const String&  t) {return !t.compare(s);}
inline bool operator== (const String&  s, const String&  t) {return !t.compare(s);}
inline bool operator== (variant_t&     s, const String&  t) {return !t.compare(String(s));}
inline bool operator== (const String&  t,       TCchar*  s) {return !t.compare(s);}
inline bool operator== (const String&  t, const tstring& s) {return !t.compare(s);}
inline bool operator== (const String&  t, const Cstring& s) {return !t.compare(s);}
inline bool operator== (const String&  t,       Cstring& s) {return !t.compare(s);}
inline bool operator== (const String&  t, const CString& s) {return !t.compare(s);}
inline bool operator== (const String&  t,       CString& s) {return !t.compare(s);}
inline bool operator== (const String&  t, const bstr_t&  s) {return !t.compare(s);}
inline bool operator== (const String&  t, variant_t&     s) {return !t.compare(String(s));}


inline bool operator!= (      TCchar*  s, const String&  t) {return t.compare(s) != 0;}
inline bool operator!= (const tstring& s, const String&  t) {return t.compare(s) != 0;}
inline bool operator!= (const Cstring& s, const String&  t) {return t.compare(s) != 0;}
inline bool operator!= (      Cstring& s, const String&  t) {return t.compare(s) != 0;}
inline bool operator!= (const CString& s, const String&  t) {return t.compare(s) != 0;}
inline bool operator!= (      CString& s, const String&  t) {return t.compare(s) != 0;}
inline bool operator!= (const bstr_t&  s, const String&  t) {return t.compare(s) != 0;}
inline bool operator!= (variant_t&     s, const String&  t) {return t.compare(String(s)) != 0;}
inline bool operator!= (const String&  s, const String&  t) {return t.compare(s) != 0;}
inline bool operator!= (const String&  t,       TCchar*  s) {return t.compare(s) != 0;}
inline bool operator!= (const String&  t, const tstring& s) {return t.compare(s) != 0;}
inline bool operator!= (const String&  t, const Cstring& s) {return t.compare(s) != 0;}
inline bool operator!= (const String&  t,       Cstring& s) {return t.compare(s) != 0;}
inline bool operator!= (const String&  t, const CString& s) {return t.compare(s) != 0;}
inline bool operator!= (const String&  t,       CString& s) {return t.compare(s) != 0;}
inline bool operator!= (const String&  t, const bstr_t&  s) {return t.compare(s) != 0;}
inline bool operator!= (const String&  t, variant_t&     s) {return t.compare(String(s)) != 0;}


inline bool operator>  (      TCchar*  s, const String&  t) {return t.compare(s) < 0;}
inline bool operator>  (const tstring& s, const String&  t) {return t.compare(s) < 0;}
inline bool operator>  (const Cstring& s, const String&  t) {return t.compare(s) < 0;}
inline bool operator>  (      Cstring& s, const String&  t) {return t.compare(s) < 0;}
inline bool operator>  (const bstr_t&  s, const String&  t) {return t.compare(s) < 0;}
inline bool operator>  (variant_t&     s, const String&  t) {return t.compare(String(s)) < 0;}
inline bool operator>  (const String&  t,       TCchar*  s) {return t.compare(s) > 0;}
inline bool operator>  (const String&  t, const tstring& s) {return t.compare(s) > 0;}
inline bool operator>  (const String&  t, const Cstring& s) {return t.compare(s) > 0;}
inline bool operator>  (const String&  t,       Cstring& s) {return t.compare(s) > 0;}
inline bool operator>  (const String&  t, const CString& s) {return t.compare(s) > 0;}
inline bool operator>  (const String&  t,       CString& s) {return t.compare(s) > 0;}
inline bool operator>  (const String&  t, const bstr_t&  s) {return t.compare(s) > 0;}
inline bool operator>  (const String&  t, variant_t&     s) {return t.compare(String(s)) > 0;}
inline bool operator>  (const String&  t, const String&  s) {return t.compare(s) > 0;}

inline bool operator>= (      TCchar*  s, const String&  t) {return t.compare(s) <= 0;}
inline bool operator>= (const tstring& s, const String&  t) {return t.compare(s) <= 0;}
inline bool operator>= (const Cstring& s, const String&  t) {return t.compare(s) <= 0;}
inline bool operator>= (      Cstring& s, const String&  t) {return t.compare(s) <= 0;}
inline bool operator>= (const CString& s, const String&  t) {return t.compare(s) <= 0;}
inline bool operator>= (      CString& s, const String&  t) {return t.compare(s) <= 0;}
inline bool operator>= (const bstr_t&  s, const String&  t) {return t.compare(s) <= 0;}
inline bool operator>= (variant_t&     s, const String&  t) {return t.compare(String(s)) <= 0;}
inline bool operator>= (const String&  t,       TCchar*  s) {return t.compare(s) >= 0;}
inline bool operator>= (const String&  t, const tstring& s) {return t.compare(s) >= 0;}
inline bool operator>= (const String&  t, const Cstring& s) {return t.compare(s) >= 0;}
inline bool operator>= (const String&  t,       Cstring& s) {return t.compare(s) >= 0;}
inline bool operator>= (const String&  t, const CString& s) {return t.compare(s) >= 0;}
inline bool operator>= (const String&  t,       CString& s) {return t.compare(s) >= 0;}
inline bool operator>= (const String&  t, const bstr_t&  s) {return t.compare(s) >= 0;}
inline bool operator>= (const String&  t, variant_t&     s) {return t.compare(String(s)) >= 0;}
inline bool operator>= (const String&  t, const String&  s) {return t.compare(s) >= 0;}

inline bool operator<  (      TCchar*  s, const String&  t) {return t.compare(s) > 0;}
inline bool operator<  (const tstring& s, const String&  t) {return t.compare(s) > 0;}
inline bool operator<  (const Cstring& s, const String&  t) {return t.compare(s) > 0;}
inline bool operator<  (      Cstring& s, const String&  t) {return t.compare(s) > 0;}
inline bool operator<  (const CString& s, const String&  t) {return t.compare(s) > 0;}
inline bool operator<  (      CString& s, const String&  t) {return t.compare(s) > 0;}
inline bool operator<  (const bstr_t&  s, const String&  t) {return t.compare(s) > 0;}
inline bool operator<  (variant_t&     s, const String&  t) {return t.compare(String(s)) > 0;}
inline bool operator<  (const String&  t,       TCchar*  s) {return t.compare(s) < 0;}
inline bool operator<  (const String&  t, const tstring& s) {return t.compare(s) < 0;}
inline bool operator<  (const String&  t, const Cstring& s) {return t.compare(s) < 0;}
inline bool operator<  (const String&  t,       Cstring& s) {return t.compare(s) < 0;}
inline bool operator<  (const String&  t, const bstr_t&  s) {return t.compare(s) < 0;}
inline bool operator<  (const String&  t, variant_t&     s) {return t.compare(String(s)) < 0;}
inline bool operator<  (const String&  t, const String&  s) {return t.compare(s) < 0;}

inline bool operator<= (      TCchar*  s, const String&  t) {return t.compare(s) >= 0;}
inline bool operator<= (const tstring& s, const String&  t) {return t.compare(s) >= 0;}
inline bool operator<= (const Cstring& s, const String&  t) {return t.compare(s) >= 0;}
inline bool operator<= (      Cstring& s, const String&  t) {return t.compare(s) >= 0;}
inline bool operator<= (const CString& s, const String&  t) {return t.compare(s) >= 0;}
inline bool operator<= (      CString& s, const String&  t) {return t.compare(s) >= 0;}
inline bool operator<= (const bstr_t&  s, const String&  t) {return t.compare(s) >= 0;}
inline bool operator<= (variant_t&     s, const String&  t) {return t.compare(String(s)) >= 0;}


inline bool operator<= (const String&  t,       TCchar*  s) {return t.compare(s) <= 0;}
inline bool operator<= (const String&  t, const tstring& s) {return t.compare(s) <= 0;}
inline bool operator<= (const String&  t, const Cstring& s) {return t.compare(s) <= 0;}
inline bool operator<= (const String&  t,       Cstring& s) {return t.compare(s) <= 0;}
inline bool operator<= (const String&  t, const CString& s) {return t.compare(s) <= 0;}
inline bool operator<= (const String&  t,       CString& s) {return t.compare(s) <= 0;}
inline bool operator<= (const String&  t, const bstr_t&  s) {return t.compare(s) <= 0;}
inline bool operator<= (const String&  t, variant_t&     s) {return t.compare(String(s)) <= 0;}
inline bool operator<= (const String&  s, const String&  t) {return t.compare(s) <= 0;}


// s = v + s1; where s and s1 are String(s) and v is tstring, Cstring, bstr_t, variant_t,
//  const Tchar* (TCchar*), Tchar (only to tstring), long, ulong, double.

template<class S> String operator+   (S& t, const String& s);
template<class S> String operator+<S>(S& t, const String& s) {return String(t) + s;}

template<class S> String operator+   (const S& t, const String& s);
template<class S> String operator+<S>(const S& t, const String& s) {return String(t) + s;}

template<class S, class T> String operator+      (S& s, T& t);
template<class S, class T> String operator+<T, S>(S& s, T& t) {String ss(s); return ss += t;}

template<class S, class T> String operator+      (S& s, const T& t);
template<class S, class T> String operator+<T, S>(S& s, const T& t) {String ss(s); return ss += t;}

template<class S, class T> String operator+      (const S& s, T& t);
template<class S, class T> String operator+<T, S>(const S& s, T& t) {String ss(s); return ss += t;}

template<class S, class T> String operator+      (const S& s, const T& t);
template<class S, class T> String operator+<T, S>(const S& s, const T& t) {String ss(s); return ss += t;}


template<class T> String toString(T t);


#ifndef Win2K
template<class T> String toString<T>(T t) {tstring r = to_tstring((T) t); return r;}
#else
template<typename T> String toString<T>(T t) {tstring r = to_tstring((T) t); return r;}
#endif


String dblToString(double v, int width, int precision);
String intToString(long   v, int width);
String uintToString(ulong v, int width);


class TokenString : public String {
int pos;

public:

  TokenString() : pos (0) {}

  TokenString& operator= (const String&  t) {String& s = *this; s = t; pos = 0; return *this;}

  bool next(String& stg, TCchar* delim);  // Returns next token in tstring defined by deliminter chars
  };


// Convert a String (Tchar, i.e. wide character) to Ansi string (8 bit char)

class ToAnsi {
int   cnt;
char* p;

public:

  ToAnsi(TCchar* s) : p(0), cnt(0) {convert(s);}
  ToAnsi(String& s) : p(0), cnt(0) {convert(s.str());}

  ~ToAnsi();

  char* operator() () {return p;}
  int   length() {return cnt;}

private:

  ToAnsi() : p(0), cnt(0) {}

  void convert(TCchar* tp);
  };




// Convert an Ansi string (8 bit char) to a UniCode String (Tchar, i.e. wide character)

class ToUniCode {
int    cnt;
Tchar* p;

public:

  ToUniCode(Cchar* s) : p(0), cnt(0) {convert(s);}

 ~ToUniCode();

  Tchar* operator() () {return p;}
  int    length()      {return cnt;}

private:

  ToUniCode() : p(0), cnt(0) {}

  void convert(Cchar* tp);
  };


/*
basic_string Class Visual Studio 2017Visual Studio 2022

Send Feedback on this topic to Microsoft. OnlineView this topic online in your default browser.

The sequences controlled by an object of template class basic_string are the Standard C++ tstring class
and are usually referred to as strings, but they should not be confused with the null-terminated C-style
strings used throughout the Standard C++ Library. The Standard C++ tstring is a container that enables the
use of strings as normal types, such as comparison and concatenation operations, iterators, STL
algorithms, and copying and assigning with class allocator managed memory. If you need to convert a
Standard C++ tstring to a null-terminated C-style tstring, use the basic_string::c_str member.

View ColorizedCopy to Clipboardtemplate <class CharType, class Traits = char_traits<CharType>,
                                                                  class Allocator = allocator<CharType>>
class basic_string;
template <class CharType, class Traits = char_traits<CharType>, class Allocator = allocator<CharType>>
class basic_string;

Parameters
--------------------------------------------------------------------------------

CharType
The data type of a single character to be stored in the tstring. The Standard C++ Library provides
specializations of this template class, with the type definitions tstring for elements of type Tchar,
wstring, for wchar_t, u16string for char16_t, and u32string for char32_t.

Traits
Various important properties of the CharType elements in a basic_string specialization are described by
the class Traits. The default value is char_traits<CharType>.

Allocator
The type that represents the stored allocator object that encapsulates details about the tstring's
allocation and deallocation of memory. The default value is allocator<CharType>.


--------------------------------------------------------------------------------

Constructors
--------------------------------------------------------------------------------

basic_string
  Constructs a tstring that is empty or initialized by specific characters or that is a copy of all or
  part of some other tstring object or C-tstring.


Typedefs
--------------------------------------------------------------------------------

allocator_type
  A type that represents the allocator class for a tstring object.

const_iterator
  A type that provides a random-access iterator that can access and read a const element in the tstring.

const_pointer
  A type that provides a pointer to a const element in a tstring.

const_reference
  A type that provides a reference to a const element stored in a tstring for reading and performing const
  operations.

const_reverse_iterator
  A type that provides a random-access iterator that can read any const element in the tstring.

difference_type
  A type that provides the difference between two iterators that refer to elements within the same tstring.

iterator
  A type that provides a random-access iterator that can read or modify any element in a tstring.

npos
  An unsigned integral value initialized to –1 that indicates either "not found" or
  "all remaining characters" when a search function fails.

pointer
  A type that provides a pointer to a character element in a tstring or character array.

reference
  A type that provides a reference to an element stored in a tstring.

reverse_iterator
  A type that provides a random-access iterator that can read or modify an element in a reversed tstring.

size_type
  An unsigned integral type for the number of elements in a tstring.

traits_type
  A type for the character traits of the elements stored in a tstring.

value_type
  A type that represents the type of characters stored in a tstring.


Member Functions
--------------------------------------------------------------------------------

append
  Adds characters to the end of a tstring.

assign
  Assigns new character values to the contents of a tstring.

at
  Returns a reference to the element at a specified location in the tstring.

back

begin
  Returns an iterator addressing the first element in the tstring.

c_str
  Converts the contents of a tstring as a C-style, null-terminated, tstring.

capacity
  Returns the largest number of elements that could be stored in a tstring without increasing the memory
  allocation of the tstring.

cbegin
  Returns a const iterator addressing the first element in the tstring.

cend
  Returns a const iterator that addresses the location succeeding the last element in a tstring.

clear
  Erases all elements of a tstring.

compare
  Compares a tstring with a specified tstring to determine if the two strings are equal or if one is
  lexicographically less than the other.

copy
  Copies at most a specified number of characters from an indexed position in a source tstring to a
  target character array. Deprecated. Use basic_string::_Copy_s instead.

crbegin
  Returns a const iterator that addresses the first element in a reversed tstring.

crend
  Returns a const iterator that addresses the location succeeding the last element in a reversed tstring.

_Copy_s
  Copies at most a specified number of characters from an indexed position in a source tstring to a
  target character array.

data
  Converts the contents of a tstring into an array of characters.

empty
  Tests whether the tstring contains characters.

end
  Returns an iterator that addresses the location succeeding the last element in a tstring.

erase
  Removes an element or a range of elements in a tstring from a specified position.

find
  Searches a tstring in a forward direction for the first occurrence of a substring that matches a
  specified sequence of characters.

find_first_not_of
  Searches through a tstring for the first character that is not any element of a specified tstring.

find_first_of
  Searches through a tstring for the first character that matches any element of a specified tstring.

find_last_not_of
  Searches through a tstring for the last character that is not any element of a specified tstring.

find_last_of
  Searches through a tstring for the last character that is an element of a specified tstring.

front
  Returns a reference to the first element in a tstring.

get_allocator
  Returns a copy of the allocator object used to construct the tstring.

insert
  Inserts an element or a number of elements or a range of elements into the tstring at a specified
  position.

length
  Returns the current number of elements in a tstring.

max_size
  Returns the maximum number of characters a tstring could contain.

pop_back
  Erases the last element of the tstring.

push_back
  Adds an element to the end of the tstring.

rbegin
  Returns an iterator to the first element in a reversed tstring.

rend
  Returns an iterator that points just beyond the last element in a reversed tstring.

replace
  Replaces elements in a tstring at a specified position with specified characters or characters copied
  from other ranges or strings or C-strings.

reserve
  Sets the capacity of the tstring to a number at least as great as a specified number.

resize
  Specifies a new size for a tstring, appending or erasing elements as required.

rfind
  Searches a tstring in a backward direction for the first occurrence of a substring that matches a
  specified sequence of characters.

shrink_to_fit
  Discards the excess capacity of the tstring.

size
  Returns the current number of elements in a tstring.

substr
  Copies a substring of at most some number of characters from a tstring beginning from a specified
  position.

swap
  Exchange the contents of two strings.


Operators
--------------------------------------------------------------------------------

operator+=
  Appends characters to a tstring.

operator=
  Assigns new character values to the contents of a tstring.

operator[]
  Provides a reference to the character with a specified index in a tstring.


Remarks
--------------------------------------------------------------------------------

If a function is asked to generate a sequence longer than max_size elements, the function reports a
length error by throwing an object of type length_error.

References, pointers, and iterators that designate elements of the controlled sequence can become invalid
after any call to a function that alters the controlled sequence, or after the first call to a non- const
member function.

Requirements
--------------------------------------------------------------------------------

Header: <tstring>

Namespace: std

See Also
--------------------------------------------------------------------------------

Reference
<tstring>
Thread Safety in the C++ Standard Library
© 2022 Microsoft Corporation. All rights reserved.

Send Feedback on this topic to Microsoft.
*/
