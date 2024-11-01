// Wix Data Class


#pragma once
#include "Features.h"


class WixAppDlg;


class WixData {

bool   isNewProject;
bool   loadingFileNow;



public:

       WixData() : isNewProject(false), loadingFileNow(false) {}
      ~WixData() {}

  bool newProject(WixAppDlg* dialog);
  void openProject(WixAppDlg* dialog);

  void setDefaults(WixAppDlg* dialog);

  bool validate(bool rptErrors = true);
  void outputProduct();
  void outputWxd();

private:

  bool readWixData();
  bool readWxdPath(String& path);
  void saveWxdPath(TCchar* path);
  void delWxdPath();

  void clearAllSections();
  void prepareUninstalls();
  void copyHelperFile(TCchar* wxsPath, TCchar* fileName);
  };


extern WixData wixData;
