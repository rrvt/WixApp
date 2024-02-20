// CDoc -- Used to override Serialize so that Unicode may be used internally


#pragma once
#include "Archive.h"
#include "GetPathDlg.h"
#include <setupapi.h>




class CDoc : public CDocument {
protected:

String path;

public:

  virtual bool setOpenPath(PathDlgDsc& dsc)    {return getOpenDlg(   dsc, path);}
  virtual bool setIncOpenPath(PathDlgDsc& dsc) {return getIncOpenDlg(dsc, path);}

  virtual BOOL OnOpenDocument(LPCTSTR lpszPathName) override;

  virtual bool OnOpenIncDocument(LPCTSTR lpszPathName);

          bool reOpenDocument();              // Position to end of file

  virtual void OnOpenArb(void* arbObj);

  virtual bool setSaveAsPath(PathDlgDsc& dsc);          // Request approval to over write existing file
  virtual bool setIncSavePath(PathDlgDsc& dsc);         // Over write file always, use backup to save file
  virtual void backupFile(int noBackups);               // move current file to one with 12 digit ext

  virtual BOOL OnSaveDocument(LPCTSTR lpszPathName) override;
          bool onSaveDocument(LPCTSTR lpszPathName, bool savePath = false);

  virtual BOOL DoFileSave() override;

  virtual const CString& GetPathName();
  virtual TCchar* getPath() {return path;}

  virtual void SetPathName(LPCTSTR lpszPathName, BOOL bAddToMRU = TRUE)
                                         {path = lpszPathName; SetupAddToSourceList(SRCLIST_USER, path);}

  virtual void serialize(Archive& arcv) = 0;
  };


