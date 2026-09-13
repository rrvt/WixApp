// Some operations cannot take place at the invocation when the buffer has content that "precedes"
// the operation, i.e. left over from the last np node due to an end of line or end of page.


#include "pch.h"
#include "DeviceBfr.h"
#include "CDCex.h"
#include "DeviceCtx.h"



int DeviceBfr::findNextBrk(int pos) {
int lng  = buf.length();
int next;

  if (pos >= lng) return -1;

  for (next = buf.find(_T(' '), pos); next == 0;  next = buf.find(_T(' '), pos + 1)) continue;

  if (next < 0) next = lng;

  return next > 0 ? next : -1;
  }


int DeviceBfr::getExtent(int pos) {return w = cdcEx.getExtent(buf, pos).cx;}


void DeviceBfr::textOut(DeviceCtx* ctx, int pos) {

  cdcEx.textOut(ctx->x, ctx->y, buf, pos); ctx->setMaxX(ctx->x += w);   buf = buf.substr(pos);

  if (!buf.isEmpty()/* || !defer*/) return;
  }





////////////------------------
//bool DeviceBfr::doBegUnderLine()
//  {if (buf.isEmpty()) return true;      set(BgULDfr);   return false;}

//bool DeviceBfr::doEndUnderLine()
//  {if (buf.isEmpty()) return true;      set(EnULDfr);   return false;}

#if 0
  if (isDeferred(LMgnDfr)) {ctx->finLeftMgn(dfrLMgnV);          clr(LMgnDfr);}
  if (isDeferred(RMgnDfr)) {ctx->finRightMgn(dfrLMgnV);         clr(RMgnDfr);}
  if (isDeferred(FaceDfr)) {ctx->finFace(dfrFaceV);             clr(FaceDfr);   dfrFaceV.clear();}
  if (isDeferred(FSzeDfr)) {ctx->finPoint(dfrFSize);            clr(FSzeDfr);   dfrFSize = 0;}
  if (isDeferred(BoldDfr)) {ctx->finBold();                     clr(BoldDfr);}
  if (isDeferred(ItlcDfr)) {ctx->finItalic();                   clr(ItlcDfr);}
  if (isDeferred(FUnLDfr)) {ctx->finFUnderline();               clr(FUnLDfr);}
  if (isDeferred(StkODfr)) {ctx->finStrikeout();                clr(StkODfr);}
  if (isDeferred(PopFDfr)) {ctx->finPopFont();                  clr(PopFDfr);}
  if (isDeferred(ClTbDfr)) {ctx->finClearTabs();                clr(FUnLDfr);}
  if (isDeferred(StTbDfr)) {ctx->finSetTab(dfrTab);             clr(StkODfr);}
  if (isDeferred(MvTbDfr)) {ctx->finMoveNextTab();              clr(PopFDfr);}
  if (isDeferred(CntrDfr)) {ctx->finCenter();                   clr(PopFDfr);}
  if (isDeferred(RghtDfr)) {ctx->finRight();                    clr(PopFDfr);}

  if (isDeferred(BgULDfr)) {                                    clr(BgULDfr);}

  if (isDeferred(TextDfr))
      {if ((defer & PriorAttr) == 0) {buf = dfrText;   clr(TextDfr);   dfrText.clear();}   return;}

  if (isDeferred(EnULDfr) && buf.isEmpty()) {clr(EnULDfr);}
#endif
#if 0
bool DeviceBfr::doLeftMargin(double v)
  {if (buf.isEmpty()) return true;     set(LMgnDfr);   dfrLMgnV = v;       return false;}

bool DeviceBfr::doRightMargin(double v)
  {if (buf.isEmpty()) return true;      set(RMgnDfr);   dfrRMgnV = v;     return false;}

bool DeviceBfr::doSetFace(TCchar* face)
  {if (buf.isEmpty()) return true;      set(FaceDfr);   dfrFaceV = face;   return false;}

bool DeviceBfr::doSetFSize(int tenths)
  {if (buf.isEmpty()) return true;      set(FSzeDfr);   dfrFSize = tenths;   return false;}

bool DeviceBfr::doBold()
  {if (buf.isEmpty()) return true;      set(BoldDfr);   return false;}

bool DeviceBfr::doItalic()
 {if (buf.isEmpty()) return true;      set(ItlcDfr);   return false;}

bool DeviceBfr::doFUnderline()
{if (buf.isEmpty()) return true;      set(FUnLDfr);   return false;}

bool DeviceBfr::doStrikeout()
{if (buf.isEmpty()) return true;      set(StkODfr);   return false;}

bool DeviceBfr::doPopFont()
{if (buf.isEmpty()) return true;      set(PopFDfr);   return false;}

bool DeviceBfr::doClrTabs()
{if (buf.isEmpty()) return true;      set(ClTbDfr);   return false;}

bool DeviceBfr::dosetTab(NoteTab& tab)
{if (buf.isEmpty()) return true;  set(StTbDfr);  dfrTab = tab;  return false;}

bool DeviceBfr::doMoveNextTab()
  {if (buf.isEmpty()) return true;      set(MvTbDfr);   return false;}

bool DeviceBfr::doCenter()
  {if (buf.isEmpty()) return true;      set(CntrDfr);   return false;}

bool DeviceBfr::doRight()
  {if (buf.isEmpty()) return true;      set(RghtDfr);   return false;}


void DeviceBfr::doText(TCchar* txt)
  {if (buf.isEmpty()) {buf = txt;   return;}   set(TextDfr);    dfrText += txt;}

void DeviceBfr::mergeDfrd() {
  if (!dfrText.isEmpty()) {buf += dfrText;   clr(TextDfr);   dfrText.clear();}
  }
#endif

