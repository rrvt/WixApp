// Clip Line using the Mouse Left Button to Select


#pragma once


class ClipLine {

CDC*     dc{0};
int      savedDC{0};
bool     opened{false};
CPoint   point{-99, -99};
int      hzPos{0};
int      tabSpaces{0};
COLORREF textColor;
COLORREF bkgdColor;

public:

String clipped;

  ClipLine() { }
 ~ClipLine() { }

  void clear();
  void open(CDC* cdc);
  bool isOpen() {return opened;}

  void set(CPoint& pt) {point = pt;}
  void setHzPos(int pos) {hzPos = pos;}
  int  tabWidth(int newPos, int maxChWidth);

  void clipRegion(TCchar* txt, int xPos, int width, CDC* cdc);

  void close(CDC* cdc);

  bool load();                                    // Load into Windows Clip Board

private:

  void     add(TCchar* txt);

  COLORREF invert(COLORREF c);

  void save(CDC* dc);
  void restore(CDC* dc);
  };


extern ClipLine clipLine;




////============

//bool loadClipBoard(TCchar* s);

