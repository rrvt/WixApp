// Path Descriptor -- Stores a path relative to path to solution, however writes and reads full path
//                    from WixData file.


#pragma once
#include "GetPathDlg.h"


class BrowseDsc : public PathDlgDsc {
public:

String dfltKey;

  BrowseDsc() { }
  BrowseDsc(TCchar* key, TCchar* ttl, TCchar* nm, TCchar* e, TCchar* pat);
  BrowseDsc(BrowseDsc& dsc) {copy(dsc);}
 ~BrowseDsc() { }

  BrowseDsc& operator= (BrowseDsc& dsc) {copy(dsc); return *this;}

private:

  void copy(BrowseDsc& dsc) {PathDlgDsc::copy(dsc); dfltKey = dsc.dfltKey;}
  };


class PathDesc {

String relSol;                            // Path relative to Solution path $(SolutionDir)

public:

  PathDesc() { }
  PathDesc(PathDesc& p) {relSol = p.relSol;}
 ~PathDesc() { }

 void       clear() {relSol.clear();}

  void      readWixData(BrowseDsc& dsc, TCchar* section, TCchar* key);
  void      writeWixData(TCchar* section, TCchar* key);

  PathDesc& operator= (PathDesc& p) {relSol = p.relSol; return *this;}
  PathDesc& operator= (String& fullPath) {relativeSolution(fullPath); return *this;}
  PathDesc& operator= (TCchar* fullPath) {String s = fullPath; relativeSolution(s); return *this;}

  bool      isEmpty()  {return relSol.isEmpty();}
  operator  TCchar*()  {return relSol;}
  String&   browse(BrowseDsc& dsc);
  String    path();
  String    relative() {return relSol.isEmpty() ? String(_T("")) : _T("$(var.SolutionDir)") + relSol;}

private:

  void      relativeSolution(const String& fullPath);
  };
