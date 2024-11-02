// Utilities --


#pragma once
#include "IniFile.h"
#include "Resource.h"
#include "SendMsg.h"



String  toRemotePath(TCchar* path);     // Replace '\' with '/'
String  toLocalPath(TCchar* path);      // Replace '/' with '\'
String  parseName(TCchar* path);        // Finds the name after the last '\'
String& fixLocalPath( String& path);
String& fixRemotePath(String& path);

String  removeSpaces(TCchar* name);     // Remove Spaces

String  ensureSite(TCchar* name);

int     pathLevel(TCchar* path);

inline void sendDisplayMsg()                  {sendMsg(ID_DisplayMsg,    0,      0);}
inline void sendStepPrgBar()                  {sendMsg(ID_StepPrgBarMsg, 0,      0);}
inline void sendWdwScroll(bool scroll = true) {sendMsg(ID_SetWdwScroll,  scroll, 0);}

void    dspLines(String& s);


class Baffle : protected IniFile {
String es;
public:

  Baffle() { }
 ~Baffle() { }

  void clear() {es.clear();}

  String& operator<< (String& s)   {return es = encodePassword(s);}
  String& operator<< (CString& cs) {String s = cs; return *this << s;}
  String  operator>> (String& s)   {return s  = decodePassword(es);}
  String  operator>> (CString& cs) {String s; *this >> s; cs = s; return s;}
  String  operator() ()            {return es;}

  Baffle& operator=  (String& s)   {es = s;    return *this;}
  Baffle& operator=  (Baffle& b)   {es = b.es; return *this;}

  bool    isEmpty() {return es.isEmpty();}
  };



