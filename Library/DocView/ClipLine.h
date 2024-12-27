// Clip Line using the Mouse Left Button to Select


#pragma once

class DeviceContext;
class VertMgmt;


class ClipLine {

int      savedDC;
bool     opened;
CPoint   point;
int      hzPos;
int      tabSpaces;
COLORREF textColor;
COLORREF bkgdColor;

public:

String clipped;

  ClipLine() : savedDC(0), opened(false), point{-99, -99}, hzPos(0), tabSpaces(0) { }
 ~ClipLine() { }

  void clear();
  void open(DeviceContext& dvx);
  bool isOpen() {return opened;}

  void set(CPoint& pt) {point = pt;}
  void setHzPos(int pos) {hzPos = pos;}
  int  tabWidth(int newPos, int maxChWidth);

  void clipRegion(TCchar* txt, int xPos, int width, VertMgmt& vert, DeviceContext& dvx);

  void close(DeviceContext& dvx);

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

