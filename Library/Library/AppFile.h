// AppFile Object -- find and and/or execute an application


#pragma once
#include "Expandable.h"
#include "IterT.h"


class AppFile;
typedef IterT<AppFile, String> AppIter;


class AppFile {
Expandable<String, 2> data;

String exePath;

public:

  AppFile(TCchar* startPath) {loadFolders(startPath);}
 ~AppFile() { }

  void    loadFolders(TCchar* startPath);

  String& find(TCchar* regExpr);          // Find app in "startPath*"
  String& path() {return exePath;}

private:

  // returns either a pointer to datum at index i in array or zero

  String* datum(int i) {return 0 <= i && i < nData() ? &data[i] : 0;}

  int     nData()      {return data.end();}         // Returns number of data items in array,
                                                    // not necessarily private
  void    removeDatum(int i) {if (0 <= i && i < nData()) data.del(i);}

  friend typename AppIter;
  };

