// Calibrate Display and/or Printer Fonts


#include "pch.h"
#include "CalibDspPrt.h"
#include "CalibDspPrtDlg.h"
#include "CApp.h"
#include "CScrView.h"


void CalibDspPrt::operator() () {
double         scale = cView()->getFontScale(false);
CalibDspPrtDlg dlg;

  dlg.calib = dlg.procdr = -1;

  if (dlg.DoModal() == IDOK) {
    scale = dlg.value;
    bool printing = dlg.calib == 1;

    cView()->setFontScale(printing, scale);
    }
  }

