// Collect CDC data for debugging


#include "stdafx.h"
#include "DCdata.h"
#include "Display.h"


void DCdata::operator() (String& s, Display& dev, bool inDev) {

  if (!dev.printing || s.substr(0,3) != _T("06 ")) return;

  cs = s;  dc = dev.getDC();

  if (dc) {
    dc->GetClipBox(&clip);
    dc->GetTextMetrics(&metric);
    textExt   = dc->GetTextExtent(cs);
    textWidth = textExt.cx;
    spacing   = dc->GetTextCharacterExtra();
    viewPt    = dc->GetViewportExt();
    wndExt    = dc->GetWindowExt();
    }

  clipWidth   = clip.right - clip.left;
  attr        = dev.getCurFont();
  logFont     = dev.getLogFont();

  tPos        = dev.getTextPos();
  width       = tPos.getPortWidth();
  remaining   = tPos.remaining() + (inDev ? tPos.widthCh() : 0);

  x           = 1;
  }

