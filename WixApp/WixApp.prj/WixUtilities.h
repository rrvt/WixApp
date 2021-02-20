// Utility Functions


#pragma once
#include "WixOut.h"


String removeBlnks(String& s);

String cleanName(String& s);

String getWixID(String& id, TCchar* ext);

void   findLastName(String& path, String& parent, String& name);

String combineNames(String& parent, String& name);

inline String getText(CEdit& ctrl) {CString cs; ctrl.GetWindowText(cs); return cs;}


String findExeFile(TCchar* name);

String getModulePath();

#if 0
void outputLine(int tab, TCchar* prefix, String& s, TCchar* suffix, FileIO& file);
void outputStg(int tab, String& s, FileIO& file);
inline void outputLit(int tab, TCchar*  p, FileIO& file) {String s = p; outputStg(tab, s, file);}
int  outCount(String& line);              // outCounts '\"' as one character
#endif

