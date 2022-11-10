// Display Directories


#include "pch.h"
#include "DspDirs.h"
#include "GetPathDlg.h"
#include "PFFdirectories.h"
#include "PMFdirectories.h"


static TCchar* Title  = _T("Output Directories");
static TCchar* DefExt = _T("txt");
static TCchar* ExtPat = _T("*.txt");

DspDirs::DspDirs() {
PathDlgDsc dsc(Title, 0, DefExt, ExtPat);

  if (getSaveAsPathDlg(dsc, path)) fo.open(path, (FileIO::OpenParms) (FileIO::Write|FileIO::Create));
  }


void DspDirs::output() {

  output(_T("PFF Directories"), pffDirectories.stor);
  output(_T("PMF Directories"), pmfDirectories.stor);
  }


void DspDirs::output(TCchar* title, DirStor& stor) {
DirStorIter iter(stor);
DirDesc*    dsc;
String      s;

  if (!fo.isOpen()) return;

  fo.write(title); fo.crlf(); fo.crlf();

  for (dsc = iter(); dsc; dsc = iter++) {

    if (dsc->id.isEmpty()) continue;

    fo.write(_T("ID:           ")); fo.write(dsc->id); fo.crlf();
    fo.write(_T("WixID:        ")); fo.write(dsc->wixID); fo.crlf();
    fo.write(_T("Parent:       ")); fo.write(dsc->parent); fo.crlf();
    fo.write(_T("Name:         ")); fo.write(dsc->name); fo.crlf();
    fo.write(_T("Has Children: ")); s.format(_T("%i"), dsc->hasChildren);  fo.write(s); fo.crlf();
    fo.write(_T("In Use:       ")); s.format(_T("%i"), dsc->inUse);        fo.write(s); fo.crlf();
    fo.crlf();
    }
  }

