// Site Files -- A list of all the web site files (filtered) in the local and remote site tree


#pragma once
#include "Date.h"
#include "ExpandableP.h"
#include "IterT.h"

class CSVLex;
class CSVOut;
class Archive;


enum   SiteFlSts {NilSts, WebPutSts, DifPutSts, GetSts, DelSts, OthSts};

class SiteFileKey {
public:
bool   dir;
String path;

  SiteFileKey() : dir(false) { }
  SiteFileKey(TCchar* pth) {dir = false; path = pth;}
  SiteFileKey(String& s)   {dir = false; path = s;}
  SiteFileKey(SiteFileKey& key) {copy(key);}

  void clear() {dir = false; path.clear();}

  SiteFileKey& operator- (String& s)   {dir = false;   path = s;    return *this;}
  SiteFileKey& operator- (TCchar* pth) {dir = false;   path = pth;  return *this;}

  SiteFileKey& operator= (SiteFileKey& key) {copy(key); return *this;}

  bool         operator== (SiteFileKey& k);
  bool         operator!= (SiteFileKey& k);
  bool         operator<  (SiteFileKey& k);
  bool         operator<= (SiteFileKey& k);

  bool         operator>  (SiteFileKey& k);
  bool         operator>= (SiteFileKey& k);

private:

  void copy(SiteFileKey& key) {dir = key.dir;   path = key.path;}
  };



class SiteFileDsc {
public:

String      name;             // Name of file, <name>.<ext>
SiteFileKey key;
int         size;
Date        date;

SiteFlSts   status;
bool        check;
bool        updated;

  SiteFileDsc() :size(0), key(), date(Date::MinDate), status(NilSts), check(false), updated(false) { }
 ~SiteFileDsc() { }

  SiteFileDsc(SiteFileDsc& siteFile) {copy(siteFile);}                            // copy data
  SiteFileDsc& operator= (SiteFileDsc& siteFile) {copy(siteFile); return *this;}

  // Allows sorted data
  bool   operator== (SiteFileDsc& dsc) {return key == dsc.key;}
  bool   operator!= (SiteFileDsc& dsc) {return key != dsc.key;}

  bool   operator>  (SiteFileDsc& dsc) {return key >  dsc.key;}
  bool   operator>= (SiteFileDsc& dsc) {return key >= dsc.key;}
  bool   operator<  (SiteFileDsc& dsc) {return key <  dsc.key;}
  bool   operator<= (SiteFileDsc& dsc) {return key <= dsc.key;}

  bool operator<  (SiteFileKey& s);             // Required for Binary Search
  bool operator<= (SiteFileKey& s);             // Required for Binary Search
  bool operator== (SiteFileKey& s);
  bool operator!= (SiteFileKey& s);
  bool operator>  (SiteFileKey& s);
  bool operator>= (SiteFileKey& s);

  void   addLclAttr(TCchar* path);

  bool   load(CSVLex& lex);
  bool   loadV1(CSVLex& lex);
  void   save(CSVOut& csvOut);
  void   clrSts() {status = NilSts; check = false;   updated = false;}

  bool   put(           int& noFiles);
  bool   createWebDir(  int& noFiles);
  bool   get(           int& noFiles);
  bool   createLocalDir(int& noFiles);
  bool   del(           int& noFiles);
  bool   removeDir(     int& noFiles);

  void   display();
  void   log();

private:

  void copy(SiteFileDsc& siteFile) {
    name   = siteFile.name;    key   = siteFile.key;
    size   = siteFile.size;    date  = siteFile.date;
    status = siteFile.status;  check = siteFile.check;   updated = siteFile.updated;
    }
  };


class SiteFileDscs;

typedef DatumPtrT<SiteFileDsc, SiteFileKey> SiteFileP;
typedef IterT<SiteFileDscs, SiteFileDsc>    FileDscsIter;          // Iterator Declaration


class SiteFileDscs {

String root;
int    rootLng;
bool   loaded;

ExpandableP<SiteFileDsc, SiteFileKey, SiteFileP, 2> data;             // List of all files in web site (local and remote)

public:
                SiteFileDscs() : rootLng(0), loaded(false) { }
                SiteFileDscs(SiteFileDscs& sf) {copy(sf);}
               ~SiteFileDscs() {clear();}

  void          clear() {root.clear(); rootLng = 0; data.clear();}                 // XXX
  void          clrSts();

  SiteFileDscs& operator= (SiteFileDscs& sf) {copy(sf);   return *this;}

  bool          isEmpty() {return nData() == 0;}

  void          setRoot(TCchar* path) {root = path;   rootLng = root.length();}    // XXX

  bool          loadFromPC();
  SiteFileDsc*  add(SiteFileKey& key);             // Key contains local relative path (rel to localRoot)
  SiteFileDsc*  addFile(TCchar* path);             // full path to either a web file or local file
  SiteFileDsc*  addDir( TCchar* path);

  SiteFileDsc*  add(   SiteFileDsc& uf);
  void          update(SiteFileDsc& uf);
  void          delRcd(SiteFileDsc& uf);

  void          setCheck();
  bool          updateFromPC();

  bool          loadFromCSV(Archive& ar);
  void          saveCSV(Archive& ar);
  void          update(SiteFileDscs& curDscs);
  void          updateDel(SiteFileDscs& curDscs);

  SiteFileDsc*  findDir(TCchar* filePath);
  SiteFileDsc*  find(SiteFileKey& key);      // {return data.bSearch(path);}

//SiteFileDsc* bSearchxx(SiteFileKey& key);   // Debugging

  void          display(TCchar* title);

  int           nData() {return data.end();}                 // returns number of data items in array
  SiteFileDsc*  datum(int i) {return 0 <= i && i < nData() ? data[i].p : 0;}

  void          logSelected(TCchar* title);

private:

  void          copy(SiteFileDscs& sf);

  void          loadOneDir(TCchar* path);

  bool          load(CSVLex& lex);
  bool          loadV1(CSVLex& lex);

  bool          filterFile(TCchar* path);

  void          checkForDel(SiteFileDscs& newDscs);

  void          removeDatum(int i) {if (0 <= i && i < nData()) data.del(i);}

  friend typename FileDscsIter;
  };


extern SiteFileDscs prvFileDscs;
extern SiteFileDscs curFileDscs;




//  void      startFromWeb(TCchar* path);
//  void      fromWeb(TCchar* path);

// returns either a pointer to data (or datum) at index i in array or zero
//extern SiteFileDscs updateFileDscs;

