// Device Context with all the details


#pragma once
#include "FontMgr.h"
#include "Horiz.h"
#include "VertMgmt.h"


class DeviceContext {

CDC*     dc;

bool     suppress;                         // Suppress output when true;

public:

FontMgr  fontMgr;                 // Keep track of font changes
Horiz    horz;                    // Manage horizontal movement in window or page
VertMgmt vert;                    // Manage vertical movement in window or page

           DeviceContext();
          ~DeviceContext();

  void     clear();
  void     restore(DeviceContext& devCtx) {vert = devCtx.vert;   fontMgr.restore();}

  void     copy(DeviceContext& dvc);

  void     setDC(CDC* pDC) {dc    = pDC;   fontMgr.setDC(dc);   if (dc) dc->SetMapMode(MM_TEXT);}
  CDC*     get()           {return dc;}

  void     initHoriz(double left, double right) {horz.initialize(left, right);}
  void     setLeftEdge( double val)             {horz.setLeftEdge( val);}
  void     setRightEdge(double val)             {horz.setRightEdge(val);}
  int      dcWidth()                            {return dc->GetDeviceCaps(HORZRES);}
  int      dcHeight()                           {return dc->GetDeviceCaps(VERTRES);}
  void     wdwDim();

  int      width(TCchar* tc);

  int      widthInt() {return int(horz.charWidth + 0.5);}

  void     txtOut(TCchar* tc, int horzPos, int vertPos);
  void     line(POINT* points, int& n);

  // Font Manipulation of CDC

  void     setBaseFont(TCchar* face, double size) {fontMgr.setBase(dc, face, size);   setMetric();}
  bool     isFontItalic() {return fontMgr.isFontItalic();}
  void     setFace(TCchar* face) {fontMgr.setFace(face);    setMetric();}
  void     setSize(double  size) {fontMgr.setSize(size);    setMetric();}
  void     setBold()             {fontMgr.setBold();        setMetric();}
  void     setItalic()           {fontMgr.setItalic();      setMetric();}
  void     setUnderLine()        {fontMgr.setUnderLine();   setMetric();}
  void     setStrikeOut()        {fontMgr.setStrikeOut();   setMetric();}
  void     popFont()             {fontMgr.pop();            setMetric();}

  void     restoreFontCtx()      {fontMgr.restore(); setMetric();}

  COLORREF getBkColor()  {return dc->GetBkColor();}                     // Background color
  COLORREF getTxtColor() {return dc->GetTextColor();}                   // Text Color

  void     setBkColor( COLORREF color) {dc->SetBkColor(color);}         // Background color
  void     setTxtColor(COLORREF color) {dc->SetTextColor(color);}       // Text Color

/*
typedef struct tagLOGFONTW {
  LONG  lfHeight;
  LONG  lfWidth;
  LONG  lfEscapement;
  LONG  lfOrientation;
  LONG  lfWeight;
  BYTE  lfItalic;
  BYTE  lfUnderline;
  BYTE  lfStrikeOut;
  BYTE  lfCharSet;
  BYTE  lfOutPrecision;
  BYTE  lfClipPrecision;
  BYTE  lfQuality;
  BYTE  lfPitchAndFamily;
  WCHAR lfFaceName[LF_FACESIZE];
} LOGFONTW, *PLOGFONTW, *NPLOGFONTW, *LPLOGFONTW;
*/

  void     examineCurFont(TCchar* tc);
  bool     getCurFont(LOGFONT& logFont);

private:

  void     setMetric() {setAvgLgChWidth(); setHeight();}
  void     setAvgLgChWidth();
  void     setHeight();
  void     outError(TCchar* stg);

  friend class Device;
  friend class ClipLine;
  };



////--------------------------------

