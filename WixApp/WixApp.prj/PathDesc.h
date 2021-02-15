// Path Descriptor -- Stores a path relative to path to solution, however writes and reads full path
//                    from WixData file.


#pragma once


class PathDesc {

String relSol;                            // Path relative to Solution path $(SolutionDir)

public:

  PathDesc() { }
  PathDesc(PathDesc& p) {relSol = p.relSol;}
 ~PathDesc() { }

 void    clear() {relSol.clear();}

  void readWixData(TCchar* section, TCchar* key);
  void writeWixData(TCchar* section, TCchar* key);

  PathDesc& operator= (PathDesc& p) {relSol = p.relSol; return *this;}
  PathDesc& operator= (String& fullPath) {relativeSolution(fullPath); return *this;}

  bool      isEmpty()  {return relSol.isEmpty();}
  operator  TCchar*()  {return relSol;}
  String&   browse(TCchar* title, TCchar* ext, TCchar* pat);
  String    path();
  String    relative() {return relSol.isEmpty() ? _T("") : _T("$(var.SolutionDir)") + relSol;}

private:

  void      relativeSolution(const String& fullPath);
  };

