// Uses a Windows dialog to obtain a path to a file


#include "pch.h"
#include "GetPathDlg.h"
#include "FileName.h"


// Local functions

static bool saveDlg(PathDlgDsc& dsc, bool overwrt, String& path);
static bool openDlg(PathDlgDsc& dsc, bool overwrt, String& path);
static bool pathDlg(PathDlgDsc& dsc, bool openDlg, DWORD flags, String& path);


bool getOpenDlg(   PathDlgDsc& dsc, String& path) {return openDlg(dsc, false, path);}
bool getIncOpenDlg(PathDlgDsc& dsc, String& path) {return openDlg(dsc, true,  path);}

// Overwrites existing file

bool getSaveAsPathDlg(PathDlgDsc& dsc, String& path) {return saveDlg(dsc, true, path);}


// Allows adding to existing file

bool getIncSavePathDlg(PathDlgDsc& dsc, String& path) {return saveDlg(dsc, false, path);}



bool openDlg(PathDlgDsc& dsc, bool overwrt, String& path)
                                    {return pathDlg(dsc, true, overwrt ? OFN_OVERWRITEPROMPT : 0, path);}
bool saveDlg(PathDlgDsc& dsc, bool overwrt, String& path)
                                   {return pathDlg(dsc, false, overwrt ? OFN_OVERWRITEPROMPT : 0, path);}


static bool pathDlg(PathDlgDsc& dsc, bool openDlg, DWORD flags, String& path) {
String        e = dsc.title + _T('|') + dsc.pattern + _T("|All Files (*.*)|*.*||");
CFileDialog   fileDialog(openDlg, dsc.ext, dsc.name, flags, e, 0);
OPENFILENAME& ofn = fileDialog.m_ofn;

  ofn.lpstrTitle = dsc.title;   ofn.lpstrInitialDir = dsc.getPath();

  if (fileDialog.DoModal() == IDOK) {path = fileDialog.GetPathName(); return true;}

  return false;
  }


bool getDirPathDlg(TCchar* title, String& path) {
CFolderPickerDialog dlg(path);

  dlg.m_ofn.lpstrTitle = title;

  if (!path.isEmpty()) dlg.m_ofn.lpstrInitialDir = path;

  if (dlg.DoModal() == IDOK) {path = dlg.GetPathName(); return true;}

  return false;
  }


PathDlgDsc::PathDlgDsc(TCchar* ttl, TCchar* nm, TCchar* e, TCchar* pat) {
  title   = ttl;
  name    = nm ? nm : _T("");
  ext     = e;
  pattern = pat;
  }


TCchar* PathDlgDsc::getPath() {path = ::getPath(name);  return path;}


void PathDlgDsc::copy(PathDlgDsc& dsc)
                           {title = dsc.title;  name = dsc.name;   ext = dsc.ext;  pattern = dsc.pattern;}



//static bool dlgPath(CFileDialog* dlg, TCchar* title, String& path);

// Start a dialog box to obtain the path to a file
//   title       - the name of the file type (e.g. "Text)"
//   inifileName - a file name that will appear in the edit box of the dialog, set to 0 if not needed
//   defExt      - the default extension which is appended if not extension provided by user, 0 if not
//                 needed
//   extPat      - one or more wild card filter of the extensions requested (e.g. "*.txt;*.cpp")
//                 The All Files filter is appended to the list as another alternative



#if 0
bool getPathDlg(PathDlgDsc& dsc, String& path)
                                    {return getPathDlg(dsc.title, dsc.name, dsc.ext, dsc.pattern, path);}



bool getPathDlg(TCchar* title, TCchar* iniFileName, TCchar* defExt, TCchar* extPat, String& path) {

String e = title;   e += _T('|');    e += extPat;    e += _T("|All Files|*.*||");

CFileDialog fileDialog(true, defExt, iniFileName, OFN_FILEMUSTEXIST, e, 0);

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



  return dlgPath(&fileDialog, title, path);
#else
#endif
#if 1
#else
String        e = dsc.title;   e += _T('|');    e += dsc.pattern;    e += _T("|All Files (*.*)|*.*||");
DWORD         flags = overwrt ? OFN_OVERWRITEPROMPT : 0;
CFileDialog   fileDialog(false, dsc.ext, dsc.name, flags, e, 0);
OPENFILENAME& ofn = fileDialog.m_ofn;

  fileDialog.m_ofn.lpstrTitle = dsc.title;

  ofn.lpstrInitialDir = _T("xyz");

  if (fileDialog.DoModal() == IDOK) {path = fileDialog.GetPathName(); return true;}

  return false;
#endif
#if 1
#else
String        e = dsc.title;   e += _T('|');    e += dsc.pattern;    e += _T("|All Files (*.*)|*.*||");
DWORD         flags = overwrt ? OFN_OVERWRITEPROMPT : 0;
CFileDialog   fileDialog(true, dsc.ext, dsc.name, flags, e, 0);
OPENFILENAME& ofn = fileDialog.m_ofn;

  fileDialog.m_ofn.lpstrTitle = dsc.title;

  ofn.lpstrInitialDir = _T("xyz");

  if (fileDialog.DoModal() == IDOK) {path = fileDialog.GetPathName(); return true;}

  return false;
#endif
//DWORD flags = overwrt ? OFN_OVERWRITEPROMPT : 0;
//DWORD flags = overwrt ? OFN_OVERWRITEPROMPT : 0;

//TCchar* title, TCchar* iniFileName, TCchar* defExt, TCchar* extPat

