// Wix Data Class


#pragma once
#include "Features.h"


class WixDataDlg;


class WixData {

bool newFileNow;
bool loadingFileNow;

public:

  WixData() : newFileNow(false) {}
 ~WixData() {}

  bool        readWixData();              //String& wixPath
  void        writeWixData(TCchar* filePath);
  void        clearAllSections();

  void        newFile(WixDataDlg* dialog);
  void        openFile(WixDataDlg* dialog);
  void        setDefaults(WixDataDlg* dialog);

  void        getWxdPath( String& path);
  void        saveWxdPath(TCchar* path);

  bool        validate(bool rptErrors = true);
  void        output();

private:

  void prepareUninstalls();
  void copyHelperFile(String& wxsPath, TCchar* fileName);
  };


extern WixData wixData;
