// Uses a Windows dialog to obtain a path to a file


#include "stdafx.h"
#include "GetPathDlg.h"


// Local function

static bool saveDlg(TCchar* title, TCchar* iniFileName, TCchar* defExt, TCchar* extPat, bool overwrt,
                                                                                            String& path);


PathDlgDsc::PathDlgDsc(TCchar* ttl, TCchar* nm, TCchar* e, TCchar* pat)
            {title = ttl; name = nm ? nm : _T(""); ext = e; pattern = pat;}


void PathDlgDsc::copy(PathDlgDsc& dsc)
                          {title = dsc.title;  name = dsc.name;   ext = dsc.ext;  pattern = dsc.pattern;}


static bool dlgPath(CFileDialog* dlg, TCchar* title, String& path);

// Start a dialog box to obtain the path to a file
//   title       - the name of the file type (e.g. "Text)"
//   inifileName - a file name that will appear in the edit box of the dialog, set to 0 if not needed
//   defExt      - the default extension which is appended if not extension provided by user, 0 if not
//                 needed
//   extPat      - one or more wild card filter of the extensions requested (e.g. "*.txt;*.cpp")
//                 The All Files filter is appended to the list as another alternative




bool getPathDlg(PathDlgDsc& dsc, String& path)
                                    {return getPathDlg(dsc.title, dsc.name, dsc.ext, dsc.pattern, path);}



bool getPathDlg(TCchar* title, TCchar* iniFileName, TCchar* defExt, TCchar* extPat, String& path) {
String e = title;   e += _T('|');    e += extPat;    e += _T("|All Files|*.*||");

CFileDialog fileDialog(true, defExt, iniFileName, OFN_FILEMUSTEXIST, e, 0);

  fileDialog.m_ofn.lpstrTitle = title;

  if (fileDialog.DoModal() == IDOK) {path = fileDialog.GetPathName(); return true;}

  return false;
  }


// Overwrites existing file

bool getSaveAsPathDlg(PathDlgDsc& dsc, String& path)
                              {return getSaveAsPathDlg(dsc.title, dsc.name, dsc.ext, dsc.pattern, path);}


bool getSaveAsPathDlg(TCchar* title, TCchar* iniFileName, TCchar* defExt, TCchar* extPat, String& path)
                                        {return saveDlg(title, iniFileName, defExt, extPat, true, path);}


// Allows adding to existing file

bool getIncSavePathDlg(TCchar* title, TCchar* iniFileName, TCchar* defExt, TCchar* extPat, String& path)
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
  if (!path.isEmpty()) dlg->m_ofn.lpstrInitialDir = path;

  if (dlg->DoModal() == IDOK) {path = dlg->GetPathName(); return true;}

  return false;

  }

