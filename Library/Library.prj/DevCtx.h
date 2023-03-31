// Device Context with all the details


#pragma once
#include "FontMgr.h"

struct DevCtxBkp;


class DevCtx {

CDC*    dc;

bool    suppress;                              // Suppress output when true;

FontMgr font;

public:

double  scale;                                 // Font Scale (just need one of these)

int     pgWidth;
double  rightMgn;
double  leftMgn;

int     pgHeight;
double  topMgn;
double  botMgn;

double  avgFlChWidth;
double  avgLgChWidth;                           // Character Width
double  edgeChWidth;                            // Average character width used to determine page margins

int     chHeight;                               // Text height
int     uLineDelta;                             // Y delta from y for the line option

           DevCtx();
          ~DevCtx();

  void     set(CDC* pDC)         {dc    = pDC;   if (dc) dc->SetMapMode(MM_TEXT);}
  CDC*     get()                 {return dc;}

  void     dcDim() {pgWidth = dc->GetDeviceCaps(HORZRES); pgHeight = dc->GetDeviceCaps(VERTRES);}
  void     wdwDim();

  int      width(TCchar* tc);

  int      lgChWidth() {return int(avgLgChWidth + 0.5);}

  void     txtOut(TCchar* tc, int horzPos, int vertPos);
  void     line(POINT* points, int& n);

  // Font Manipulation of CDC

  void     setBaseFont(TCchar* face, double size) {font.setBase(*this, face, size);   setMetric();}
  bool     isFontItalic() {return font.isFontItalic();}
  void     setFace(TCchar* face) {font.setFace(face);    setMetric();}
  void     setSize(double  size) {font.setSize(size);    setMetric();}
  void     setBold()             {font.setBold();        setMetric();}
  void     setItalic()           {font.setItalic();      setMetric();}
  void     setUnderLine()        {font.setUnderLine();   setMetric();}
  void     setStrikeOut()        {font.setStrikeOut();   setMetric();}
  void     pop()                 {font.pop();            setMetric();}
  void     restoreFontCtx()      {font.restoreContext(); setMetric();}

  bool     getLogFont(TCchar* face, double sz, LOGFONT& logFont);
  bool     createFont(LOGFONT& logFont, CFont& font);
  bool     select(CFont& font);

  COLORREF getBkColor()  {return dc->GetBkColor();}                     // Background color
  COLORREF getTxtColor() {return dc->GetTextColor();}                   // Text Color

  void     setBkColor( COLORREF color) {dc->SetBkColor(color);}         // Background color
  void     setTxtColor(COLORREF color) {dc->SetTextColor(color);}       // Text Color

  void     saveDvx(DevCtxBkp& bkp);
  void     restoreDvx(DevCtxBkp& bkp);

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

  friend class DevBase;
  friend class ClipLine;
  };


struct DevCtxBkp {
bool    suppress;
FontMgr font;
double  scale;

int     pgWidth;
double  rightMgn;
double  leftMgn;

int     pgHeight;
double  topMgn;
double  botMgn;

double  avgFlChWidth;
double  avgLgChWidth;                           // Character Width
double  edgeChWidth;                            // Character Width

int     chHeight;                               // Text height
int     uLineDelta;                             // Y delta from y for the line option

  DevCtxBkp() { }
 ~DevCtxBkp();
  };



