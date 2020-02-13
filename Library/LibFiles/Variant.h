// Variant additions


#pragma once


//inline long toLong(variant_t& v) {return v.vt == VT_NULL ? 0 : v.lVal;}

inline void notNull(variant_t& v) {if (v.vt <= VT_NULL) v = long(0);}


class Variant : public variant_t {
public:

  Variant() : variant_t() {}
  Variant(const String& s) : variant_t(s.str()) {}
 ~Variant() {}

  Variant& operator= (const String& s) {*this = s.str(); return *this;}

  operator long() {return vt == VT_I4 ? lVal : 0;}
  };


