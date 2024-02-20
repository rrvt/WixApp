// Simple file I/O using CFile from MFC


#pragma once


enum Encoding {NilEncode, Utf8, Utf16, Utf16le};


class FileIO {
String         path;
uint           openParms;
CFile          cfile;
ULONGLONG      pos;
int            encdState;
Encoding       encoding;
CFileException err;

char*          pbuf;                // read buffer
char*          ebuf;
char           buf[1024];
bool           rtnSeen;
bool           eof;                 // End of File when reading

int            tabSize;
int            col;

int            lastOP;

public:

enum OpenParms {Read=1, Write=2, Create=4};

  FileIO();
 ~FileIO() {close();}

  void setMode(int parms);                   // Set if parms
  bool open(String& filePath, int parms) {return open(filePath.str(), parms);}
  bool open(TCchar* filePath, int parms);
  void close();

  String getFilePath();

  void seekEnd();

  bool reOpen();

  bool isOpen() {return cfile.m_hFile != CFile::hFileNull;}

  bool getModifiedTime(CTime& time);
  int  getLength();
  int  getLength(TCchar* path);

  bool write(String& s);                            // Writes a string (unicode or ansi)
  bool write(TCchar* s);                            // Writes a string of Tchars (unicod or ansi)
  bool write(Tchar   c);                            // Writes one character (unicode or ansi)
  bool write(void* blk, int noBytes);               // writes n bytes (not chars or Tchars) without
                                                    // interpretation of /n or /r
  bool write(Byte    v);                            // Writes one byte without interpretation of /n or /r

  bool read(String& s);                             // Reads a line terminated by _T("\n")
  bool read(String& s, int  n);                     // Read n characters into a string
  bool read(Tchar&  c);                             // reads on char or wchar (i.e. a Tchar)
  bool read(Byte&   v);                             // Reads one byte without interpretation of /n or /r
  bool read(void* blk, int& n);                     // Reads up to n bytes in blk, returns true and no of
                                                    // bytes in blk
  void setTabSize(int nSpaces)                      // Set Tab Size for output, default is 2 spaces
              {if (nSpaces > 0) tabSize = nSpaces;}
  void tab(   int nTabs);                           // Tab every 2 spaces
  void spaces(int nSpaces);
  void crlf() {write(_T('\n'));}

  Tchar* getLastError();                            // Returns last error

  void   clearException();

private:

  bool getChar(char& ch);
  void sendChar(char ch);
  void enCode(Tchar tch);
  bool fillBuf();
  void flush();
  void saveExcp(CFileException* e);
  };



//  int  readRaw(void* blk, uint n);                  // Read block of data from buffer without
//  void writeRaw(void* blk, int noBytes);

