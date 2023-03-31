// Sample Data XmlStore


#include "pch.h"
#include "XmlStore.h"
#include "Xmlparser.h"


XmlStore xmlStore;                                        // Global since all classes need access


// Load is called from serialize in the Document class
// The basic idea is that the method reads some chunk from the file (Archive is a fileio surrogate)

void XmlStore::load(Archive& ar) {XMLparser parser(ar);  parser.load(elem);}


// NilTag, CntnTag, SmplTag, EnhcTag, EndTag, ElemTag

XmlElement* XmlStore::find(XmlStoreIter& iter, TCchar* tgtTag) {
XmlBase*     base;

  for (base = iter(); base; base = iter++)
    switch (base->tagType) {
      case NilTag :
      case EndTag :
      case CntnTag: continue;
      case SmplTag:
      case EnhcTag:
      case ElemTag: if (base->tag == tgtTag) return (XmlElement*) base;
                    break;
      }

  return 0;
  }


XmlElement* XmlStore::findNext(XmlStoreIter& iter, TCchar* tgtTag) {
XmlBase*     base;

  for (base = iter++; base; base = iter++)
    switch (base->tagType) {
      case NilTag :
      case EndTag :
      case CntnTag: continue;
      case SmplTag:
      case EnhcTag:
      case ElemTag: if (base->tag == tgtTag) return (XmlElement*) base;
                    break;
      }

  return 0;
  }



void XmlStore::removeEmptyElem() {
XmlStoreIter iter(elem);
XmlBase*     base;

  for (base = iter(); base; base = iter++)
    if (base->dtmType == ElementDtm) {
      XmlElement& elem = *(XmlElement*) base;

      if (elem.isEmpty()) iter.remove();
      }
  }


void XmlStore::makeOneTags() {
XmlStoreIter iter(elem);
XmlBase*     base;

  for (base = iter(); base; base = iter++)
    if (base->dtmType == ElementDtm) {
      XmlElement& elem = *(XmlElement*) base;

      if (elem.isEmpty()) {
        String s = base->line;
        int    n = s.length();

        base->line = s.substr(0, n-1) + _T(" />");   base->tagType = ElemTag;   elem.removeEndTag(0);
        }
      }
  }


void XmlStore::consolidate() {
XmlStoreIter iter(elem);
XmlBase*     base;

  for (base = iter(); base; base = iter++)
    if (base->dtmType == ElementDtm)
                  {XmlElement& elem = *(XmlElement*) base;   if (elem.isConsolidatable()) elem.consolidate();}
  }


void XmlStore::remove(TCchar* tag) {                 //ClCompile
XmlStoreIter iter(elem);
XmlBase*     base;

  for (base = iter(); base; base = iter++) {

    if (base->tag == _T("ClCompile") && base->tagType == EnhcTag) {
      remTag(iter, tag, base->tag);
      }
    }
  }


void XmlStore::remTag(XmlStoreIter& iter, TCchar* tag, TCchar* endTag) {
XmlBase* base;

  for (base = iter++; base; base = iter++) {
    if (base->tag == endTag) return;
    if (base->tag == tag) iter.remove();
    }
  }


void XmlStore::display(XmlBase& base) {

  if (firstAttr)              tab++;
  if (base.tagType == EndTag) tab--;

  doTab(tab);   base.display();

  firstAttr = base.dtmType == ElementDtm && base.tagType != ElemTag ? true : false;
  }


static String spaces = _T("                                                                        ");

void XmlStore::doTab(int n) {notePad << spaces.substr(0, n*2);}


XmlStoreIter::~XmlStoreIter() {while (pop()) continue;}


XmlBase* XmlStoreIter::operator() () {
  stkX = -1; ElIter& itr = *push(&elem);   return itr();
  }


XmlBase* XmlStoreIter::operator++ (int) {
ElIter&  it   = *data[stkX];
XmlBase* base = it.current();

  if (base && base->dtmType == ElementDtm) {
    XmlElement* elem = (XmlElement*) base;
    if (elem->nData()) {ElIter* itr = push(elem);  return (*itr)();}
    }

  base = it++;   if (base) return base;

  while (stkX >= 0) {
    ElIter* itr = pop();   if (!itr) continue;

    base = (*itr)++;   if (base) return base;
    }

  return 0;
  }


void XmlStoreIter::remove() {ElIter& it = *data[stkX];   it.remove();}



ElIter* XmlStoreIter::push(XmlElement* element) {
NewAlloc(ElIter);

  if (++stkX < noElements(data))
    data[stkX] = AllocNodeI1(*element);

  return data[stkX];
  }


ElIter* XmlStoreIter::pop() {

  if (stkX < 0) return 0;

  NewAlloc(ElIter);   FreeNode(data[stkX]);  stkX--;  return stkX >= 0 ? data[stkX] : 0;
  }

