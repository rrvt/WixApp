// Scroll View Base Class


#pragma once
#include "DisplayMgr.h"

class  NotePad;
class  DisplayNtPd;
struct PrinterInfo;
class  PrintNtPd;


struct DisplayInfo {
String docName;                 // display name on boarder
};


class ScrollView : public CScrollView {

static int lastPos;

DisplayMgr dm;

String     appName;
String     docName;
int        width{1};
int        length{1};
CRect      winSize;

public:
                   ScrollView(TCchar* name);
                  ~ScrollView() {displayDC.~CDCex();}

  // Calls from MFC into the app
  virtual void     OnInitialUpdate();
  virtual void     OnPrepareDC(CDC* dc, CPrintInfo* info = 0);
  virtual void     OnDraw(     CDC* dc);
  virtual void     OnUpdate(CView* view, LPARAM lParam, CObject* obj);
  virtual BOOL     OnScroll(UINT nScrollCode, UINT pos, BOOL doScroll = TRUE);

  virtual bool     onPreparePrinter(PrinterInfo& info) {return false;}
  virtual NotePad& onPreparePrinting() {return *(NotePad*)0; }

  virtual void     setHeader(PrintNtPd& prntNp) { }
  virtual void     setFooter(PrintNtPd& prntNP) { }

          void     enablePrtWrap() { }
          void     disablePrtWrap() { }

  // Virtual calls that ask the app to supply data

  virtual void     onPrepareDisplay(DisplayInfo& info) { }    // Gather info to apply to display

  virtual NotePad& onPrepareOutput() {return *(NotePad*)0;}   // Create output on a notepad and
                                                              // return it to this infrastructure
  virtual void     setHeader(DisplayNtPd& dsplyNp) { }        // Pass in a font or other attr to
  virtual void     setFooter(DisplayNtPd& dsplyNP) { }        // be used for headers and foooters

          void     setFont(TCchar* face, int tenths) {dm.setFonts(face, tenths);}
          void     enableWrap()  {dm.enableWrap();}
          void     disableWrap() {dm.disableWrap();}

protected:

          void     setScrollSize();
          void     getDocAttr(CSize size) { }

          void     printFile(bool getNoPages);
          void     printFilePreview(bool getNoPages) { }
          void     printerSetup();
  };




////////------------------------

//  virtual void     getHeader(NotePad& np, int pageNo) { }     // Create header and footers in the
//  virtual void     getFooter(NotePad& np, int pageNo) { }     // supplied notepad
#if 0
//  virtual BOOL     OnPreparePrinting(       CPrintInfo* info);
  virtual void     OnPrint(CDC* dc,         CPrintInfo* info);
  virtual void     OnBeginPrinting(CDC* dc, CPrintInfo* info);
  virtual void     OnEndPrinting(  CDC* dc, CPrintInfo* info);
  virtual void     OnEndPrintPreview(CDC* dc, CPrintInfo* info, POINT point, CPreviewView* view);
#endif
//#include "FontStack.h"

