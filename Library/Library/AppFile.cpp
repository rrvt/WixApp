// AppFile Object -- find and and/or execute an application


#include "pch.h"
#include "AppFile.h"
#include "FileName.h"
#include "FileSrch.h"

//#include "MessageBox.h"
//#include "NotePad.h"


void AppFile::loadFolders(TCchar* startPath) {
String   initialPath = startPath;
int      lng         = initialPath.length();
String   srchPat;
FileSrch fileSrch;
String   s;

  if (initialPath[lng-1] == _T('\\')) initialPath = initialPath.substr(0, lng-1);

  srchPat = removePath(initialPath);   initialPath = getPath(initialPath);

  fileSrch.findSubDirs(initialPath, srchPat);

  while (fileSrch.getName(s)) {data += s;   s += _T("*");   loadFolders(s);}
  }


// Find app in "c:\Program Files "

String& AppFile::find(TCchar* regExpr) {
AppIter  iter(*this);
String*  path;
FileSrch fileSrch;

  exePath.clear();

  for (path = iter(); path; path = iter++)
     {fileSrch.findFiles(path->str(), regExpr);   if (fileSrch.getName(exePath)) {return exePath;}}

  return exePath;
  }

