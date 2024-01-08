// Search File


#include "pch.h"
#include "SrchFile.h"
#include "RegExpr.h"



SrchFile::SrchFile(TCchar* path) {
  opened = Open(path, CFile::modeRead, &err);
  }


// lines have form:  Key=val

bool SrchFile::forValOf(TCchar* key, CString& val) {
String pat = _T('^');   pat += key;   pat += _T('=');

  if (!opened) return false;

  if (!forPat(pat)) return false;

  String s = line;   val = s.substr(s.find(_T('=')) + 1);   return true;
  }


bool SrchFile::forPat(TCchar* pat) {
RegExpr re(pat);

  if (!opened) return false;

  while (ReadString(line)) {
    if (re.match(line)) return true;
    }

  return false;
  }

