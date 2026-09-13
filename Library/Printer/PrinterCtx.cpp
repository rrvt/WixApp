// There are three contexts for output to the printer: Header, Body and Footer


#include "pch.h"
#include "PrinterCtx.h"
#include "PrinterMgr.h"


PrinterCtx::PrinterCtx(PrinterMgr& prtrMgr) : DeviceCtx(printerDC), pm(prtrMgr) { }

bool PrinterCtx::isAtLeftMgn() {return x <= pm.leftMgn;}


void PrinterCtx::setLeftMgn( double v) {
int lm = printerDC.xActualPixels(v);

  if (x == pm.leftMgn) x = lm;   pm.leftMgn = lm;
  }


void PrinterCtx::setRightMgn(double v)
                       {pm.rightMgn = printerDC.xActualPixels(v);   xEnd = pm.width - pm.rightMgn;}



void PrinterCtx::startUnderlining() {if (!pm.suppress) underlineText.start(x, y + deltaUL);}


void PrinterCtx::moveNextTab() {
int relPos = x - pm.leftMgn;
int next   = tabs.find(relPos);

  x = next + pm.leftMgn;
  }




// Advances one line and returns true if another line is allowed in page

bool PrinterCtx::crlf() {

  drawUnderline();

  x = pm.leftMgn;   y += deltaY;

  if (isUnderlining()) startUnderlining();

  return !isEndPage();
  }


bool PrinterCtx::flush() {

  while (!isBufEmpty()) {

    if (pm.output()) continue;

    buf.trimLeft();   if (!crlf()) return false;
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

void PrinterCtx::updateMetrics() {

  if (!printerDC.getMetrics(metrics) || !pm.width) return;

  drawUnderline();

  avgCharWidth = metrics.tmAveCharWidth;                  // Used for tab spacing only
  deltaY       = metrics.tmHeight + (metrics.tmExternalLeading ? metrics.tmExternalLeading : 5);
  deltaUL      = metrics.tmHeight;

  pm.setLineHeight(deltaY);   pm.setCharWidth(avgCharWidth);

  if (isUnderlining()) startUnderlining();
  }




//////////--------------
//  pm.setFtrOffset();

