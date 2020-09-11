// CDoc -- Used to override Serialize so that Unicode may be used internally


#pragma once
#include "Archive.h"
#include <setupapi.h>



class CDoc : public CDocument {
String path;

public:

  virtual BOOL OnNewDocument() override {path.clear(); return true;}

  virtual BOOL OnOpenDocument(LPCTSTR lpszPathName) override;

  virtual bool OnOpenIncDocument(LPCTSTR lpszPathName);

          bool reOpenDocument();              // Position to end of file

  virtual BOOL OnSaveDocument(LPCTSTR lpszPathName) override;
          bool onSaveDocument(LPCTSTR lpszPathName, bool savePath = false);

  virtual BOOL DoFileSave() override;

  virtual const CString& GetPathName();


  virtual void SetPathName(LPCTSTR lpszPathName, BOOL bAddToMRU = TRUE)
    {path = lpszPathName; SetupAddToSourceList(SRCLIST_USER, path);}

  virtual void serialize(Archive& arcv) = 0;
  };

