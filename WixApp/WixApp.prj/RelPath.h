// Path Descriptor -- Stores a path relative to path to solution, however writes and reads full
//                    path from WixData file.


#pragma once
#include "Archive.h"
#include "FileName.h"
//#include "GetPathDlg.h"
//#include "PathDlgDsc.h"

class KeyedPathDlg;



class RelPath {

String relPath;                            // Path relative to Solution path $(SolutionDir)

public:

  RelPath() { }
  RelPath(RelPath& p) {relPath = p.relPath;}
 ~RelPath() { }

 void       clear() {relPath.clear();}

  void      writeWixData(TCchar* section, TCchar* key);

  void      readWxdPath( TCchar* section, TCchar* key, KeyedPathDlg& dlg);
  void      readWxdData( TCchar* section, TCchar* key, KeyedPathDlg& dlg);
  void      writeWxdData(TCchar* section, TCchar* key);

  RelPath& operator= (RelPath& p) {relPath = p.relPath; return *this;}
  RelPath& operator= (TCchar* fullPath) {String s = fullPath; setRelPath(s); return *this;}

  bool      isEmpty()  {return relPath.isEmpty();}
  bool      isRelPath(String& path) {return path[0] == _T('.') || path.find(_T(':')) < 0;
  }
  operator  TCchar*()  {return relPath;}

  String&   getPath(KeyedPathDlg& pathDlg);      // Get path of existing file
  String    getFileName() {return removePath(relPath);}

  String    localPath();          // Local full path -- Prefix with local root directory
  String    prodPath();           // Product full path -- Prefix relative path with root directory

  void      saveData(Archive& ar) {ar << relPath;}

private:

  String&   getPath(    TCchar* path, KeyedPathDlg& dlg);
  String&   getPathOnly(TCchar* path, KeyedPathDlg& dlg);
  void      setRelPath(const String& fullPath);
  String&   getRelPath(String& fullPath);
  };

