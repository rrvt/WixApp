// Tab Value used in Note


#pragma once


struct NoteTab {
int  pos;                                 // Integer multiple of char width
bool right;                               // Right Tab

  NoteTab() : pos(0), right(false) { }
  NoteTab(int v, bool r = false) : pos(v), right(r) { }

  void     clear() {pos = 0; right = false;}

  bool     isEmpty() {return pos == 0;}

  bool     operator== (NoteTab&   tab) {return pos == tab.pos;}
  bool     operator>= (NoteTab&   tab) {return pos >= tab.pos;}
  NoteTab& operator=  (NoteTab& t)     {pos = t.pos;   right = t.right;   return *this;}

  void     set(int val, bool r = false) {pos = val; right = r;}
  };


