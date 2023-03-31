// Printer Device


#include "pch.h"
#include "DevPrntr.h"


void DevPrntr::prepare(CDC* dc, CPrintInfo* pInfo) {

  dvx.set(dc);   pInfo->m_bContinuePrinting = printing = true;   initTxt();

  dvx.dcDim();   /*initPageSize();*/   setBeginPage();
  }


bool DevPrntr::doEndPageChk() {

  if (!vert.isEndPage()) return false;

  vert.atEndPageCond();  return true;
  }




