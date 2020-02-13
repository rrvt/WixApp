// Configure ConfigFile Class


#pragma once
#include "Expandable.h"
#include "AppDoc.h"
#include "IniFile.h"


class Device;


extern const String LastFile;
extern const String LastTmplFile;


class DataFile {
FileType typ;
String   path;
int      readX;       // read index during reading from lines, incremented after each readNext
int      curRdLine;   // Current line index during reading
TCchar*  pLine;       // Pointer to current line when getCh invoked
int      writeX;      // write index during writing to lines
int      curWrtLine;  // Current line index during writing
int      curWrtPos;   // Current character pos in line (0 indexed)

Expandable <String, 128> lines;

public:

String name;

  DataFile(FileType t = GenericType) : typ(t),    readX(0),      curRdLine(0),  pLine(0),
                                       writeX(0), curWrtLine(0), curWrtPos(0) {}
  ~DataFile() { lines.clr(); }

  void  setType(FileType t) {typ = t;}

  void clear();                               // Clear everything
  void clearLines();                          // Clear Lines

  bool empty() {return lines.end() == 0;}     // Determine if there is any data in object.

  // Read lines in object

  String* startRead() {curRdLine = readX = 0; pLine = 0;  return readNext();}

  String* readNext() {
    if (readX >= lines.end()) {curRdLine = readX = 0; return 0;}

    curRdLine = readX;   return &lines[readX++];
    }

  bool getCh(Tchar& ch);                // Reads one character from data and increments current position

  bool isFinished() {return readX < lines.end();}

  bool readCur(String*& line)
                  {if (curRdLine < lines.end()) {line = &lines[curRdLine]; return true;}   return false;}

  // Write lines to object

  void openWrite() {writeX = curWrtLine = curWrtPos = 0; clearLines();}

  void writeNext(String& line) {
    curWrtLine = lines.end();
    lines[curWrtLine] = line;
//    String& xx = lines[curWrtLine];
    curWrtPos = line.length();
    }

  void put(TCchar ch);      // Put character in current line with interpretation of tab, return, line feed

  // Open file on disk and start read
  bool open(Device* dev, String& defName, TCchar* defExt, TCchar* filter)
                                              {return openFor(LastFile, dev, defName, defExt, filter);}
  void readFile(CArchive& ar);                                       // Read lines from disk to lines

  // Save lines to file and start write

  void saveAs(Device* dev, String& defName, TCchar* defExt, TCchar* filter)
                                        {return saveFileFor(dev, LastFile, defName, defExt, filter);}

  bool openTemplate(Device* dev, String& defName, TCchar* defExt, TCchar* filter)
                                    {return openFor(LastTmplFile, dev, defName, defExt, filter);}
  void saveTemplate(Device* dev, String& defName, TCchar* defExt, TCchar* filter)
                                    {return saveFileFor(dev, LastTmplFile, defName, defExt, filter);}
  bool loadFile(Device* dev, String& fullPath);
  bool saveFile(Device* dev, String& fullPath);

private:

  bool openFor( const String& key, Device* dev, String& defName, TCchar* defExt, TCchar* filter);
  void saveFileFor(Device* dev, const String& key, String& defName, TCchar* defExt, TCchar* filter);
  void iniRead( const String& key, CString& fileName) {iniFile.readString(getObjName(), key, fileName);}
  void iniWrite(const String& key, CString& fullPath) {iniFile.writeString(getObjName(), key, fullPath);}
  };
