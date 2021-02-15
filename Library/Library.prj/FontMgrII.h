// font Manager II -- Second attempt at managing fonts in Windows!!!


#pragma once
#include "Expandable.h"
#include "IterT.h"




struct FontAttr {
CDC*   dc;                // Current CDC
String face;              // Current Attributes
double sz;
bool   bold;
bool   italic;
bool   underline;
bool   strikeout;

  FontAttr() : dc(0), sz(0.0), bold(false), italic(false), underline(false), strikeout(0) { }
  FontAttr(FontAttr& d) {copy(d);}
 ~FontAttr() { }

  FontAttr& operator= (FontAttr& d) {copy(d); return *this;}

  void clear();

private:

  void copy(FontAttr& d);
  };


struct FontAttrP {
FontAttr* p;

  FontAttrP()                 {p = 0;}
  FontAttrP(FontAttrP& dataP) {p = dataP.p;}
 ~FontAttrP() { }

  FontAttrP& operator= (FontAttrP& dataP) {
    p = dataP.p;
    return *this;
    }

  FontAttr*  get();
  };


class FontMgrII {

CDC*                     dc;

int                      stkX;
Expandable<FontAttrP, 2> stk;

double                   scale;

public:

  FontMgrII() : stkX(0) {get(stkX);}
 ~FontMgrII();

  void set(    TCchar* face, double fontSize, CDC* dc);
  void setFace(TCchar* face);
  void setSize(double  size);
  void setBold();
  void setItalic();
  void setUnderLine();
  void setStrikeOut();

  bool isFontItalic() {return top()->italic;}

  void      clear() {stkX = 0;}

  void      pop();

  double    getScale() {return scale;}
  void      setScale(double scl) {scale = scl;}

private:

  bool      createFont(FontAttr& attr);
  bool      createPointFont(CDC* dc, TCchar* face, double size, CFont& font);
  bool      select(CFont& font, CDC* dc);

  FontAttr* top()  {return datum(stkX);}
  FontAttr* next();

  FontAttr* get(int i) {return stk[i].get();}

  FontAttr* datum(int i) {return 0 <= i && i < nData() ? stk[i].get() : 0;}

  int       nData()      {return stk.end();}

  void      removeDatum(int i) {if (0 <= i && i < nData()) stk.del(i);}
  };


