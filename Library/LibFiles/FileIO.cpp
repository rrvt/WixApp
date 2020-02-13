// Simple file I/O using CFile from MFC



#include "stdafx.h"
#include "FileIO.h"


static const int WriteMode = CFile::modeWrite | CFile::modeReadWrite;


FileIO::FileIO() : openParms(0), pos(0), pbuf(buf), ebuf(buf), rtnSeen(false),
                                                                        tabSize(2), col(0) {buf[0] = 0;}


bool FileIO::open(String& filePath, OpenParms parms) {

  if (isOpen()) return true;

  openParms = 0;

  if (      (parms & Read) && !(parms & Write)) openParms |= CFile::modeRead;
  else if (!(parms & Read) &&  (parms & Write)) openParms |= CFile::modeWrite;
  else if ( (parms & Read) &&  (parms & Write)) openParms |= CFile::modeReadWrite;

  openParms |= (openParms & 3) == CFile::modeWrite ? CFile::modeCreate : CFile::modeNoTruncate;

  openParms |= CFile::typeBinary | CFile::shareDenyNone;

  if ((openParms & 3) == CFile::modeRead) pbuf = ebuf = buf;
  else                                   {pbuf = buf; ebuf = buf + noElements(buf);}

  path = filePath;   return cfile.Open(path, openParms, &err) != 0;
  }


bool FileIO::reOpen() {

  if (isOpen()) return true;   if (path.isEmpty()) return false;

  if (cfile.Open(path, openParms, &err) != 0) {cfile.Seek(pos, 0);   return true;}

  return false;
  }


void FileIO::close() {

  if (openParms & WriteMode) flush();

  if (isOpen()) {pos = cfile.GetPosition(); cfile.Close();}
  }


bool FileIO::getModifiedTime(CTime& time) {
CFileStatus status;

  if (cfile.m_hFile == CFile::hFileNull) return false;

  if (!cfile.GetStatus(status)) return false;

  time = status.m_mtime; return true;
  }



bool FileIO::write(TCchar* s) {
Tchar ch;

  if (!s) return true;

  for (ch = *s; ch; ch = *++s) write(ch);

  return err.m_cause == CFileException::none;
  }


bool FileIO::write(String& s) {
int n = s.length();
int i;

  for (i = 0; i < n; i++) write(s[i]);

  return err.m_cause == CFileException::none;
  }


void FileIO::tab(int nTabs) {
int n = nTabs * tabSize - col;

  if (n > 0) spaces(n);
  }


void FileIO::spaces(int nSpaces) {
int i;

  for (i = 0; i < nSpaces; i++) write(_T(' '));
  }


bool FileIO::write(Tchar ch) {

  if (ch == _T('\n') && !rtnSeen) write(_T('\r'));

  if (pbuf >= ebuf) flush();

  *pbuf++ = (char) ch;   rtnSeen = false;

  if (ch == _T('\r')) rtnSeen = true;

  col = ch == _T('\n') ? 0 : col+1;

  return err.m_cause == CFileException::none;
  }


bool FileIO::write(void* blk, int nBytes) {
int   i;
char* p = (char*) blk;

  for (i = 0; i < nBytes; i++) {

    if (pbuf >= ebuf) flush();

    *pbuf++ = *p++;
    }

  return err.m_cause == CFileException::none;
  }


// Writes one byte without interpretation of /n or /r

bool FileIO::write(Byte v) {
  if (pbuf >= ebuf) flush();

  *pbuf++ = v;

  return err.m_cause == CFileException::none;
  }


void FileIO::flush() {
uint noBytes = (uint) (pbuf - buf);

  if (noBytes) try {cfile.Write(buf, noBytes);} catch (CFileException* e) {saveExcp(e); return;}

  pbuf = buf;
  }


bool FileIO::read(String& s) {
Tchar ch;

  s.clear();

  while (read(ch)) {

    s += ch;   if (ch == _T('\n')) return true;;
    }

  return s.length() > 0;
  }



// Read n characters into a string

bool FileIO::read(String& s, int n) {
int   i;
Tchar ch;

  s.clear();   if (!n) return true;

  for (i = 0; i < n; i++) {read(ch);   s += ch;}

  return s.length() > 0;
  }


bool FileIO::read(Tchar& c) {

  if (pbuf >= ebuf && !fillBuf()) return false;

  c = *pbuf++;   c &= 0xff;

  return c == _T('\r') ? read(c) : true;
  }


// Read block of data from buffer

bool FileIO::read(void* blk, int n) {
int   i;
char* p = (char*) blk;

  for (i = 0; i < n; i++) {
    if (pbuf >= ebuf && !fillBuf()) return false;

    *p++ = *pbuf++;
    }

  return true;
  }


// Reads one byte without interpretation of /n or /r

bool FileIO::read(Byte& v) {
  if (pbuf >= ebuf && !fillBuf()) return false;

  v = (Byte) *pbuf++;  return true;
  }


// returns number of bytes read or -1

bool  FileIO::fillBuf() {
uint noBytes;

  try {noBytes = cfile.Read(buf, noElements(buf));}
  catch (CFileException* e) {saveExcp(e); return false;}

  if (noBytes == 0) return false;

  ebuf = (pbuf = buf) + noBytes;   return true;
  }


void FileIO::saveExcp(CFileException* e) {
  err.m_cause       = e->m_cause;
  err.m_lOsError    = e->m_lOsError;
  err.m_strFileName = e->m_strFileName;
  e->Delete();
  }



// Returns last error

Tchar* FileIO::getLastError()
        {static Tchar stg[128];   stg[0] = 0;   err.GetErrorMessage(stg, noElements(stg));   return stg;}


