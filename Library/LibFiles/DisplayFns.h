// Display Functions -- Interface into Doc/View class


#pragma once
#include "DataFile.h"



void        clrDsp();
void        printDsp(TCchar* fmt, ...);
void        crlfDsp();

void        putDsp( DataFile* cf, Tchar ch);
inline void crlfDsp(DataFile* cf) {putDsp(cf, '\n');}

void        invalidate();

      void   displayStg(TCchar* fmt, ...);
      void   displayStg(String& msg);
      void   displayStg(String& msg, int v);

inline void  displayMsg(TCchar* msg)        {String s = msg; displayStg(s);}
inline void  displayMsg(TCchar* msg, int v) {String s = msg; displayStg(s, v);}
      DWORD  dspError(  TCchar* fmt);
      DWORD  dspError(  TCchar* fmt, DWORD v);

