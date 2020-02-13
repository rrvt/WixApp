// File IO using the CArchive mechanism
// Copyright Software Design & Engineering, Robert R. Van Tuyl, 2013.  All rights reserved.


#pragma once
#include "Date.h"
#include "Currency.h"


class ArchiveIO {
CArchive& ar;
bool      accepted;
String    r;
int       no;
String    s;

public:

  ArchiveIO(CArchive& archive) : ar(archive) {accepted = true; no = 0;}
 ~ArchiveIO() {}

  ArchiveIO& operator<< (TCchar* s)   {Cstring t = s; ar << t;    return *this;}

  ArchiveIO& operator<< (String& s) {      Cstring t = s; ar << t;    return *this;}
  ArchiveIO& operator<< (int     t) {ar << t;                         return *this;}
  ArchiveIO& operator<< (long    t) {ar << t;                         return *this;}
  ArchiveIO& operator<< (bool    t) {ar << t;                         return *this;}
  ArchiveIO& operator<< (Byte    t) {ar << t;                         return *this;}
  ArchiveIO& operator<< (double  t) {ar << t;                         return *this;}
  ArchiveIO& operator<< (float   t) {ar << t;                         return *this;}

  ArchiveIO& operator>> (String& s) {      Cstring t; ar >> t; s = t; return *this;}
  ArchiveIO& operator>> (int&    t) {ar >> t;                         return *this;}
  ArchiveIO& operator>> (long&   t) {ar >> t;                         return *this;}
  ArchiveIO& operator>> (bool&   t) {ar >> t;                         return *this;}
  ArchiveIO& operator>> (Byte&   t) {ar >> t;                         return *this;}
  ArchiveIO& operator>> (double& t) {ar >> t;                         return *this;}
  ArchiveIO& operator>> (float&  t) {ar >> t;                         return *this;}

  friend ArchiveIO& operator<< (ArchiveIO& ario, Date& d);
  friend ArchiveIO& operator>> (ArchiveIO& ario, Date& d);
  friend ArchiveIO& operator<< (ArchiveIO& ario, CTimeSpan& d);
  friend ArchiveIO& operator>> (ArchiveIO& ario, CTimeSpan& d);
  friend ArchiveIO& operator<< (ArchiveIO& ario, Currency& c);
  friend ArchiveIO& operator>> (ArchiveIO& ario, Currency& c);

  void writeHdr(TCchar* name, int  n) {*this << name << n << "[]";}
  bool readHdr( TCchar* name, int& n) {

    if (accepted) {*this >> r >> n >> s;  no = n; accepted = false;}

bool x = r == name;
bool y = s == "[]";

    return r == name && s == "[]";
    }

  void accept() {accepted = true;}
  };


inline ArchiveIO& operator<< (ArchiveIO& ario, Date&     d) {/*ario.ar << d.v;*/   return ario;}  // ** Fix
inline ArchiveIO& operator>> (ArchiveIO& ario, Date&     d) {/*ario.ar >> d.v;*/   return ario;}
inline ArchiveIO& operator<< (ArchiveIO& ario, CTimeSpan& d) {return ario;}
inline ArchiveIO& operator>> (ArchiveIO& ario, CTimeSpan& d) {return ario;}

inline ArchiveIO& operator<< (ArchiveIO& ario, Currency& c) {ario.ar << c.cur; return ario;}
inline ArchiveIO& operator>> (ArchiveIO& ario, Currency& c) {ario.ar >> c.cur; return ario;}

