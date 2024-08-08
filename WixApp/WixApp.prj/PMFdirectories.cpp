// Directory Fragment


#include "pch.h"
#include "PMFdirectories.h"
#include "Feature.h"
#include "WixUtilities.h"

TCchar* PmfExt       = _T("pmf");

TCchar* StartMenuDir = _T("StartMenuDir");
TCchar* DeskTopDir   = _T("DesktopFolder");
TCchar* StartupDir   = _T("StartupFolder");


PMFdirectories pmfDirectories;


String PMFdirectories::fullPath(String& id) {
DirDesc* dsc;
String   path;

  for (dsc = find(id); dsc; dsc = find(dsc->parent)) {
    path = path.isEmpty() ? path = dsc->name : dsc->name + _T('\\') + path;
    }

  return path;
  }


void PMFdirectories::output(int& tab) {
String   line;

  if (startMenuSeen && stor.getDefault()) {
    wix.crlf();
    wix(tab, _T("<Directory Id=\"ProgramMenuFolder\">"));

    tab += TabVal;

    stor.outputSubs(tab, _T(""));

    tab -= TabVal;
    wix(tab, _T("</Directory>"));
    }

  if (desktopDirSeen) {wix.crlf(); wix(tab, _T("<Directory Id=\"DesktopFolder\" />"));}
  if (startupDirSeen) {wix.crlf(); wix(tab, _T("<Directory Id=\"StartupFolder\" />"));}

  tab -= TabVal;
  wix(tab, _T("</Directory>"));
  }


