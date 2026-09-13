// FontStack


#pragma once
#include "ExpandableP.h"

class CDCex;


// One Font

class FontItem : public CFont {

CFont* oldFont{0};

String face;                        // Font attributes
int    tenthsPt{0};
bool   bold{false};
bool   italic{false};
bool   underline{false};
bool   strikeout{false};

public:

           FontItem() { }
          ~FontItem();

  void     set(TCchar* face, int tenthsPt);     // tenthsPt, eg. 12.0 pt is 120
  bool     create(CDCex& cdcEx);

  FontItem& operator= (FontItem& f) {copy(f);  return *this;}

  bool     isItalic()     {return italic;}

  void     getItem(FontItem* f);
  void     setFace(TCchar* face)  {this->face     = face;}
  void     setPoint(int tenthsPt) {this->tenthsPt = tenthsPt;}
  void     setBold()              {bold           = true;}
  void     setItalic()            {italic         = true;}
  void     setUnderline()         {underline      = true;}
  void     setStrikeout()         {strikeout      = true;}

  TCchar*  getFace() {return face;}

private:

  void     copy(FontItem& f);
  friend class FontStack;
  };


typedef DatumPtrT<FontItem, int> DisplayFontItemP;


// Stack of FontStack

class FontStack {

CDCex& cdcEx;

FontItem*                                       top{0}; // Top of stack after push/pop
ExpandableP<FontItem, int, DisplayFontItemP, 2> data;   // Stack except for top

FontItem defaultFont;

public:

           FontStack(CDCex& cdcEX) : cdcEx(cdcEX) { }
          ~FontStack() {popAll();}

  void     setDefault(TCchar* face, int tenthsPt);
  void     initialize();

  bool     create(TCchar* face, int tenthsPt)
                                  {push();   top->set(face, tenthsPt);   return create();}
  bool     setFace(TCchar* face)  {push();   top->setFace(face);         return create();}
  bool     setPoint(int tenthsPt) {push();   top->setPoint(tenthsPt);    return create();}
  bool     setBold()              {push();   top->setBold();             return create();}
  bool     setItalic()            {push();   top->setItalic();           return create();}
  bool     setUnderline()         {push();   top->setUnderline();        return create();}
  bool     setStrikeout()         {push();   top->setStrikeout();        return create();}

  bool     create() {return top->create(cdcEx);}

  void     pop();
  void     popAll();

  FontItem* getTop() {return top;}

private:

  void     push();
  };






///////////--------------

  // zero or -1 and the argument is ignored, true indicates item selected, false indicates item
  // turned off (underline, strikeout)
//  void     setFont(TCchar* face, int tenthsPt, int bold, int italic, int underline, int strikeout);

