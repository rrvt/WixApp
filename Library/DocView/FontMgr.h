// font Manager II -- Second attempt at managing fonts in Windows!!!


#pragma once
#include "ExpandableP.h"
#include "IterT.h"


struct FontAttr {
CDC*           dc;
String         face;              // Current Attributes
double         scaledSize;        // Scale * font Size in points
bool           bold;
bool           italic;
bool           underline;
bool           strikeout;

  FontAttr() : dc(0), scaledSize(10000),
                                 bold(false), italic(false), underline(false), strikeout(false) { }
 ~FontAttr() { }

  FontAttr& operator= (FontAttr& d) {copy(d); return *this;}

  void clear();

  void init(CDC* cdc, TCchar* name, double scsz);

  void setFace(TCchar* face)       {this->face      = face;   createFont();}
  void setScaledSize(double  scsz) {scaledSize      = scsz;   createFont();}
  void setBold()                   {this->bold      = true;   createFont();}
  void setItalic()                 {this->italic    = true;   createFont();}
  void setUnderLine()              {this->underline = true;   createFont();}
  void setStrikeOut()              {this->strikeout = true;   createFont();}

  bool createFont();

private:

  bool getLogFont(LOGFONT& logFont);
  bool createFont(LOGFONT& logFont, CFont& font);
  bool select(CFont& font);

  void copy(FontAttr& d);
  };


typedef DatumPtrT<FontAttr, String> FontAttrP;            // Usually defined just before iterator


class FontMgr {
CDC*                                        dc;
FontAttr*                                   cur;
int                                         stkX;
ExpandableP<FontAttr, String, FontAttrP, 8> data;

public:

double                                      scale;        // Font Scale (just need one of these)

  FontMgr();                                              // : dc(0), cur(0), stkX(-1), scale(0) { }
 ~FontMgr();

  void      setDC(CDC* cdc)      {dc    = cdc;}

  void      clear() {dc = 0;   cur = 0;   stkX = -1;  data.clear();   scale = 0;}

  FontMgr&  operator= (FontMgr& fm) {copy(fm); return *this;}
  void      restore()               {if (cur) cur->createFont();}


  void      setBase(CDC* cdc, TCchar* face, double fontSize);
  void      setFace(TCchar* face) {FontAttr& attr = *next();   attr.setFace(face);}
  void      setSize(double size)  {FontAttr& attr = *next();   attr.setScaledSize(size * scale);}
  void      setBold()             {FontAttr& attr = *next();   attr.setBold();}
  void      setItalic()           {FontAttr& attr = *next();   attr.setItalic();}
  void      setUnderLine()        {FontAttr& attr = *next();   attr.setUnderLine();}
  void      setStrikeOut()        {FontAttr& attr = *next();   attr.setStrikeOut();}
  void      pop();
  void      restoreContext()      {if (cur) cur->createFont();}

  bool      isFontItalic() {return cur ? cur->italic : false;}

private:

  FontAttr* next();

  void      copy(FontMgr& fm);
  FontAttr* getAttr(int i);

  // returns either a pointer to data (or datum) at index i in array or zero

  FontAttr* datum(int i) {return 0 <= i && i < nData() ? data[i].p : 0;}

  int       nData()      {return data.end();}             // returns number of data items in array

  void      removeDatum(int i) {if (0 <= i && i < nData()) data.del(i);}
  };




