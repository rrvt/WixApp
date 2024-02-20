// Edit Box
// Copyright Software Design & Engineering, Robert R. Van Tuyl, 2013.  All rights reserved.


#include "pch.h"
#include "EditBoxes.h"


EditBoxes editBoxes;


void EditBoxes::create(int vPos, int editBoxX, int x, int y, int width, int height) {
CPoint scl       = ((CScrollView*)view)->GetScrollPosition();
int    yScrolled = y - scl.y;

CRect  rect(x, yScrolled, x+width, yScrolled+height);

  boxes.getData(editBoxX).create(vPos, rect, view, editBoxX);
  }

