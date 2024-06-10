// ToolBar Critical Dimensions (Font and Window)


#include "pch.h"
#include "ToolBarDim.h"
#include "ToolBarBase.h"


ToolBarDim toolBarDim;


void ToolBarDim::set(CRect& dlgRect, ToolBarBase* tb) {
CDC* dc;

  if (!tb) return;

  height = dlgRect.bottom - dlgRect.top;   width = dlgRect.right - dlgRect.left;

  maxBtnWidth = (width - 16 * nIcns);   if (nBtns) maxBtnWidth /= nBtns;

  dc = tb->GetDC();   fontDim.set(dc);
  }


void FontDim::set(CDC* dc) {
TEXTMETRIC metric;
POINT      point;

  if (!dc) {width = 9; height = 21; return;}

  dc->GetTextMetrics(&metric);

  point.x = (3 * metric.tmAveCharWidth + metric.tmMaxCharWidth + 2) / 4;
  point.y = metric.tmHeight + metric.tmInternalLeading + metric.tmExternalLeading;

  dc->LPtoDP(&point);   width = point.x;   height = point.y * 1333 / 1000;
  }


int ToolBarDim::getHoriz(int noChars) {
int w = fontDim.getHoriz(noChars);

  return maxBtnWidth ? (w >= maxBtnWidth ? maxBtnWidth : w) : w;
  }

