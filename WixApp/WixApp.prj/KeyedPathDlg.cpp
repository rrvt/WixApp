// Keyed Path Descriptor -- Key added to save as default path


#include "pch.h"
#include "KeyedPathDlg.h"


KeyedPathDlg::KeyedPathDlg(TCchar* key,       TCchar* ttl,     TCchar* path,
                           TCchar* extension, TCchar* fltrTtl, TCchar* pat) :
                                      dfltKey(key), PathDlg(ttl, path, extension, fltrTtl, pat) { }

