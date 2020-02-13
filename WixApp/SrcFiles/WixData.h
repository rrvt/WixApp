// Wix Data Class


#pragma once
#include "Feature.h"


class WixDataDlg;


class WixData {

public:

  WixData() {}
 ~WixData() {}

  bool        readWixData( );              //String& wixPath
  void        writeWixData(String& wxsPath);
  void        clearAllSections();
  bool        validate();
  void        output();

private:

  void prepareUninstalls();
  void copyHelperFile(String& wxsPath, TCchar* fileName);
  void finalMsg();
  };


extern WixData wixData;
