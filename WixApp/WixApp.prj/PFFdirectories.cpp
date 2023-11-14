// Directory Fragment


#include "pch.h"
#include "PFFdirectories.h"
#include "Feature.h"
#include "WixUtilities.h"

TCchar* PffExt       = _T("pff");


PFFdirectories pffDirectories;


String PFFdirectories::fullPath(String& id) {
DirDesc* dsc;
String   path;

  for (dsc = find(id); dsc; dsc = find(dsc->parent)) {
    path = path.isEmpty() ? path = dsc->name : dsc->name + _T('\\') + path;
    }

  return path;
  }



void PFFdirectories::begOutput() {
String   line;

  wix.crlf();

  wix(0);   wix(_T("<!--Directory structure-->"));   wix.crlf();
            wix(_T("<Fragment>"));   wix.crlf();

  wix(1);   wix(_T("<Directory Id=\"TARGETDIR\" Name=\"SourceDir\">"));   wix.crlf();
  wix(2);   wix(_T("<Directory Id=\"ProgramFilesFolder\">"));   wix.crlf();

  stor.outputSubs(_T(""), 3);

  wix(2);   wix(_T("</Directory>"));   wix.crlf(2);
  }


void PFFdirectories::finOutput() {
DirDesc* d;

  d = stor.getDefault();   wix(1);
  if (d) wix(_T("<Property Id=\"ApplicationFolderName\" Value=\""), d->name, _T("\" />"));

  wix(_T("<Property Id=\"WIXUI_INSTALLDIR\"      Value=\""), appDir->wixID, _T("\"/>"));
  wix(_T("<Property Id=\"WixAppFolder\"          Value=\""), _T("WixPerMachineFolder"), _T("\" />"));
  wix(0);   wix(_T("</Fragment>"));   wix.crlf();
  }





#if 0
void DirDesc::store(CString& cs, TCchar* ext) {
  findLastName(String(cs), parent, name);   wixID = getWixID(id, ext);
  }
#endif
#if 0
  if (pmf.getDefault()) {
    wix.lit(2, _T("<Directory Id=\"ProgramMenuFolder\" Name=\"StartMenu\">\n"));

    pmf.outputSubs(String(_T("")), 3);

    wix.lit(2, _T("</Directory>\n\n"));
    }
  wix.out(2, _T("<Directory Id=\""), String(DeskTopDir), _T("\" Name=\"Desktop\" />"));

  wix.lit(1, _T("</Directory>\n\n"));
#endif
//TCchar* PmfExt       = _T("pmf");
//TCchar* DktExt       = _T("dkt");

//TCchar* StartMenuDir = _T("StartMenuDir");
//TCchar* DeskTopDir   = _T("DesktopFolder");

