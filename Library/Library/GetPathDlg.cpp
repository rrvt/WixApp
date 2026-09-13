// Uses a Windows dialog to obtain a path to a file


#include "pch.h"
//#include "GetPathDlg.h"
#include "FileName.h"
#include "PathDlgDsc.h"

//#include "MessageBox.h"


// Local functions

static bool saveDlg(PathDlgDsc& dsc, bool overwrt, String& path);
static bool openDlg(PathDlgDsc& dsc, bool overwrt, String& path);
static bool pathDlg(PathDlgDsc& dsc, bool openDlg, DWORD flags, String& path);

                                                                              // OFN_CREATEPROMPT
bool createFileDlg(PathDlgDsc& dsc, String& path) {return pathDlg(dsc, true, 0, path);}

bool getOpenDlg(   PathDlgDsc& dsc, String& path) {return openDlg(dsc, false, path);}
bool getIncOpenDlg(PathDlgDsc& dsc, String& path) {return openDlg(dsc, true,  path);}

// Overwrites existing file

bool getSaveAsPathDlg(PathDlgDsc& dsc, String& path) {return saveDlg(dsc, true, path);}


// Allows adding to existing file

bool getIncSavePathDlg(PathDlgDsc& dsc, String& path) {return saveDlg(dsc, false, path);}



bool openDlg(PathDlgDsc& dsc, bool overwrt, String& path)
                             {return pathDlg(dsc, true,  overwrt ? OFN_OVERWRITEPROMPT : 0, path);}
bool saveDlg(PathDlgDsc& dsc, bool overwrt, String& path)
                             {return pathDlg(dsc, false, overwrt ? OFN_OVERWRITEPROMPT : 0, path);}


static bool pathDlg(PathDlgDsc& dsc, bool openDlg, DWORD flags, String& path) {
String        name   = getMainName(dsc.name);
String        filter = dsc.title + _T('|');
String        newTitle;

  if (!name.isEmpty() && !dsc.ext.isEmpty() && dsc.ext != _T("*")) name += _T('.') + dsc.ext;

  if (!dsc.pattern.isEmpty()) filter += dsc.pattern + _T("|All Files (*.*)|");
  filter += _T("*.*||");

  CFileDialog   dlg(openDlg, dsc.ext, name, flags, filter);
  OPENFILENAME& ofn    = dlg.m_ofn;

  newTitle = _T("Ugly ") + dsc.title;

  ofn.lpstrTitle = newTitle;   ofn.lpstrInitialDir = dsc.getPath();

  if (dlg.DoModal() != IDOK) return false;                                   //ofn.lpstrFile

  path = dlg.GetPathName();   return true;
  }


bool getDirPathDlg(TCchar* title, String& path) {
CFolderPickerDialog dlg(path);

  dlg.m_ofn.lpstrTitle = title;

  if (!path.isEmpty()) dlg.m_ofn.lpstrInitialDir = path;

  if (dlg.DoModal() != IDOK)return false;

  path = dlg.GetPathName();   if (path[path.length()-1] != _T('\\')) path += _T("\\");

  return true;
  }

