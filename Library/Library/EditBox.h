// Edit Box -- a child of CEdit
// Copyright Software Design & Engineering, Robert R. Van Tuyl, 2013.  All rights reserved.


#pragma once


enum BoxType {NilBox, TimeBox, OrdinalClassBox, OrdinalRideBox, BreakBox};


class EditBox : public CEdit {
bool    created;
BoxType boxType;
String  initialVal;
int     vertPos;

public:
  EditBox() : created(false), boxType(NilBox), vertPos(0) { }
 ~EditBox() {delBox();}
  EditBox(EditBox& eb) {copy(eb);}

 void     clear() {Clear();}

 EditBox& operator= (EditBox& eb) {copy(eb); return *this;}

  void    create(int vPos, RECT& rect, CWnd* view, int id, bool integerOnly = false);

  void    set(String& s, BoxType bt) {initialVal = s; boxType = bt;}

  void    setFocus(bool home = false);

  int     getVertPos() {return vertPos;}

  void    getRect(RECT& rect);

  void    getWindowPos(HWND hWnd, int* x, int* y);

  void    getLine(String& line);

private:

  void    delBox() {if (created) DestroyWindow(); created = false;}

  void    copy(EditBox& eb);

  friend class EditBoxes;
  };

