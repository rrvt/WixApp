// Wix Data Class


#pragma once
#include "Features.h"


class WixDataDlg;


class WixData {

bool newFileNow;

public:

  WixData() : newFileNow(false) {}
 ~WixData() {}

  void        updatePath(String& s);
  bool        readWixData();              //String& wixPath
  void        writeWixData();
  void        clearAllSections();

  void        newFile(WixDataDlg* dialog);
  void        openFile(WixDataDlg* dialog);
  void        setDefaults(WixDataDlg* dialog);

  TCchar*     getWixPath(String& path);
  void        saveWixPath(TCchar* path);

  bool        validate();
  void        output();

private:

  void prepareUninstalls();
  void copyHelperFile(String& wxsPath, TCchar* fileName);
  };


extern WixData wixData;
