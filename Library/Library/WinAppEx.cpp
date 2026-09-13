// WinAppEx -- provides common data for Dialog and DocView Apps


#include "pch.h"
#include "WinAppEx.h"


WinAppEx* winAppEx;


WinAppEx::WinAppEx(WinAppEx* app) : CWinAppEx() {winAppEx = app;}


WinAppEx::~WinAppEx() {
  if (hdlDevMode)  {GlobalFree(hdlDevMode);    hdlDevMode  = 0;}
  if (hdlDevNames) {GlobalFree(hdlDevNames);   hdlDevNames = 0;}

#ifdef DebugMemoryLeaks
  _CrtDumpMemoryLeaks();
#endif
  }


BOOL WinAppEx::InitInstance()
                         {bool rslt = CWinAppEx::InitInstance();   winAppEx = this;   return rslt;}


void WinAppEx::setDevMode(HANDLE& h)
                               {if (hdlDevMode) GlobalFree(hdlDevMode);   hdlDevMode = h;   h = 0;}


void WinAppEx::setDevNames(HANDLE& h)
                            {if (hdlDevNames) GlobalFree(hdlDevNames);   hdlDevNames = h;   h = 0;}





////////////--------------------

#if 0
void WinAppEx::setDfltDevMode(PrintDialog& dlg) {
PRINTDLG printDlg;

if (m_hDevMode) {dlg.m_pdex.hDevMode = m_hDevMode;   dlg.m_pdex.hDevNames = m_hDevNames;   return;}

  // 2. Ensure application's printer settings are initialized
  GetPrinterDeviceDefaults(&printDlg);

  if (printDlg.hDevMode)
            {dlg.m_pdex.hDevMode = printDlg.hDevMode;   dlg.m_pdex.hDevNames = printDlg.hDevNames;}
  }
#endif
