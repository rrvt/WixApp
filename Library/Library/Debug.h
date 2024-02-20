// Debug Module for outputing print statements in a Message Box
// rrvt 05/12/2015


#pragma once

class Node;
class Note;


struct DbgData {
int   ordinal;
void* node;
void* secnd;

  DbgData() : node(0), secnd(0), ordinal(0) { }
  };


class Debug {
String hdr;
String text;

int     dbgX;
int     ordinal;
DbgData dbgData[100];

public:

int    count;
bool   printing;

       Debug() : count(0), printing(false), dbgX(0), ordinal(0) {}
      ~Debug() {}

  void title(String& h);
  void title(Tchar*  h);

  void stg(String& t);
  void stg(Tchar*  t);

  void num(String& t, int n);
  void num(Tchar*  t, int n);
  void num(String& t, double n);
  void num(Tchar*  t, double n);

  bool isPrinting()  {return printing;}
  void printingNow() {printing = true;}
  void notPrinting() {printing = false;}

//  void examine();
//  void examineNode(Node* n);
  void examineTwo(void* n, void* two);
  void examineBlk(void* n, int sz);

private:

  void messageBox() {MessageBox(0, text, hdr, S_OK);}

  void add(void* nd, void* lnk);
  };





struct MemData {
void*  beg;
void*  end;
bool   allocated;
bool   deallocated;
int    lineNo;
String file;

  MemData() : beg(0), end(0), allocated(false), deallocated(false) { }
  };


class Memory {
int     n;
MemData memData[40];

public:

  Memory() : n(0) { }

  void alct(void* b, uint sz, Cchar* f, int ln);
  void dalct(void* b);

  void alct(HFONT h, TCchar* f, int ln);
  void dalct(HFONT h);

private:

  MemData* findEmpty(void* b, int& i);
  MemData* findExact(void* b);
  };


extern Memory memory;
extern Debug  dbg;
