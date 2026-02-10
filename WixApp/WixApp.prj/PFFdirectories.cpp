// Directory Fragment


#include "pch.h"
#include "PFFdirectories.h"
#include "Feature.h"
#include "WixUtilities.h"


TCchar* PffExt = _T("pff");




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
DirDesc* d = stor.getDefault();

  wix(tab, _T("<!--Directory structure-->"));
  wix(tab, _T("<Fragment>"));

  tab += TabVal;

  if (d) wix(tab, _T("<Property Id=\"ApplicationFolderName\" Value=\""), d->name, _T("\" />"));
  wix(tab, _T("<Property Id=\"WixAppFolder\"          Value=\""),
                                                           _T("WixPerMachineFolder"), _T("\" />"));
  wix.crlf();

  wix(tab, _T("<StandardDirectory Id=\"ProgramFiles6432Folder\">"));

  tab += TabVal;   stor.outputSubs(tab, _T(""));   tab -= TabVal;

  wix(tab, _T("</StandardDirectory>"));
  }


void PFFdirectories::finOutput(int tab) {

  tab -= TabVal;

  wix(tab, _T("</Fragment>"));
  }


void PFFdirectories::saveData(Archive& ar) {

  ar << _T("PFF Directories") << aCrlf;

  if (appDir) {ar << _T("App Directory") << aCrlf;   appDir->saveData(ar);}

  stor.saveData(ar);
  }



/////////---------------

//DirDesc* d;

//  d = stor.getDefault();

//  if (d) wix(tab, _T("<Property Id=\"ApplicationFolderName\" Value=\""), d->name, _T("\" />"));

//  wix(tab, _T("<Property Id=\"WIXUI_INSTALLDIR\"      Value=\""), appDir->wixID, _T("\" />"));

