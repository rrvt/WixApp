// Utility Functions


#pragma once
#include "WixOut.h"


String removeBlnks(String& s);

String cleanName(String& s);

String getWixID(String& id, TCchar* ext);

void   findLastName(String& path, String& parent, String& name);

String combineNames(String& parent, String& name);

inline String getText(CEdit& ctrl) {CString cs; ctrl.GetWindowText(cs); return String(cs);}


String findExeFile(TCchar* name);

String getModulePath();



