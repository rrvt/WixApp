// Manage Ini File (A Place where stuff is remembered from execution to execution)
// Copyright Software Design & Engineering, Robert R. Van Tuyl, 2013.  All rights reserved.


#pragma once
#include "Random.h"


class IniFileBase {

Random   rand;

protected:

String   iniFilePath;
int      pathLng;

public:

  IniFileBase() : rand(), pathLng(0) { }
 ~IniFileBase() { }

  String  getAppDataPath(TCchar* helpPath);   // Sets ini File Name & path to appData/Roming
                                              // directory returns path to directory
  void    setPath(TCchar* filePath);          // Sets ini file name & path.
  String  path() {return iniFilePath;}

  virtual void    setAppDataPath(TCchar* helpPath);
  virtual void    setFilePath(String& pth);
  virtual void    setFilePath(TCchar* pth);


  String  initIniDatum(TCchar* section, TCchar* name, TCchar* deflt, String& valu);
  void    saveIniDatum(TCchar* section, TCchar* name, String& valu);
  String  initIniPwd(  TCchar* section, TCchar* name, TCchar* deflt, String& valu);
  void    saveIniPwd(  TCchar* section, TCchar* name, String& valu);

  bool    writeString(TCchar* section, TCchar* key, TCchar*  val);
  bool    write(      TCchar* section, TCchar* key, TCchar*  val);
  bool    writeString(TCchar* section, TCchar* key, CString& val);
  bool    write(      TCchar* section, TCchar* key, CString& val);
  bool    write(      TCchar* section, TCchar* key, int      val)
                                                              {return writeInt(section, key, val);}
  bool    write(      TCchar* section, TCchar* key, bool     val);

  bool    writeInt(   TCchar* section, TCchar* key, int      val);
  bool    write(      TCchar* section, TCchar* key, double   val);
  bool    writePwd(   TCchar* section, TCchar* key, String&  val);
  void    writeEnd();

  bool    readString( TCchar* section, TCchar* key, String&  val);
  bool    read(       TCchar* section, TCchar* key, String&  val)
                                                            {return readString(section, key, val);}
  bool    readString( TCchar* section, TCchar* key, Cstring& val);
  bool    read(       TCchar* section, TCchar* key, Cstring& val)
                                                            {return readString(section, key, val);}
  bool    read(       TCchar* section, TCchar* key, int&     val);            // Defaults to zero

  bool    readString( TCchar* section, TCchar* key, String&  val, TCchar* dflt);
  bool    readString( TCchar* section, TCchar* key, Cstring& val, TCchar* dflt);
  bool    read(       TCchar* section, TCchar* key, String&  val, TCchar* dflt);
  bool    read(       TCchar* section, TCchar* key, Cstring& val, TCchar* dflt);
  bool    read(       TCchar* section, TCchar* key, int&     val, int     dflt);
  bool    read(       TCchar* section, TCchar* key, bool&    val, bool    dflt);
  int     readInt(    TCchar* section, TCchar* key, int      def);
  bool    readPwd(    TCchar* section, TCchar* key, String&  val);

  void    deleteString(TCchar* section, TCchar* key);
  void    deleteSection(TCchar* section);

String    getIniPath() {return iniFilePath;}

protected:
  void    checkPath();
#if 0
  void    setTheAppPath();
  void    clrTheAppPath();
#endif

  String  encodePassword(String& password);
  Tchar   getRandCh();
  String  decodePassword(String& cipher);

private:


  friend class IniSectIter;
  };


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

IniFileBase& ini;
Tchar*       p;
Tchar*       endBuf;
Tchar*       buf;
int          bufSize;

public:

  IniSectIter(IniFileBase& iniFile) : ini(iniFile), p(buf), endBuf(buf), buf(0), bufSize(16) { }
 ~IniSectIter() {clear();}


  TCchar* operator() ();
  TCchar* operator++ (int);

private:

  IniSectIter() : ini(*(IniFileBase*)0), p(0), endBuf(0), buf(0), bufSize(0)     { }
  void clear() {if (buf) {NewArray(Tchar); FreeArray(buf); buf = 0;}}
  };

