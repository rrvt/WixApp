// Clip Board -- load



#include "pch.h"
#include "ClipBoard.h"
#include "MessageBox.h"


bool ClipBoard::load(TCchar* tc) {
ToAnsi  ansi(tc);
size_t  cbStr = ansi.length() + 1;
char*   p;
HGLOBAL hData;

  NewArray(char);   p = AllocArray(cbStr);   if (!p) return false;

  hData = GlobalAlloc(GMEM_MOVEABLE, cbStr);

  memcpy_s(GlobalLock(hData), cbStr, ansi(), cbStr);

  GlobalUnlock(hData);   FreeArray(p);

  if (!OpenClipboard(0)) {messageBox(_T("Cannot open the Clipboard")); return false;}

  if (!EmptyClipboard())
                 {messageBox(_T("Cannot empty the Clipboard"));    CloseClipboard(); return false;}

  if (::SetClipboardData(CF_TEXT, hData) == NULL)
                 {messageBox(_T("Unable to set Clipboard data"));  CloseClipboard(); return false;}

  CloseClipboard();   return true;
  }


