// Uses a Windows dialog to obtain a path to a file


#include "stdafx.h"
#include "GetPathDlg.h"


static bool saveDlg(TCchar* title, TCchar* iniFileName, TCchar* defExt, TCchar* extPat, bool overwrt,
                                                                                            String& path);


static bool dlgPath(CFileDialog* dlg, TCchar* title, String& path);

// Start a dialog box to obtain the path to a file
//   title       - the name of the file type (e.g. "Text)"
//   inifileName - a file name that will appear in the edit box of the dialog, set to 0 if not needed
//   defExt      - the default extension which is appended if not extension provided by user, 0 if not
//                 needed
//   extPat      - one or more wild card filter of the extensions requested (e.g. "*.txt;*.cpp")
//                 The All Files filter is appended to the list as another alternative

bool getPathDlg(TCchar* title, TCchar* iniFileName, TCchar* defExt, TCchar* extPat, String& path) {
String e = title;   e += _T('|');    e += extPat;    e += _T("|All Files|*.*||");

CFileDialog fileDialog(true, defExt, iniFileName, OFN_FILEMUSTEXIST, e, 0);

  fileDialog.m_ofn.lpstrTitle = title;

  if (fileDialog.DoModal() == IDOK) {path = fileDialog.GetPathName(); return true;}

  return false;
  }


// Overwrites existing file

bool getSaveAsPathDlg(TCchar* title, TCchar* iniFileName, TCchar* defExt, TCchar* extPat, String& path) {
  return saveDlg(title, iniFileName, defExt, extPat, true, path);

#if 0
String        e = title;   e += _T('|');    e += extPat;    e += _T("|All Files (*.*)|*.*||");
CFileDialog   fileDialog(false, defExt, iniFileName, OFN_OVERWRITEPROMPT, e, 0);
OPENFILENAME& ofn = fileDialog.m_ofn;

  fileDialog.m_ofn.lpstrTitle = title;

  ofn.lpstrInitialDir = _T("xyz");

  if (fileDialog.DoModal() == IDOK) {path = fileDialog.GetPathName(); return true;}

  return false;
#endif
  }


// Allows adding to existing file

bool getSaveIncPathDlg(TCchar* title, TCchar* iniFileName, TCchar* defExt, TCchar* extPat, String& path)
                                        {return saveDlg(title, iniFileName, defExt, extPat, false, path);}


bool saveDlg(TCchar* title, TCchar* iniFileName, TCchar* defExt, TCchar* extPat, bool overwrt,
                                                                                          String& path) {
String        e = title;   e += _T('|');    e += extPat;    e += _T("|All Files (*.*)|*.*||");
DWORD         flags = overwrt ? OFN_OVERWRITEPROMPT : 0;
CFileDialog   fileDialog(false, defExt, iniFileName, flags, e, 0);
OPENFILENAME& ofn = fileDialog.m_ofn;

  fileDialog.m_ofn.lpstrTitle = title;

  ofn.lpstrInitialDir = _T("xyz");

  if (fileDialog.DoModal() == IDOK) {path = fileDialog.GetPathName(); return true;}

  return false;
  }




bool getDirPathDlg(TCchar* title, String& path) {
CFolderPickerDialog fileDialog(path);

  return dlgPath(&fileDialog, title, path);

  fileDialog.m_ofn.lpstrTitle = title;

  if (fileDialog.DoModal() == IDOK) {path = fileDialog.GetPathName(); return true;}

  return false;
  }



bool dlgPath(CFileDialog* dlg, TCchar* title, String& path) {
  if (!dlg) return false;

  dlg->m_ofn.lpstrTitle = title;

  if (dlg->DoModal() == IDOK) {path = dlg->GetPathName(); return true;}

  return false;

  }

