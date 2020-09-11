// Edit Box -- a child of CEdit
// Copyright Software Design & Engineering, Robert R. Van Tuyl, 2013.  All rights reserved.


#pragma once
//#include "Expandable.h"


enum BoxType {NilBox, TimeBox, OrdinalClassBox, OrdinalRideBox, BreakBox};


class EditBox : public CEdit {
bool    created;
BoxType boxType;
String  initialVal;
int     vertPos;

public:
  EditBox() : created(false), boxType(NilBox), vertPos(0) { }
 ~EditBox() {deleteBox();}

  void create(int vPos, RECT& rect, CWnd* view, int id, bool integerOnly = false);

  void set(String& s, BoxType bt) {initialVal = s; boxType = bt;}

  void setFocus() {if (created) {SetFocus(); SetSel(0, -1); SetSel(-1);}}

  int  getVertPos() {return vertPos;}

  void getRect(RECT& rect);

  void getWindowPos(HWND hWnd, int* x, int* y);

  void getLine(String& line);

  void clear() {Clear();}

  void deleteBox() {
    if (created) DestroyWindow(); created = false;
    }

  friend class  EditBoxes;
  friend struct EditBoxP;
  };

