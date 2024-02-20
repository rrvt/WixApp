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

