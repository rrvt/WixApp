// Invalidate View from anywhere in a Doc/View


#include "LibGlobals.h"
#include "Invalidate.h"


Invalidate invalidate;


void Invalidate::operator() () {
  if (!view) {
    if (!mainFrame) mainFrame = (CFrameWnd*) AfxGetMainWnd();   if (!mainFrame) return;
                    view      = mainFrame->GetActiveView();     if (!view)      return;
    }

  view->Invalidate();
  }

