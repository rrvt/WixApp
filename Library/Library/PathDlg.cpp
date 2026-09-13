// Rework of PathDlgDsc and GetPathDlg


#include "pch.h"
#include "PathDlg.h"
#include "FileName.h"


PathDlg::PathDlg(TCchar* dlgTtl, TCchar* iniPath, TCchar* extension, TCchar* fltrTtl,
  TCchar* pat) : title(dlgTtl), iPath(iniPath), ext(extension), filterTtl(fltrTtl), pattern(pat) { }


PathDlg& PathDlg::operator() (TCchar* ttl, TCchar* iniPath, TCchar* extension,
                                                                    TCchar* fltrTtl, TCchar* pat) {
    title = ttl;         iPath      = iniPath ? iniPath : _T("");
    ext   = extension;   filterTtl = fltrTtl;   pattern = pat;   return *this;
    }


void PathDlg::copy(PathDlg& dsc) {
  title     = dsc.title;
  iPath     = dsc.iPath;
  ext       = dsc.ext;
  filterTtl = dsc.filterTtl;
  pattern   = dsc.pattern;
  }


bool PathDlg::createFile(String& path) {return doDlg(true, 0, path);}

bool PathDlg::open(bool overwrt, String& path)
                                  {return doDlg(true,  overwrt ? OFN_OVERWRITEPROMPT : 0, path);}

bool PathDlg::save(bool overwrt, String& path)
                                  {return doDlg(false, overwrt ? OFN_OVERWRITEPROMPT : 0, path);}


bool PathDlg::doDlg(bool open, DWORD flags, String& path) {
String mainName = getMainName(iPath);
String iniPath  = getPath(iPath);
String filter;

  if (!mainName.isEmpty() && !ext.isEmpty() && ext != _T("*")) mainName += _T('.') + ext;

  filter = filterTtl + _T('|');
  if (!pattern.isEmpty()) filter += pattern + _T("|All Files (*.*)|");
  filter += _T("*.*||");

  CFileDialog dlg(open, ext, mainName, flags, filter);

  OPENFILENAME& ofn = dlg.m_ofn;   ofn.lpstrTitle = title;   ofn.lpstrInitialDir = iniPath;

  if (dlg.DoModal() != IDOK) return false;

  path = dlg.GetPathName();   return true;
  }





bool PathDlg::folder(String& path) {
CFolderPickerDialog dlg(path);

  dlg.m_ofn.lpstrTitle = title;

  if (!iPath.isEmpty()) dlg.m_ofn.lpstrInitialDir = getPath(iPath);

  if (dlg.DoModal() != IDOK)return false;

  path = dlg.GetPathName();   if (path[path.length()-1] != _T('\\')) path += _T("\\");

  return true;
  }

