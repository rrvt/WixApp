// Configure ConfigFile Class


#include "stdafx.h"
#include "DataFile.h"
#include "AppDoc.h"


const String LastFile     = _T("LastFileUsed");
const String LastTmplFile = _T("LastTmplFileUsed");



// Reads one character from data and increments current position

bool DataFile::getCh(Tchar& ch) {

  if (readX >= lines.end() && !pLine) return false;

  if (!pLine) {curRdLine = readX; pLine = lines[readX++].str();}

  ch = *pLine++;   if (!ch) {ch = '\n'; pLine = 0;}

  return true;
  }


void DataFile::put(TCchar ch) {

  if (ch == _T('\r')) {curWrtPos = 0;               return;}
  if (ch == _T('\n')) {curWrtLine++; curWrtPos = 0; return;}

  String& line(lines[curWrtLine]);
  int     endPos = line.size();

  if (ch == _T('\t')) {
    int i   = curWrtPos;
    int pos = i % 2;   pos = (pos + 1) * 2;

    for (; i < pos; i++) {
      if (curWrtPos < endPos) line[curWrtPos++] = ch;
      else {                  line += _T(' '); curWrtPos++;}
      }
    return;
    }

  if (curWrtPos < endPos) line[curWrtPos++] = ch;
  else {                  line += ch; curWrtPos++;}
  }


void DataFile::clear() {

  name.clear(); path.clear(); readX = curRdLine = writeX = curWrtLine = curWrtPos = 0;
  clearLines();
  }


void DataFile::clearLines() {
int i;

  for (i = 0; i < lines.end(); i++) lines[i].clear();
  lines.clr();
  }


// Open file on disk and start read

bool DataFile::openFor(const String& key, Device* dev, String& defName, TCchar* defExt, TCchar* filter) {
Cstring fileName = defName;
CString fullPath;

  iniRead(key, fileName);

  CFileDialog dlgFile(true, defExt, fileName,
                                        OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST, filter, AfxGetMainWnd());
  if (dlgFile.DoModal() != IDOK) return false;

  fileName  = dlgFile.GetFileName();
  String fp = fullPath  = dlgFile.GetPathName();

  if (!loadFile(dev, fp)) return false;

  iniWrite(key, fullPath);

  return true;
  }


bool DataFile::loadFile(Device* dev, String& fullPath) {return doOpen(dev, this, fullPath);}


// Read lines from disk to lines

void DataFile::readFile(CArchive& ar) {
CString line;

  openWrite();

  while (ar.ReadString(line)) {String l = line; writeNext(l);}
  }


void DataFile::saveFileFor(Device* dev, const String& key, String& defName,
                                                                          TCchar* defExt, TCchar* filter) {
Cstring fileName = defName;
int     lng      = defName.size();
CString fullPath;

  if (!lng) iniRead(key, fileName);

  CFileDialog dlgFile(false, defExt, fileName, OFN_OVERWRITEPROMPT, filter, AfxGetMainWnd());

  if (dlgFile.DoModal() != IDOK) return;

  fileName  = dlgFile.GetFileName();
  String fp = fullPath  = dlgFile.GetPathName();

  if (!saveFile(dev, fp)) return;

  if (!lng) iniWrite(key, fullPath);
  }


bool DataFile::saveFile(Device* dev, String& fullPath) {return doSave(dev, this, fullPath);}

