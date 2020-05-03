// Directory Fragment


#include "stdafx.h"
#include "Directory.h"
#include "Feature.h"
#include "WixUtilities.h"

TCchar* PffExt       = _T("pff");
TCchar* PmfExt       = _T("pmf");
TCchar* DktExt       = _T("dkt");

TCchar* StartMenuDir = _T("StartMenuDir");
TCchar* DeskTopDir   = _T("DesktopFolder");


Directory directory;


#if 0
void DirDesc::store(CString& cs, TCchar* ext) {
  findLastName(String(cs), parent, name);   wixID = getWixID(id, ext);
  }
#endif


void Directory::output() {
String   line;
DirDesc* d;

  wix.crlf();

  wix.lit(0, _T("<!--Directory structure-->\n"));
  wix.lit(0, _T("<Fragment>\n"));

  wix.lit(1, _T("<Directory Id=\"TARGETDIR\" Name=\"SourceDir\">\n"));
  wix.lit(2, _T("<Directory Id=\"ProgramFilesFolder\">\n"));

  pff.outputSubs(String(_T("")), 3);

  wix.lit(2, _T("</Directory>\n\n"));

  if (pmf.getDefault()) {
    wix.lit(2, _T("<Directory Id=\"ProgramMenuFolder\" Name=\"StartMenu\">\n"));

    pmf.outputSubs(String(_T("")), 3);

    wix.lit(2, _T("</Directory>\n\n"));
    }

  wix.out(2, _T("<Directory Id=\""), String(DeskTopDir), _T("\" Name=\"Desktop\" />"));

  wix.lit(1, _T("</Directory>\n\n"));

  d = pff.getDefault();
  if (d) wix.out(1, _T("<Property Id=\"ApplicationFolderName\" Value=\""), d->name, _T("\" />"));

  wix.out(1, _T("<Property Id=\"WIXUI_INSTALLDIR\"      Value=\""), appDir.wixID, _T("\"/>"));
  wix.out(1, _T("<Property Id=\"WixAppFolder\"          Value=\""),
                                                        String(_T("WixPerMachineFolder")), _T("\" />"));
  wix.lit(0, _T("</Fragment>\n"));
  }


void Directory::outputRemoves(int tab) {
  pmf.outputRemoves(tab);                             //dkt.outputRemoves(tab);
  }




void Directory::prepareUninstalls(Feature* grp) {pmf.prepareUninstalls(grp);}


