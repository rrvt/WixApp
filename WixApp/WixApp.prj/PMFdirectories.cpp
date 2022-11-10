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






#if 0
//  d = pff.getDefault();
  d = 0;   // Fix this
  if (d) wix.out(1, _T("<Property Id=\"ApplicationFolderName\" Value=\""), d->name, _T("\" />"));

  wix.out(1, _T("<Property Id=\"WIXUI_INSTALLDIR\"      Value=\""), appDir.wixID, _T("\"/>"));
  wix.out(1, _T("<Property Id=\"WixAppFolder\"          Value=\""),
                                                        String(_T("WixPerMachineFolder")), _T("\" />"));
  wix.lit(0, _T("</Fragment>\n"));
  }
#endif
#if 0
void DirDesc::store(CString& cs, TCchar* ext) {
  findLastName(String(cs), parent, name);   wixID = getWixID(id, ext);
  }
#endif


#if 0
void PMFdirectories::output() {
String   line;
DirDesc* d;

  wix.crlf();

  wix.lit(0, _T("<!--Directory structure-->\n"));
  wix.lit(0, _T("<Fragment>\n"));

  wix.lit(1, _T("<Directory Id=\"TARGETDIR\" Name=\"SourceDir\">\n"));
  wix.lit(2, _T("<Directory Id=\"ProgramFilesFolder\">\n"));

//  pff.outputSubs(String(_T("")), 3);

  wix.lit(2, _T("</Directory>\n\n"));
#endif

