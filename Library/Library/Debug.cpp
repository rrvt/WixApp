// Debug Module for outputing print statements in a Message Box
// rrvt 05/12/2015


#include "pch.h"
#include "Debug.h"
#include "NotePad.h"


Debug dbg;


void Debug::title(String& h) {hdr = h;}
void Debug::title(Tchar*  h) {hdr = h;}


void Debug::stg(String& t) {text = t; messageBox();}
void Debug::stg(Tchar*  t) {text = t; messageBox();}


void Debug::num(String& t,    int n) {text = t; text += (long) n; messageBox();}
void Debug::num(Tchar*  t,    int n) {text = t; text += (long) n; messageBox();}
void Debug::num(String& t, double n) {text = t; text += n; messageBox();}
void Debug::num(Tchar*  t, double n) {text = t; text += n; messageBox();}



#ifdef UglyDebug
void Debug::examine() {
NtPdIter iter(notePad);
Note*    note;

  for (note = iter(); note; note = iter++) {
    examineNode(note);
    }
  }


void Debug::examineNode(Node* n) {
Note* nt = (Note*) n;

  if (!n) return;

  add(n, n->secnd);   ordinal++;
  }
#endif


void Debug::examineTwo(void* n, void* two) {

  if (!n) return;

  add(n, two);  ordinal++;
  }


void Debug::examineBlk(void* n, int sz) {
int st = (int) n;
int en = st + sz;

  if (!n) return;

  add((void*) st, (void*) en);
  }



void Debug::add(void* nd, void* secnd) {
int i;

  if (dbgX >= noElements(dbgData))
    return;

  for (i = 0; i < dbgX; i++) {
    DbgData& dbgD = dbgData[i];

    if (dbgD.node == nd && dbgD.secnd == secnd) return;
    }

  DbgData& dbgD = dbgData[dbgX++];  dbgD.node = nd;  dbgD.secnd = secnd;  dbgD.ordinal = ordinal;

if (ordinal >= 28) {
int x = 1;
}
  }





void Memory::alct(void* b, uint sz, Cchar* f, int ln) {
int      i;
MemData*  md   = findEmpty(b, i);        if (!b) return;
int       p    = (int) b + sz - 1;
void*     e    = (void*) p;
ToUniCode fl(f);
String    file = fl();
int       pos;

  if (n >= noElements(memData)) {
int x = 1;  return;
  }

  if (md && !md->deallocated) {
    int x = 1;
    }

  if (!md) md = &memData[n++];

  pos = file.findLastOf(_T('\\'));  if (pos >= 0) file = file.substr(pos);

  md->beg = b; md->end = e; md->file = file;   md->lineNo = ln;

  md->allocated = true;  md->deallocated = false;
  }


void Memory::dalct(void* b) {
MemData* md = findExact(b);      if (!b) return;
  if (md) {md->deallocated = true; return;}

  int x = 1;
  }


void Memory::alct(HFONT h, TCchar* f, int ln) {
void*    b  = h;              if (!h) return;
int      i;
MemData* md = findEmpty(b, i);

  if (n >= noElements(memData)) {
int x = 1;  return;
  }

  if (md && !md->deallocated) {
    int x = 1;
    }

if (i == 23) {
int x = 1;
}
if (i == 24) {
int x = 1;
}

  if (!md) md = &memData[n++];

  md->beg = b; md->end = 0; md->file = f; md->lineNo = ln;

  md->allocated = true;  md->deallocated = false;
  }


void Memory::dalct(HFONT h) {
void*    b  = h;               if (!h) return;
MemData* md = findExact(b);

  if (md) {md->deallocated = true; return;}

  md = &memData[n++];

  md->beg = b; md->end = (void*) -1; md->allocated = false; md->deallocated = true;
  }



MemData* Memory::findEmpty(void* b, int& i) {
MemData* md;

  if (!b) return 0;

  for (i = 0; i < n; i++) {
    md = &memData[i];

    if ((md->deallocated && md->allocated) || md->beg == b) return md;
    }

  return 0;
  }


MemData* Memory::findExact(void* b) {
int      i;
MemData* md;

  for (i = 0; i < n; i++) {md = &memData[i];  if (md->beg == b) return md;}

  return 0;
  }


