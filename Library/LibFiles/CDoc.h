// CDoc -- Used to override Serialize so that Unicode may be used internally


#pragma once
#include "Archive.h"
#include <setupapi.h>



class CDoc : public CDocument {
String path;

public:

  virtual BOOL OnNewDocument() override {return CDocument::OnNewDocument();}

  virtual BOOL OnOpenDocument(LPCTSTR lpszPathName) override {

    path = lpszPathName;

    Archive ar(path, FileIO::Read);   if (!ar.isOpen()) return false;

    serialize(ar); return true;
    }

  virtual BOOL OnSaveDocument(LPCTSTR lpszPathName) override {

    String path = lpszPathName;

    Archive ar(path, FileIO::Write);   if (!ar.isOpen()) return false;

    serialize(ar); return true;
    }

  virtual BOOL DoFileSave() override {

    if (path.isEmpty()) return false;

    Archive ar(path, FileIO::Write);   if (!ar.isOpen()) return false;

    serialize(ar); return true;
    }

  virtual const CString& GetPathName() {static CString cs = path; return cs;}

  virtual void SetPathName(LPCTSTR lpszPathName, BOOL bAddToMRU = TRUE)
    {path = lpszPathName; SetupAddToSourceList(SRCLIST_USER, path);}

  virtual void serialize(Archive& arcv) = 0;
  };

