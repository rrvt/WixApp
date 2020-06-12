// Simple file I/O using CFile from MFC



#include "stdafx.h"
#include "FileIO.h"


static const int WriteMode = CFile::modeWrite | CFile::modeReadWrite;

// First Characters of file determine encoding of file
// UTF-8 EF BB BF
// UTF-16 FE FF    Big Endian Unicode as best as I can determine at this time
// UTF-16 FF FE    Little Endian Unicode as best as I can determine at this time
// Other cases not used herein
// None of the above means UTF-8 (i.e. Ansi encoding)

static char EF = (char) 0xef;
static char BB = (char) 0xbb;
static char BF = (char) 0xbf;
static char FF = (char) 0xff;
static char FE = (char) 0xfe;


FileIO::FileIO() : encoding(NilEncode), encdState(0), openParms(0), pos(0), pbuf(buf), ebuf(buf), 
																						rtnSeen(false), tabSize(2), col(0), lastOP(0) {buf[0] = 0;}


bool FileIO::open(String& filePath, int parms) {

  if (isOpen()) return true;

  setMode(parms);

//  if ((openParms & 3) == CFile::modeRead) pbuf = ebuf = buf;
//  else                                   {pbuf = buf; ebuf = buf + noElements(buf);}

  pbuf = ebuf = buf;  path = filePath;

  if (cfile.Open(path, openParms, &err) != 0) return true;

  return false;
  }


void FileIO::setMode(int parms) {

  openParms = 0;

  if (      (parms & Read) && !(parms & Write)) openParms |= CFile::modeRead;
  else if (!(parms & Read) &&  (parms & Write)) openParms |= CFile::modeWrite;
  else if ( (parms & Read) &&  (parms & Write)) openParms |= CFile::modeReadWrite;

  openParms |= (openParms & 3) == CFile::modeWrite ? CFile::modeCreate : CFile::modeNoTruncate;

  openParms |= CFile::typeBinary | CFile::shareDenyNone;
  }


bool FileIO::reOpen() {

  if (isOpen()) return true;   if (path.isEmpty()) return false;

  if (cfile.Open(path, openParms, &err) != 0) {cfile.Seek(pos, 0); pbuf = ebuf = buf; return true;}

  return false;
  }


void FileIO::seekEnd() {
  if ((lastOP & Write) && (openParms & WriteMode)) flush();

  pos = cfile.SeekToEnd(); pbuf = ebuf = buf;
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


bool FileIO::write(Tchar c) {

  if (encoding == NilEncode) {
    switch (encdState) {
      case 0: if (c == 0xef)   {encdState = 1;      break;}
              if (c == 0xfeff) {encoding = Utf16le; break;}
              if (c == 0xfffe) {encoding = Utf16;   break;}
                                encoding = Utf8;    break;

      case 1: if (c == 0xbb)    encdState = 2;      break;
      case 2: if (c == 0xbf)    encoding = Utf8;    break;
      }
    }

  if (c == _T('\n') && !rtnSeen) write(_T('\r'));

  sendChar((char) c);

  if (encoding == Utf16le) sendChar((char) (c >> 8));

  rtnSeen = false;

  if (c == _T('\r')) rtnSeen = true;

  col = c == _T('\n') ? 0 : col+1;

  return err.m_cause == CFileException::none;
  }


bool FileIO::write(void* blk, int nBytes) {
int   i;
char* p = (char*) blk;

  for (i = 0; i < nBytes; i++) {

//    if (pbuf >= ebuf) flush();   *pbuf++ =

    sendChar(*p++);
    }

  return err.m_cause == CFileException::none;
  }


// Writes one byte without interpretation of /n or /r

bool FileIO::write(Byte v) {

//  if (pbuf >= ebuf) flush();   *pbuf++ =

  sendChar(v);

  return err.m_cause == CFileException::none;
  }



void FileIO::sendChar(char ch) {if (pbuf >= ebuf) flush();   *pbuf++ = ch;  lastOP = Write;}


void FileIO::flush() {
uint noBytes = (uint) (pbuf - buf);

  if (noBytes) try {cfile.Write(buf, noBytes);} catch (CFileException* e) {saveExcp(e); return;}

  pbuf = buf;   ebuf = buf + noElements(buf);
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
char ch;

  if (!getChar(ch)) return false;

  c = ch;  c &= 0xff;

  if (encoding == Utf16le && getChar(ch)) c |= ch << 8;

  return c == _T('\r') ? read(c) : true;
  }


// Read block of data from buffer

bool FileIO::read(void* blk, int n) {
int   i;
char* p = (char*) blk;
char  ch;

  for (i = 0; i < n; i++) {
    if (!getChar(ch)) return false;

    *p++ = ch;
    }

  return true;
  }


// Reads one byte without interpretation of /n or /r

bool FileIO::read(Byte& v) {
char ch;

  if (!getChar(ch)) return false;

  v = (Byte) ch;  return true;
  }



bool FileIO::getChar(char& ch) {

  if (pbuf >= ebuf && !fillBuf()) return false;

  ch = *pbuf++;   lastOP = Read;   return true;
  }


// returns number of bytes read or -1

bool  FileIO::fillBuf() {
uint noBytes;

  try {noBytes = cfile.Read(buf, noElements(buf));}
  catch (CFileException* e) {saveExcp(e); return false;}

  if (noBytes == 0) return false;

  ebuf = (pbuf = buf) + noBytes;

  if (encoding == NilEncode) {

    if (     buf[0] == EF && buf[1] == BB && buf[2] == BF)  encoding = Utf8;
    else if (buf[0] == FF && buf[1] == FE)                  encoding = Utf16le;
    else if (buf[0] == FE && buf[1] == FF)                  encoding = Utf16;
    else                                                    encoding = Utf8;
    }

  return true;
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


