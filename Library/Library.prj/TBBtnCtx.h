// ToolBar Button Size Context


#pragma once


struct TBBtnCtx {

uint id;                  // Button Command ID

int  maxChars;
int  noLines;

int  avgWidth;            // Avg Char width
int  height;              // height of a line
int  winWidth;            // Window width & height in pixels
int  winHeight;

bool dirty;

  TBBtnCtx() {initialize();}
  TBBtnCtx(TBBtnCtx& ctx) {copy(ctx);}

  void      initialize();
  void      initialize(int winWdth, int winHght);

  TBBtnCtx& operator= (TBBtnCtx& ctx) {copy(ctx); return *this;}

  bool      operator>= (TBBtnCtx& ctx) {return id >= ctx.id;}
  bool      operator== (TBBtnCtx& ctx) {return id == ctx.id;}

  int       getHoriz();
  int       getVert();

  void      setMaxChars(TCchar* txt);
  void      incNoLines() {noLines++; dirty = true;}

private:

  void      copy(TBBtnCtx& ctx);
  };

