// ToolBar Button Size Context


#include "pch.h"
#include "TBBtnCtx.h"


void TBBtnCtx::initialize()
           {maxChars = 10; id = noLines = avgWidth = height = winWidth = winHeight = 0;   dirty = false;}


void TBBtnCtx::initialize(int winWdth, int winHght) {
  maxChars = 10; winWidth = winWdth; winHeight = winHght;

  noLines = avgWidth = height = 0; dirty = false;
  }



void TBBtnCtx::setMaxChars(TCchar* txt) {
int lng = txt ? _tcslen(txt) : 0;

  if (lng > maxChars) {maxChars = lng; dirty = true;}
  }


int TBBtnCtx::getHoriz() {
int w = maxChars * avgWidth;

  if (w > winWidth) w = winWidth*3/4;

  return w;
  }


int TBBtnCtx::getVert()  {
int ht = noLines * height;

  if (ht > winHeight) ht = winHeight - 60;

  return ht;
  }


void TBBtnCtx::copy(TBBtnCtx& ctx) {
  id        = ctx.id;
  maxChars  = ctx.maxChars;
  noLines   = ctx.noLines;
  avgWidth  = ctx.avgWidth;
  height    = ctx.height;
  winWidth  = ctx.winWidth;
  winHeight = ctx.winHeight;
  dirty     = ctx.dirty;
  }

