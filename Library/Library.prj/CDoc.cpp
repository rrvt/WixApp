// CDoc -- Used to override Serialize so that Unicode may be used internally


#include "stdafx.h"
#include "CDoc.h"


//bool CDoc::setPath(PathDlgDsc& dsc) {return getPathDlg(dsc, path);}


BOOL CDoc::OnOpenDocument(LPCTSTR lpszPathName) {

  if (!lpszPathName) return false;

  path = lpszPathName;

  Archive ar(path, FileIO::Read);   if (!ar.isOpen()) return false;

  serialize(ar); return true;
  }


bool CDoc::OnOpenIncDocument(LPCTSTR lpszPathName) {

  if (!lpszPathName) return false;

  path = lpszPathName;

  Archive ar(path, FileIO::Read | FileIO::Write);   if (!ar.isOpen()) return false;

  serialize(ar); return true;
  }


// Position to end of file

bool CDoc::reOpenDocument() {

  if (path.isEmpty()) return false;

  Archive ar(path, (FileIO::OpenParms) (FileIO::Write | FileIO::Read));   if (!ar.isOpen()) return false;

  ar.seekEnd();   serialize(ar);   return true;
  }


void CDoc::OnOpenArb(void* arbObj) {Archive ar(arbObj, FileIO::Read);   serialize(ar);}


bool CDoc::setSaveAsPath(PathDlgDsc& dsc)
                            {return getSaveAsPathDlg(dsc.title, dsc.name, dsc.ext, dsc.pattern, path);}

bool CDoc::setIncSavePath(PathDlgDsc& dsc)
                            {return getIncSavePathDlg(dsc.title, dsc.name, dsc.ext, dsc.pattern, path);}



BOOL CDoc::OnSaveDocument(LPCTSTR lpszPathName) {
String  path = lpszPathName;
Archive ar(path, FileIO::Write);   if (!ar.isOpen()) return false;

  serialize(ar); return true;
  }


bool CDoc::onSaveDocument(LPCTSTR lpszPathName, bool savePath) {
String  pth = lpszPathName;   if (savePath) path = pth;
Archive ar(pth, FileIO::Write);   if (!ar.isOpen()) return false;

  serialize(ar); return true;
  }


BOOL CDoc::DoFileSave() {

  if (path.isEmpty()) return false;

  Archive ar(path, FileIO::Write);   if (!ar.isOpen()) return false;

  serialize(ar); return true;
  }


const CString& CDoc::GetPathName() {static CString cs; cs = path; return cs;}

