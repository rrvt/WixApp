// Manage Ini File (A Place where stuff is remembered from execution to execution)
// Copyright Software Design & Engineering, Robert R. Van Tuyl, 2013.  All rights reserved.


#pragma once
#include "Random.h"


class IniFile {
String   iniFilePath;
int      pathLng;

Random   rand;

public:

  IniFile() : rand() {}
 ~IniFile() { }

  String  getAppDataPath(TCchar* helpPath);   // Sets ini File Name & path to appData/Roming directory
                                              // returns path to directory
  void    setPath(TCchar* filePath);          // Sets ini file name & path.
  String  path() {return iniFilePath;}

  void    setFilePath(String& pth,         CWinApp& theApp);
  void    setFilePath(TCchar* pth,         CWinApp& theApp);
  void    setAppDataPath(TCchar* helpPath, CWinApp& theApp);

  String  initIniDatum(TCchar* section, TCchar* name, TCchar* deflt, String& valu);
  void    saveIniDatum(TCchar* section, TCchar* name, String& valu);
  String  initIniPwd(  TCchar* section, TCchar* name, TCchar* deflt, String& valu);
  void    saveIniPwd(  TCchar* section, TCchar* name, String& valu);

  bool    writeString(TCchar* section, TCchar* key, TCchar*  val);
  bool    write(      TCchar* section, TCchar* key, TCchar*  val);
  bool    writeString(TCchar* section, TCchar* key, CString& val);
  bool    write(      TCchar* section, TCchar* key, CString& val);
  bool    write(      TCchar* section, TCchar* key, int      val) {return writeInt(section, key, val);}
  bool    writeInt(   TCchar* section, TCchar* key, int      val);
  bool    write(      TCchar* section, TCchar* key, double   val);
  bool    writePwd(   TCchar* section, TCchar* key, String&  val);
  void    writeEnd();

  bool    readString( TCchar* section, TCchar* key, String&  val);
  bool    read(       TCchar* section, TCchar* key, String&  val) {return readString(section, key, val);}
  bool    readString( TCchar* section, TCchar* key, Cstring& val);
  bool    read(       TCchar* section, TCchar* key, Cstring& val) {return readString(section, key, val);}
  bool    read(       TCchar* section, TCchar* key, int&     val);                // Defaults to zero

  bool    readString( TCchar* section, TCchar* key, String&  val, TCchar* dflt);
  bool    readString( TCchar* section, TCchar* key, Cstring& val, TCchar* dflt);
  bool    read(       TCchar* section, TCchar* key, String&  val, TCchar* dflt);
  bool    read(       TCchar* section, TCchar* key, Cstring& val, TCchar* dflt);
  bool    read(       TCchar* section, TCchar* key, int&     val, int     dflt);
  int     readInt(    TCchar* section, TCchar* key, int      def);
//int     read(       TCchar* section, TCchar* key, int      def);
  bool    readPwd(    TCchar* section, TCchar* key, String&  val);

  void    deleteString(TCchar* section, TCchar* key);
  void    deleteSection(TCchar* section);

String    getIniPath() {return iniFilePath;}

protected:
  void    checkPath();
  void    setTheAppPath(CWinApp& theApp);
  String  encodePassword(String& password);
  Tchar   getRandCh();
  String  decodePassword(String& cipher);

  friend class IniSectIter;
  };


extern IniFile iniFile;


/* Iterator for section names, use as follows:

   fn() {
   IniSectIter iter(iniFile);
   TCchar*     section;

     for (section = iter(); section; section = iter++) {
       // section is guaranteed to be non-null (not zero)
       }
     }
*/

class IniSectIter {

IniFile& ini;
Tchar*   p;
Tchar*   endBuf;
Tchar*   buf;
int      bufSize;

public:

  IniSectIter(IniFile& iniFile) : ini(iniFile), p(buf), endBuf(buf), buf(0), bufSize(16) { }
 ~IniSectIter() {clear();}


  TCchar* operator() ();
  TCchar* operator++ (int);

private:

  IniSectIter() : ini(*(IniFile*)0) { }
  void clear() {if (buf) {NewArray(Tchar); FreeArray(buf); buf = 0;}}
  };



//  Tchar*  startReadSection();
//  Tchar*  nextSection();
//  Tchar*  getSection() {return p < q ? p : 0;}
//Tchar*   p;
//Tchar*   q;
//Tchar*   buf;
// p(0), q(0), buf(0),
