// CDoc -- Used to override Serialize so that Unicode may be used internally


#pragma once
#include "Archive.h"
#include <setupapi.h>


struct PathDesc {
String title;         // Save As Parameters, examples:
String name;          // _T("mumble.txt")
String ext;           // _T("txt")
String pattern;       // _T("*.txt")
};


class CDoc : public CDocument {
protected:

String path;                                     // Path to file being processed, strictly temporary

public:

  virtual bool setPath(PathDesc& dsc);

  virtual BOOL OnNewDocument() override {path.clear(); return true;}

  virtual BOOL OnOpenDocument(LPCTSTR lpszPathName) override;

  virtual bool OnOpenIncDocument(LPCTSTR lpszPathName);

          bool reOpenDocument();              // Position to end of file

  virtual void OnOpenArb(void* arbObj);

  virtual bool setSaveAsPath(PathDesc& dsc);
  virtual bool setIncSavePath(PathDesc& dsc);

  virtual BOOL OnSaveDocument(LPCTSTR lpszPathName) override;
          bool onSaveDocument(LPCTSTR lpszPathName, bool savePath = false);

  virtual BOOL DoFileSave() override;

  virtual const CString& GetPathName();


  virtual void SetPathName(LPCTSTR lpszPathName, BOOL bAddToMRU = TRUE)
                                         {path = lpszPathName; SetupAddToSourceList(SRCLIST_USER, path);}

  virtual void serialize(Archive& arcv) = 0;
  };

