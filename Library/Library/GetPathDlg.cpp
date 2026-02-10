// Uses a Windows dialog to obtain a path to a file


#include "pch.h"
#include "GetPathDlg.h"
#include "FileName.h"
#include "PathDlgDsc.h"

#include "MessageBox.h"


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
String        e = dsc.title + _T('|') + dsc.pattern + _T("|All Files (*.*)|*.*||");
CFileDialog   dlg(openDlg, dsc.ext, dsc.name, flags, e, 0);
OPENFILENAME& ofn = dlg.m_ofn;
Cstring       lbl = _T("Mumble");

  ofn.lpstrTitle = dsc.title;   ofn.lpstrInitialDir = dsc.getPath();

  dlg.SetControlLabel(IDOK, lbl);

  if (dlg.DoModal() == IDOK) {path = dlg.GetPathName();   return true;}

  return false;                                   //ofn.lpstrFile
  }


bool getDirPathDlg(TCchar* title, String& path) {
CFolderPickerDialog dlg(path);

  dlg.m_ofn.lpstrTitle = title;

  if (!path.isEmpty()) dlg.m_ofn.lpstrInitialDir = path;

  if (dlg.DoModal() == IDOK) {
    path = dlg.GetPathName();   if (path[path.length()-1] != _T('\\')) path += _T("\\");
    return true;
    }

  return false;
  }

