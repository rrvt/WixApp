// Extension of CDialogEx

#pragma once

class  DialogApp;
class  NotePad;
class  PrintNtPd;
class  PrinterAttr;
struct PrinterInfo;
class  PrinterPageAttrDlg;


#define IDC_StatusBar 900       // Includes 901 & 902


class DialogEx : public CDialogEx {

public:

String docName;

                   DialogEx(int id, CWnd* parent);
                  ~DialogEx() { }

          BOOL     OnInitDialog();

  virtual bool     onPreparePrinter(PrinterInfo& info) {return false;}
  virtual NotePad& onPreparePrinting() {return *(NotePad*)0; }

  virtual void     setHeader(PrintNtPd& prntNp) { }
  virtual void     setFooter(PrintNtPd& prntNp) { }

          void     printerSetup();                // Respond to "Setup Printer" command
          void     printFile(bool getNoPages);
          void     printFilePreview() { }

          void     setTitle(TCchar* txt) {SetWindowText(txt);}

  afx_msg void     onSetupDisplayPage();

private:

  void setDfltPrinter();

  DialogEx() { }
  };






//////////---------------

  //  void doPrint(CPrintDialog& dlg);
//  void setDefaultPrinter();
//  void setPrinterPageDefaults(HANDLE hDevMode);

//  void setPrinterPageDlg(PrinterPageAttrDlg& dlg);
//  void getPrinterPageDlg(PrinterPageAttrDlg& dlg);
//#include "PrtrDlg.h"
//#include "StatusBar.h"
//#include "StatusBar.h"
//  int  toHMM(String& s);
//  afx_msg void     onSetupPrintPage();              // Dialog to get printer attributes from user

//  virtual void     setHeader(PrintNtPd& prntNp) { }
//  virtual void     setFooter(PrintNtPd& prntNP) { }

//  virtual void getHdr(NotePad& np, int pageNo) { }

//  virtual void     getHeader(NotePad& np, int pageNo) { }
//  virtual void     getFooter(NotePad& np, int pageNo) { }

