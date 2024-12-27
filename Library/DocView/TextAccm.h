// Text Output to device



#pragma once
#include "ClipLine.h"
#include "DevTabs.h"
#include "NoteNmbr.h"
#include "NoteTab.h"
#include "UndrLn.h"
#include "Wrap.h"


class TextAccm : public Wrap {

DevTabs  devTabs;

UndrLn   undrLn;

NoteNmbr nmbr;

double   pos;                     // position each line uses during wrap operation at page

bool     tabSeen;                 // Tab seen
bool     center;                  // center string horizonally in window when true;
bool     right;                   // right align in window when true;
bool     begUndrLn;               // Begin underline
String   sum;                     // Collect strings and numbers appending each to sum
bool     endUndrLn;               // End underline
                                  // boundary
public:

bool     nonBlankLine;            // true when first non blank line is encountered during printing

            TextAccm();
           ~TextAccm() { }

  void      initialize();

  bool      setPrinting(bool on) {return vert.setPrinting(on);}
  bool      setFooter(  bool on) {return vert.setFooter(on);}

  void      dcDim() {horz.setPgWidth(dcWidth());   vert.setPgHeight(dcHeight());}

  void      clear();

  void      copy(TextAccm& tac);

  void      setLeftMargin( double val)  {flush();   DeviceContext::setLeftEdge(val);}
  void      setRightMargin(double val)  {flush();   DeviceContext::setRightEdge(val);}

  void      setFontFace(TCchar* face)   {flush();   setFace(face);   vert.getMaxHeight();}
  void      setFontSize(double  val)    {flush();   setSize(val);    vert.getMaxHeight();}
  void      setFontBold()               {flush();   setBold();       vert.getMaxHeight();}
  void      setFontItalic()             {flush();   setItalic();     vert.getMaxHeight();}
  void      setFontUnderLine()          {flush();   setUnderLine();  vert.getMaxHeight();}
  void      setFontStrikeOut()          {flush();   setStrikeOut();  vert.getMaxHeight();}
  void      popFontAttr()               {flush();   popFont();       vert.getMaxHeight();}

  void      clrTabs()                   {flush();   devTabs.clear();}
  void      setTab(NoteTab& noteTab)    {flush();   devTabs.setTab(horz, noteTab);}
  void      setTabSeen()                {flush();   tabSeen = true;}
  void      setCenter()                 {flush();   center  = true;}
  void      setRight()                  {flush();   right   = true;}

  void      setBeginUnderLine()         {flush();   begUndrLn = true;}

  void      append(TCchar* tc)          {if (*tc) nonBlankLine = true;   sum += tc;}
  void      append(NoteNmbr& n)         {append(n.stg());}

  void      setEndUnderLine()           {flush();   endUndrLn = true;}
  void      crlf();
  void      cr()                        {horz.cr();}
  void      lf()                        {vert.lf();}            //{vert.lf(printing, footer);}

  void      flush(bool newTab = false);

  bool      isWrapFin()                 {return sum.isEmpty();}
  void      cleanupWrap();

private:

  void      write(bool wrapEnabled);
  void      output();
  void      fragmentOut(String& frag);

  friend class DevStream;
  };



///---------------

//                                        if (noteTab.right) devTabs.setRTab(horz, noteTab.pos);
//                                        else               devTabs.setTab( horz, noteTab.pos);
//                                        }
//  void      initBounds() {horz.initialize();}
//  void      vert.getMaxHeight()                 {vert.getMaxHeight();}


