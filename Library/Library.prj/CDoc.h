// CDoc -- Used to override Serialize so that Unicode may be used internally


#pragma once
#include "Archive.h"
#include "GetPathDlg.h"
#include <setupapi.h>




class CDoc : public CDocument {
protected:

String path;                                     // Path to file being processed, strictly temporary

public:

  virtual bool setPath(PathDlgDsc& dsc) {return getPathDlg(dsc, path);}

  virtual BOOL OnOpenDocument(LPCTSTR lpszPathName) override;

  virtual bool OnOpenIncDocument(LPCTSTR lpszPathName);

          bool reOpenDocument();              // Position to end of file

  virtual void OnOpenArb(void* arbObj);

  virtual bool setSaveAsPath(PathDlgDsc& dsc);
  virtual bool setIncSavePath(PathDlgDsc& dsc);

  virtual BOOL OnSaveDocument(LPCTSTR lpszPathName) override;
          bool onSaveDocument(LPCTSTR lpszPathName, bool savePath = false);

  virtual BOOL DoFileSave() override;

  virtual const CString& GetPathName();
  virtual TCchar* getPath() {return path;}

  virtual void SetPathName(LPCTSTR lpszPathName, BOOL bAddToMRU = TRUE)
                                         {path = lpszPathName; SetupAddToSourceList(SRCLIST_USER, path);}

  virtual void serialize(Archive& arcv) = 0;
  };


