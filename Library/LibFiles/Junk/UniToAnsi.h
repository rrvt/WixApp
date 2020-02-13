// Unicode to Ansi Object

#pragma once


class ToAnsi {
char* p;

public:

int   cnt;

  ToAnsi(Tchar* s)  : p(0), cnt(0) {convert(s);}
  ToAnsi(String& s) : p(0), cnt(0) {convert(s.str());}

 ~ToAnsi() {if (p) delete[] p;}

  Cchar* operator() () {return p;}

private:

  ToAnsi() : p(0), cnt(0) {}

  void convert(TCchar* tp);
  };


template <int n> class ToUni {
CArchive& ar;
Tchar*    pBuf;
Tchar*    eBuf;
Tchar     ch;
Tchar     buf[n];
int       offset;

public:

  ToUni(CArchive& archive) : ar(archive), pBuf(buf), eBuf(buf), offset(0) {}                  //
 ~ToUni() {}

  Tchar get();
  void  next() {pBuf++; offset = ch == _T('\n') || ch == _T('\r') ? 0 : offset + 1;}

private:

  int   read();
        ToUni() {}
  };


template<int n> Tchar ToUni<n>::get() {

  if (pBuf >= eBuf) {

    if (!read()) {

      if (offset > 0) *eBuf++ = _T('\n');

      *eBuf++ = 26;
      }
    }

  ch = *pBuf;   return ch;
  }




template<int n> int ToUni<n>::read() {
int  cnt;

  pBuf = buf;

#ifdef _UNICODE
char pch[n];

  cnt = ar.Read(pch, n);    if (!cnt) {eBuf = pBuf; return 0;}

  cnt = MultiByteToWideChar(CP_UTF8, 0, pch, cnt, buf, n*2);

#else
  cnt = ar.Read(buf, n);
#endif

  eBuf = pBuf + cnt;   return cnt;
  }

