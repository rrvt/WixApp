// Directory Fragment


#include "pch.h"
#include "PFFdirectories.h"
#include "Feature.h"
#include "WixUtilities.h"


TCchar* PffExt = _T("pff");


PFFdirectories pffDirectories;


String PFFdirectories::fullPath(String& id) {
DirDesc* dsc;
String   path;

  for (dsc = find(id); dsc; dsc = find(dsc->parent)) {
    path = path.isEmpty() ? path = dsc->name : dsc->name + _T('\\') + path;
    }

  return path;
  }



void PFFdirectories::begOutput(int& tab) {
String   line;

  wix(tab, _T("<!--Directory structure-->"));
  wix(tab, _T("<Fragment>"));
  tab += TabVal;   wix(tab, _T("<Directory Id=\"TARGETDIR\" Name=\"SourceDir\">"));
  tab += TabVal;   wix(tab, _T("<Directory Id=\"ProgramFilesFolder\">"));

  tab += TabVal;   stor.outputSubs(tab, _T(""));

  tab -= TabVal;   wix(tab, _T("</Directory>"));
  }


void PFFdirectories::finOutput(int tab) {
DirDesc* d;

  d = stor.getDefault();

  if (d) wix(tab, _T("<Property Id=\"ApplicationFolderName\" Value=\""), d->name, _T("\" />"));

  wix(tab, _T("<Property Id=\"WIXUI_INSTALLDIR\"      Value=\""), appDir->wixID, _T("\"/>"));
  wix(tab, _T("<Property Id=\"WixAppFolder\"          Value=\""), _T("WixPerMachineFolder"),
                                                                                      _T("\" />"));
  tab -= TabVal;

  wix(tab, _T("</Fragment>"));
  }


void PFFdirectories::saveData(Archive& ar) {

  ar << _T("PFF Directories") << aCrlf;

  if (appDir) {ar << _T("App Directory") << aCrlf;   appDir->saveData(ar);
  }

  stor.saveData(ar);
  }




