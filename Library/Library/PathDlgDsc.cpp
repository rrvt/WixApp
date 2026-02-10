// Create a descriptor for the FilePathDlg (aka CFileDialog) class


#include "pch.h"
#include "PathDlgDsc.h"
#include "FileName.h"


PathDlgDsc::PathDlgDsc(TCchar* ttl, TCchar* nm, TCchar* e, TCchar* pat) {
  title   = ttl;
  name    = nm ? nm : _T("");
  ext     = e;
  pattern = pat;
  }


TCchar* PathDlgDsc::getPath() {path = ::getPath(name);  return path;}


void PathDlgDsc::copy(PathDlgDsc& dsc)
                    {title = dsc.title;  name = dsc.name;   ext = dsc.ext;  pattern = dsc.pattern;}

