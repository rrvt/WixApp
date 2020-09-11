// Edit Box -- a child of CEdit
// Copyright Software Design & Engineering, Robert R. Van Tuyl, 2013.  All rights reserved.


#pragma once
#include "EditBox.h"
#include "Expandable.h"


//enum BoxType {NilBox, TimeBox, OrdinalClassBox, OrdinalRideBox, BreakBox};


struct EditBoxP {
EditBox* p;


  EditBoxP() {NewAlloc(EditBox); p = AllocNode;}
 ~EditBoxP() {NewAlloc(EditBox); FreeNode(p); p = 0;}

  EditBoxP(EditBoxP& fd) {p = fd.p;}

  EditBoxP& operator= (EditBoxP& fd) {p = fd.p; return *this;}
  };


class EditBoxes {

CWnd*                    view;
Expandable<EditBoxP, 64> boxes;

public:

  EditBoxes() : view(0) {}
 ~EditBoxes() {deleteBoxes();}

  void    setView(CWnd* v) {view = v;}

//  int     get() {return boxes.get();}
  void    create(int vPos, int editBoxX, int x, int y, int width, int height);
  void    set(    int x, String& s, BoxType bt) {if (-1 < x && x < boxes.end()) boxes[x].p->set(s, bt);}
  void    setFocus(int x)              {if (-1 < x && x < boxes.end()) boxes[x].p->setFocus();}
  void    getLine(int x, String& line) {if (-1 < x && x < boxes.end()) boxes[x].p->getLine(line);}
  void    deleteBox(int x)                 {if (-1 < x && x < boxes.end()) boxes[x].p->deleteBox();}
  void    deleteBoxes();
  int     getVertPos(int x)            {return -1 < x && x < boxes.end() ? boxes[x].p->getVertPos() : 0;}
  void    getRect(int x, RECT& rect) {
            if (-1 < x && x < boxes.end()) boxes[x].p->getRect(rect);
            else rect.top = rect.bottom = rect.left = rect.right = 0;
            }
  void    getWindowPos(int x, int* xx, int* yy)
                            {if (-1 < x && x < boxes.end()) boxes[x].p->getWindowPos(getHWND(x), xx, yy);}

  HWND    getHWND(int x) {return -1 < x && x < boxes.end() ? boxes[x].p->m_hWnd : 0;}

  String getInitialVal(int x) {
    if (-1 < x && x < boxes.end()) return boxes[x].p->initialVal;
    else return _T("");
    }

  BoxType getBoxType(int x) {return -1 < x && x < boxes.end() ? boxes[x].p->boxType : NilBox;}
  int     endBoxes() {return boxes.end();}
  };


extern EditBoxes editBoxes;


class EditBoxX {
int x;

public:

  EditBoxX() {x = -1; get();}
 ~EditBoxX() {if (x > -1) editBoxes.deleteBox(x);}

  operator int() {if (x < 0) get();  return x;}

  void get()   {if (x < 0) x = 1/*editBoxes.get()*/;}

  void set(String& initialVal, BoxType bt)   {get(); editBoxes.set(x, initialVal, bt);}

  void setFocus();// {if (x > -1) editBoxes.setFocus(x); docView.scrollToPos();}

  int  getVertPos()                   {return editBoxes.getVertPos(x);}
  void getRect(RECT& rect)            {editBoxes.getRect(x, rect);}
  void getWindowPos(int* xx, int* yy) {editBoxes.getWindowPos(x, xx, yy);}

  void getLine(String& line) {if (x < 0) get(); editBoxes.getLine(x, line);}

  void deleteBox() {if (x >= 0) editBoxes.deleteBox(x);}

  HWND getHWND() {return editBoxes.getHWND(x);}
  };

