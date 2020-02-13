// Edit Box -- a child of CEdit
// Copyright Software Design & Engineering, Robert R. Van Tuyl, 2013.  All rights reserved.


#pragma once
#include "Expandable.h"


enum BoxType {NilBox, TimeBox, OrdinalClassBox, OrdinalRideBox, BreakBox};


class EditBox {
bool    created;
BoxType boxType;
CEdit*  box;
String  initialVal;
int     vertPos;

public:
  EditBox() : created(false), boxType(NilBox), box(0), vertPos(0) { }
 ~EditBox() {clearBox();}

  void create(int vPos, RECT& rect, CWnd* view, int id, bool integerOnly = false);

  void set(String& s, BoxType bt) {initialVal = s; boxType = bt;}

  void setFocus() {
    if (created) {
      box->SetFocus(); box->SetSel(0, -1); box->SetSel(-1);
      }
    }

  int  getVertPos() {return vertPos;}

  void getRect(RECT& rect);

  void getWindowPos(HWND hWnd, int* x, int* y);

  void getLine(String& line);

  void clearBox() {
    if (created) box->DestroyWindow(); created = false;
    }

  friend class EditBoxes;
  friend struct EditBoxP;
  };


struct EditBoxP {
EditBox* p;
  EditBoxP() {p = new EditBox;}
 ~EditBoxP() {p = 0;}

 EditBoxP(EditBoxP& fd) {copy(fd, *this);}

 EditBoxP& operator= (EditBoxP& fd) {copy(fd, *this); return *this;}

private:

  void copy(EditBoxP& src, EditBoxP& dst) {
    EditBox* q = dst.p;
    EditBox* r = src.p;
    if (!r) {dst.p = 0; return;}

    if (!q) q = dst.p = new EditBox;

    q->created = r->created;
    q->boxType = r->boxType;
    q->box = r->box;
    q->initialVal = r->initialVal;   q->vertPos = r->vertPos;
    }
  };


class EditBoxes {

CWnd*                    view;
Expandable<EditBoxP, 64> boxes;

public:

  EditBoxes() : view(0) {}
 ~EditBoxes() {clear();}

  void    setView(CWnd* v) {view = v;}

  int     get() {return boxes.get();}
  void    create(int vPos, int editBoxX, int x, int y, int width, int height);
  void    set(    int x, String& s, BoxType bt) {if (-1 < x && x < boxes.end()) boxes[x].p->set(s, bt);}
  void    setFocus(int x)              {if (-1 < x && x < boxes.end()) boxes[x].p->setFocus();}
  void    getLine(int x, String& line) {if (-1 < x && x < boxes.end()) boxes[x].p->getLine(line);}
  void    clear(int x)                 {if (-1 < x && x < boxes.end()) boxes[x].p->clearBox();}
  void    clear();
  int     getVertPos(int x)            {return -1 < x && x < boxes.end() ? boxes[x].p->getVertPos() : 0;}
  void    getRect(int x, RECT& rect) {
            if (-1 < x && x < boxes.end()) boxes[x].p->getRect(rect);
            else rect.top = rect.bottom = rect.left = rect.right = 0;
            }
  void    getWindowPos(int x, int* xx, int* yy)
                            {if (-1 < x && x < boxes.end()) boxes[x].p->getWindowPos(getHWND(x), xx, yy);}

  HWND    getHWND(int x) {return -1 < x && x < boxes.end() ? boxes[x].p->box->m_hWnd : 0;}

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
 ~EditBoxX() {if (x > -1) editBoxes.clear(x);}

  operator int() {if (x < 0) get();  return x;}

  void get()   {if (x < 0) x = editBoxes.get();}

  void set(String& initialVal, BoxType bt)   {get(); editBoxes.set(x, initialVal, bt);}

  void setFocus();// {if (x > -1) editBoxes.setFocus(x); docView.scrollToPos();}

  int  getVertPos()                   {return editBoxes.getVertPos(x);}
  void getRect(RECT& rect)            {editBoxes.getRect(x, rect);}
  void getWindowPos(int* xx, int* yy) {editBoxes.getWindowPos(x, xx, yy);}

  void getLine(String& line) {if (x < 0) get(); editBoxes.getLine(x, line);}

  void clear() {if (x >= 0) editBoxes.clear(x);}

  HWND getHWND() {return editBoxes.getHWND(x);}
  };

