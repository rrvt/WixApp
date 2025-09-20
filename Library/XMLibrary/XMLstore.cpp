// Data Store for XML Groups and Lines


#include "pch.h"
#include "XMLstore.h"
#include "qsort.h"


void XMLstore::load(Archive& ar) {
XMLparser xmlParser(*this);

  xmlParser.input(ar);
  }


void XMLstore::remove(TCchar* tag) {
  root.remove(tag);
  }


void XMLtree::remove(TCchar* tag) {                 //ClCompile
XMLiter  iter(*this);
XMLbase* base;

  for (base = iter(); base; base = iter++) {

    if (base->tag == tag) {iter.remove();   continue;}

    if (base->typ == ElementTag) base->remove(tag);
    }
  }


void XMLtree::remGrp(XMLiter& iter, TCchar* tag, TCchar* endTag) {
XMLbase* base;
bool     isTag;


  notePad << _T("Group ") << tag << nCrlf;

  display();

  for (base = iter++; base; base = iter++) {

    isTag = base->tag == tag;

    switch (base->typ) {
      case AttribTag  :
      case DeclTag    :
      case ElementTag :
      default         : base->display(); break;
      }
    }
  }


void XMLtree::remTag(XMLiter& iter, TCchar* tag, TCchar* endTag) {
XMLbase* base;

  for (base = iter++; base; base = iter++) {
    if (base->tag == endTag) return;
    if (base->tag == tag) iter.remove();
    }
  }


XMLbase* XMLtree::findParent(TCchar* s) {
XMLiter  iter(*this);
XMLbase* p;
XMLbase* rslt;

  rslt = start.find(s);   if (rslt) return parent;
  rslt = end.find(s);     if (rslt) return parent;

  for (p = iter(); p; p = iter++) {
    if (p->typ == ElementTag) {rslt = p->findParent(s);   if (rslt) return rslt;   continue;}

    rslt = p->find(s, this); if (rslt) return rslt;
    }

  return 0;
  }


XMLbase* XMLtree::find(TCchar* s) {
XMLiter  iter(*this);
XMLbase* p;
XMLbase* rslt;

  rslt = start.find(s);   if (rslt) return this;
  rslt = end.find(s);     if (rslt) return this;

  for (p = iter(); p; p = iter++)
    {rslt = p->find(s); if (rslt) return rslt;}

  return 0;
  }


void XMLtree::setSortNames() {
XMLiter  iter(*this);
XMLbase* p;

  if (tag != _T("ItemGroup")) return;

  for (p = iter(); p; p = iter++) p->setKey();
  }


bool XMLleaf::setKey() {
int    begPos;
int    endPos;

int x = line.length();

  endPos = line.findLastOf(_T('"'));
  begPos = line.findLastOf(_T('\\'), endPos);   if (begPos < 0) begPos = line.find(_T('"'));
  begPos++;

  if (begPos < 0 || endPos <= begPos) return false;

  sortKey = line.substr(begPos, endPos-begPos);   return true;
  }


void XMLtree::sort() {
int n = data.end();

  qsort(&data[0], &data[n-1]);
  }


void XMLtree::clear() {

  XMLbase::clear();   parent = 0;   data.clear();

   start.clear(); end.clear();
  }



void XMLstore::reorder(XMLbase* first, XMLbase* second, XMLbase* third) {
#if 1
XMLbase* parent1 = first  ? first->getParent()  : 0;
XMLbase* parent2 = second ? second->getParent() : 0;
XMLbase* parent3 = third  ? third->getParent()  : 0;
XMLbase* parent  = 0;
int      nZero   = 0;

  if (!parent1) nZero++;   if (!parent2) nZero++;   if (!parent3) nZero++;
  if (nZero >= 2) return;

  if (parent1) {
    parent = parent1;
    if (parent2 && parent != parent2) return;
    }
  else if (parent2) parent = parent2;

  if (parent3 && parent != parent3) return;

  parent->reorder(first, second, third);
#else
int n = top.data.end();
int i;

  for (i = 0; i < n; i++) {
    XMLtree* proj = top.getElement(i);

    if (proj && proj->startTag.find(_T("<Project")) >= 0) {
      proj->reorder(first, second, third);  break;
      }
    }
#endif
  }


void XMLtree::reorder(XMLbase* first, XMLbase* second, XMLbase* third) {

XMLiter   iter(*this);
XMLbase* p;
int      state = 0;
int      i;

  for (p = iter(); p; p = iter++) {
    switch (state) {
      case 0  : if (!first || p == first) state++;
                if (p == second || p == third) {i = iter.index();  data[i] = first;  state++;}
                break;
      case 1  : if (!second || p == second) state++;
                if (p == first || p == third)  {i = iter.index();  data[i] = second;  state++;}
                break;
      case 2  : if (!third || p == third) return;
                if (p == first || p == second) {i = iter.index();  data[i] = third;  state++;}
                break;
      default : return;
      }
    }
  }


