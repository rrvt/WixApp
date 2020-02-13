// Is App Available


#pragma once
#include "Expandable.h"


class SoftwareDesc {
public:

String  name;
String  dspName;
TCchar* root;

  SoftwareDesc() : root(0) {}
  SoftwareDesc(SoftwareDesc& fd) {copy(fd, *this);}
  SoftwareDesc& operator=(SoftwareDesc& fd) {copy(fd, *this); return *this;}
private:

  void copy(SoftwareDesc& src, SoftwareDesc& dst)
      {dst.name = src.name; dst.dspName = src.dspName; dst.root = src.root;}
  };



class InstalledSoftware {

int                          nDescs;
Expandable<SoftwareDesc, 16> descs;
HKEY                         lclMachKey;
HKEY                         appKey;

int                          index;                 // Find parameters
String                       findName;

public:

  InstalledSoftware() : nDescs(0), lclMachKey(0), appKey(0), index(0) {}
  void init();
  void init(TCchar* root);

  bool find(TCchar* appName, SoftwareDesc*& desc);
  bool findNext(SoftwareDesc*& desc);

  bool delKey(SoftwareDesc& desc);

private:

  bool getName(int i, String& name);
  bool queryApp(TCchar* query, String& dspName);
  };


//extern InstalledSoftware installedSoftware;
