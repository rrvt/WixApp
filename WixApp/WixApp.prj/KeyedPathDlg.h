// Keyed Path Descriptor -- Key added to save as default path



#pragma once
#include "PathDlg.h"


class KeyedPathDlg : public PathDlg {
public:

String dfltKey;

  KeyedPathDlg(TCchar* key, TCchar* ttl, TCchar* path,
                                                  TCchar* extension, TCchar* fltrTtl, TCchar* pat);
  KeyedPathDlg(KeyedPathDlg& dlg) {copy(dlg);}
 ~KeyedPathDlg() { }

  KeyedPathDlg& operator= (KeyedPathDlg& dlg) {copy(dlg); return *this;}

private:

  void copy(KeyedPathDlg& dlg) {PathDlg::copy(dlg); dfltKey = dlg.dfltKey;}

  KeyedPathDlg() { }
  };



