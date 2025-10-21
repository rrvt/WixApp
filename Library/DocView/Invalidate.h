// Invalidate View from anywhere in a Doc/View


#pragma once

class Invalidate {

CFrameWnd* mainFrame;
CView*     view;

public:

  Invalidate() : mainFrame(0), view(0) { }
 ~Invalidate() { }

  void operator() ();
  };


extern Invalidate invalidate;
