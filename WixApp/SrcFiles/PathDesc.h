// Path Descriptor -- full path and short version for display


#pragma once


class PathDesc {

String dsc;

public:

  PathDesc() {}
  PathDesc(PathDesc& p) {copyObj(p);}
 ~PathDesc() { }

  PathDesc& operator= (PathDesc& p) {copyObj(p); return *this;}
  PathDesc& operator= (const String& fullPath);                   // fullPath is Path + filename
            operator String() {return dsc;}
            operator TCchar*() {return dsc;}

  String  full();
  String  relative() {return dsc.empty() ? _T("") : _T("$(var.SolutionDir)") + dsc;}

  void    clear() {dsc.clear();}
  bool    empty() {return dsc.empty();}

private:

  void    copyObj(PathDesc& p) {dsc = p.dsc;}
  };