void XMLtree::output(Archive& ar) {
XMLiter   iter(*this);
XMLbase* p;

  if (!start.isEmpty()) start.output(ar);

  for (p = iter(); p; p = iter++)
    p->output(ar);

  if (!end.isEmpty()) end.output(ar);
  }


void XMLtree::display() {
XMLiter   iter(*this);
XMLbase* p;

  if (!start.isEmpty()) notePad << start.line << nCrlf;

  for (p = iter(); p; p = iter++)
    p->display();

  if (!end.isEmpty()) notePad << end.line << nCrlf;
  }


void XMLtree::displayOne() {
XMLiter   iter(*this);
XMLbase* p;

  notePad << nClrTabs << nSetTab(12) << nSetTab(30) << nSetTab(45) << nCrlf;

  for (p = iter(); p; p = iter++)
    p->displayID();
  }


void XMLbase::dsplyID() {
String s;

  switch (typ) {
    case NilTag     : s = _T("NilTag");       break;
    case DeclTag    : s = _T("DeclTag");      break;
    case ElementTag : s = _T("ElementTag");   break;
    case StartTag   : s = _T("StartTag");     break;
    case EndTag     : s = _T("EndTag");       break;
    case AttribTag  : s = _T("AttribTag");    break;
    }

  notePad << s;

  if (!tag.isEmpty()) notePad << nTab << tag;   else notePad << nTab << _T("   ");
  }



////////////--------------------

#if 0
void XMLbaseP::clear() {

  if (!p) return;

  switch(p->typ) {
    case ElementTag : {NewAlloc(XMLtree); XMLtree* q = (XMLtree*) p;  FreeNode(q);} break;
    case AttribTag  : {NewAlloc(Attrib);  Attrib*  q = (Attrib*)  p;  FreeNode(q);} break;
    }

  p = 0;
  }




XMLtree* XMLtree::getElement(int i) {
XMLbase* p = data[i];

  return p && p->typ == ElementTag ? (XMLtree*) p : 0;
  }
#endif

#if 0
XMLtree& XMLtree::operator= (XMLtree& e) {
int n = data.end();

  typ  = e.typ;   tag = e.tag;   name     = e.name;
  parent   = e.parent;    loopX    = e.loopX;      startTag = e.startTag;
  endTag   = e.endTag;

  for (int i = 0; i < n; i++) data[i] = e.data[i];

  return *this;
  }
#endif

//
#if 0
int      n = data.end();
int      i;
bool     rslt = false;
XMLbase* elem;

  for (i = 0; i < n; i++) {
    XMLbase* p = data[i];    if (!p) return 0;
    XMLtree* el;
    Attrib*  at;

    switch (p->typ) {
      case AttribTag  : at = (Attrib*)  p; if (at->find(s)) return this; break;
      case ElementTag : el = (XMLtree*) p;
                        if (el->startTag.find(s) >= 0) return this;
                        elem = el->find(s);  if (elem) return elem;
                        break;
      }
    }

  return 0;
#endif
#if 0
ElIter   iter(*this);
XMLbase* base;
XMLtree* el;
Attrib*  at;

  if (name != _T("ItemGroup")) return;

  for (base = iter(); base; base = iter++) {
    switch (base->typ) {
      case AttribTag  : at = (Attrib*)  base; at->getFileName(); break;
      case ElementTag : el = (XMLtree*) base; el->getFileName(); break;
      }
    }
#endif
#if 0
int n = data.end();
int i;
  for (i = 0; i < n; i++) {
    XMLbase* p = data[i];    if (!p) return;
    XMLtree* el;
    Attrib*  at;

    switch (p->typ) {
      case AttribTag  : at = (Attrib*)  p; at->display(); break;
      case ElementTag : el = (XMLtree*) p; el->display(); break;
      }
    }
#endif
#if 0
    if (base->typ == ElementTag) {

      if (base->tag == _T("ClCompile") {remTag(iter, tag, base->tag);   continue;}

      base->remove(tag);
      }
#endif          }
#if 0
int n = data.end();
int i;

  for (i = 0; i < n; i++) {
    XMLtree* p = getElement(i);   if (!p) continue;
    if (p == first) break;
    if (p == second || p == third) {data[i].set(first); break;}
    }
  for (i++; i < n; i++) {
    XMLtree* p = getElement(i);   if (!p) continue;
    if (p == second) break;
    if (p == first || p == third) {data[i].set(second); break;}
    }
  for (i++; i < n; i++) {
    XMLtree* p = getElement(i);   if (!p) continue;
    if (p == third) break;
    if (p == first || p == second) {data[i].set(third); break;}
    }
#endif
#if 0

int n = data.end();
int i;

  for (i = 0; i < n; i++) {
    XMLbase* p = data[i];    if (!p) return;
    XMLtree* el;
    Attrib*  at;

    switch (p->typ) {
      case AttribTag  : at = (Attrib*)  p; at->output(ar); break;
      case ElementTag : el = (XMLtree*) p; el->output(ar); break;
      }
    }
#endif

