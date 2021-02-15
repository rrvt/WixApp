// Show Manager -- Parent class for DsplyMgr and PrintMgr


#pragma once
#include "stdafx.h"
#include "ShowMgr.h"


ShowMgr::ShowMgr(TCchar* src, CScrView& view, NotePad& notePad) : id(src),           vw(view),
                                                                  npd(notePad),      dspDev(src, notePad),
                                                                  font(_T("Arial")), fontSize(12.0),
                                                                  leftMargin(0.33),  rightMargin(0.33),
                                                                  topMargin(0.33),   botMargin(0.33) { }




void ShowMgr::setMgns(double left, double top, double right, double bot)
                            {leftMargin = left; topMargin = top;  rightMargin = right;  botMargin = bot;}

