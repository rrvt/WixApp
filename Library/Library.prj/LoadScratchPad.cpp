// Load Scratch Pad


#include "stdafx.h"
#include "LoadScratchPad.h"


bool loadScratchPad(String& s) {
int     n = s.length();
int     i;
char*   p;
size_t  cbStr;
HGLOBAL hData;

  cbStr = n + 1;

  NewArray(char);   p = AllocArray(cbStr);   if (!p) return false;

  for (i = 0; i < n; i++) p[i] = (char) s[i];    p[i] = 0;

  hData = GlobalAlloc(GMEM_MOVEABLE, cbStr);

  memcpy_s(GlobalLock(hData), cbStr, p, cbStr);

  GlobalUnlock(hData);   FreeArray(p);

  if (!OpenClipboard(0)) {AfxMessageBox(_T("Cannot open the Clipboard")); return false;}

  if (!EmptyClipboard())
                       {AfxMessageBox(_T("Cannot empty the Clipboard")); CloseClipboard(); return false;}

  if (::SetClipboardData(CF_TEXT, hData) == NULL)
                    {AfxMessageBox(_T("Unable to set Clipboard data"));  CloseClipboard(); return false;}

  CloseClipboard();   return true;
  }

