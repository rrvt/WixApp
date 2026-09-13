// There are three contexts for output to the printer: Header, Body and Footer


#include "pch.h"
#include "DisplayCtx.h"
#include "DisplayMgr.h"


DisplayCtx::DisplayCtx(DisplayMgr& displayMgr) : DeviceCtx(displayDC), dm(displayMgr) { }

bool DisplayCtx::isAtLeftMgn()         {return x <= dm.leftMgn;}


void DisplayCtx::setLeftMgn( double v)
           {int lm = displayDC.xActualPixels(v);   if (x == dm.leftMgn) x = lm;   dm.leftMgn = lm;}


void DisplayCtx::setRightMgn(double v)
                       {dm.rightMgn = displayDC.xActualPixels(v);   xEnd = dm.width - dm.rightMgn;}


void DisplayCtx::setMaxX(int x) {if (x > dm.maxX) dm.maxX = x;}


void DisplayCtx::moveNextTab() {
int relPos = x - dm.leftMgn;
int next   = tabs.find(relPos);

  x = next + dm.leftMgn;
  }


// Advances one line

bool DisplayCtx::crlf() {

  drawUnderline();

  x = dm.leftMgn;   dm.setMaxY(y += deltaY);

  if (isUnderlining()) startUnderlining();

  return !isEndPage();
  }



bool DisplayCtx::flush() {

  while (!isBufEmpty()) {

    if (dm.output()) continue;

    buf.trimLeft();   if (!crlf()) return false;;
    }

  return true;
  }


/*
typedef struct tagTEXTMETRICW {
LONG  tmHeight;                LONG  tmWeight;             WCHAR tmBreakChar;
LONG  tmAscent;                LONG  tmOverhang;           BYTE  tmItalic;
LONG  tmDescent;               LONG  tmDigitizedAspectX;   BYTE  tmUnderlined;
LONG  tmInternalLeading;       LONG  tmDigitizedAspectY;   BYTE  tmStruckOut;
LONG  tmExternalLeading;       WCHAR tmFirstChar;          BYTE  tmPitchAndFamily;
LONG  tmAveCharWidth;          WCHAR tmLastChar;           BYTE  tmCharSet;
LONG  tmMaxCharWidth;          WCHAR tmDefaultChar;        } TEXTMETRIC#endif   */

void DisplayCtx::updateMetrics() {

  if (!displayDC.getMetrics(metrics) || !dm.width) return;

  drawUnderline();

  avgCharWidth = metrics.tmAveCharWidth;                  // Used for tab spacing only
  deltaY       = metrics.tmHeight + (metrics.tmExternalLeading ? metrics.tmExternalLeading : 5);
  deltaUL      = metrics.tmHeight;

  dm.setLineHeight(deltaY);   dm.setCharWidth(avgCharWidth);

  if (isUnderlining()) startUnderlining();
  }




