// Path Descriptor -- Stores a path relative to path to solution, however writes and reads full
//                    path from WixData file.


#pragma once
#include "Archive.h"
#include "FileName.h"
#include "GetPathDlg.h"
#include "PathDlgDsc.h"



class KeyedPathDsc : public PathDlgDsc {
public:

String dfltKey;

  KeyedPathDsc(TCchar* key, TCchar* ttl, TCchar* nm, TCchar* e, TCchar* pat);
  KeyedPathDsc(KeyedPathDsc& dsc) {copy(dsc);}
 ~KeyedPathDsc() { }

  KeyedPathDsc& operator= (KeyedPathDsc& dsc) {copy(dsc); return *this;}

private:

  void copy(KeyedPathDsc& dsc) {PathDlgDsc::copy(dsc); dfltKey = dsc.dfltKey;}

  KeyedPathDsc() { }
  };


class RelPath {

String relPath;                            // Path relative to Solution path $(SolutionDir)

public:

  RelPath() { }
  RelPath(RelPath& p) {relPath = p.relPath;}
 ~RelPath() { }

 void       clear() {relPath.clear();}

  void      readWixData( TCchar* section, TCchar* key, KeyedPathDsc& dsc);
  void      writeWixData(TCchar* section, TCchar* key);

  RelPath& operator= (RelPath& p) {relPath = p.relPath; return *this;}
  RelPath& operator= (TCchar* fullPath) {String s = fullPath; setRelPath(s); return *this;}

  bool      isEmpty()  {return relPath.isEmpty();}
  operator  TCchar*()  {return relPath;}

  String&   getPath(   KeyedPathDsc& dsc);      // Get path of existing file
  String    getFileName() {return removePath(relPath);}

  String    localPath();          // Local full path -- Prefix with local root directory
  String    prodPath();           // Product full path -- Prefix relative path with root directory

  void      saveData(Archive& ar) {ar << relPath;}

private:

  void      setRelPath(const String& fullPath);
  };

