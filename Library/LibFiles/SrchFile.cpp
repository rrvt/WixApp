// Search File


#include "stdafx.h"
#include "SrchFile.h"
#include "match.h"



SrchFile::SrchFile(TCchar* path) {
  opened = Open(path, CFile::modeRead, &err);
  }


// lines have form:  Key=val

bool SrchFile::forValOf(TCchar* key, CString& val) {
String pat = '^';   pat += key;   pat += '=';

  if (!opened) return false;

  if (!forPat(pat)) return false;

  String s = line;   val = s.substr(s.find('=') + 1);   return true;
  }


bool SrchFile::forPat(TCchar* pat) {
RegExpr re(pat);

  if (!opened) return false;

  while (ReadString(line)) {
    if (re.match(line)) return true;
    }

  return false;
  }

