// Directory Fragment


#include "pch.h"
#include "PMFdirectories.h"
#include "Feature.h"
#include "WixUtilities.h"

//TCchar* PffExt       = _T("pff");
TCchar* PmfExt       = _T("pmf");
//TCchar* DktExt       = _T("dkt");

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


void PMFdirectories::output() {
String   line;

  if (startMenuSeen && stor.getDefault()) {
    wix(2);   wix(_T("<Directory Id=\"ProgramMenuFolder\">"));   wix.crlf();      // Name=\"StartMenu\"

    stor.outputSubs(String(_T("")), 3);

    wix(2);   wix(_T("</Directory>"));      wix.crlf(2);
    }

  wix(2);
  if (desktopDirSeen) wix(_T("<Directory Id=\"DesktopFolder\" />"));   wix.crlf();
  if (startupDirSeen) wix(_T("<Directory Id=\"StartupFolder\" />"));   wix.crlf();

  wix(1);   wix(_T("</Directory>"));   wix.crlf(2);
  }


