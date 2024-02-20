// Definition of an Attribute and an XmlElement, both based on XmlBase


#include "pch.h"
#include "XmlElement.h"


void XmlElement::addContent(String& frag, String& tg, TagType tagTyp) {
NewAlloc(XmlContent);  XmlContent* p = AllocNode;

  p->dtmType = ContentDtm;

  p->line = frag;  p->tag = tg; p->tagType = tagTyp; data += p;
  }


XmlElement&  XmlElement::addElement(TagType tagTyp, String& tg, XmlAttributes& attributes, String& frag) {
NewAlloc(XmlElement);  XmlElement* p = AllocNode;

  p->dtmType = ElementDtm; p->upLink = this;

  p->tagType = tagTyp; p->tag = tg; p->attributes = attributes; p->line = frag; data += p; return *p;
  }


bool XmlElement::isConsolidatable() {
int lng;
int n = nData();
int i;

  if (n > 2) return false;

  lng = line.length();

  for (i = 0; i < n; i++) {
    XmlBase& p = *data[i].p;

    if (p.dtmType != ContentDtm) return false;

    lng += p.line.length();
    }

  return lng <= 100;
  }


void XmlElement::consolidate() {
ElIter   iter(*this);
XmlBase* base;

  for (base = iter(); base; base = iter++) line += base->line;

  tagType = ElemTag;   clrData();
  }


void XmlElement::clear() {
  clrData();

  upLink = 0;

  ((XmlBase*)this)->clear();
  }


void XmlElement::clrData() {
int n;

  for (n = nData(); n; n = nData()) removeDatum(n-1);
  }


void XmlElement::removeDatum(int i) {
int n = nData();
int j;

  if (i < 0 || n <= i) return;

  data[i].clear();

  for (j = i + 1; j < n; j++, i++) data[i].p = data[j].p;

  data.decrementEndN();
  }


// The free node included in the template doesn't work for multiple node sizes.  Leaves lost memory

void XmlbaseP::clear() {

  if (!p) return;

  switch(p->dtmType) {
    case ElementDtm : {NewAlloc(XmlElement);  XmlElement* q = (XmlElement*) p;  FreeNode(q);} break;
    case ContentDtm : {NewAlloc(XmlContent);  XmlContent* q = (XmlContent*) p;  FreeNode(q);} break;
    }

  p = 0;
  }


void XmlElement::copy(XmlElement& el) {
int n = nData();
int i;

  *(XmlBase*) this = (XmlBase&) el;

  upLink = el.upLink;

  for (i = 0; i < n; i++) {el.data[i].p = data[i].p;  data[i].p = 0;}
  }


void XmlBase::copy(XmlBase& bs) {tag = bs.tag; tagType = bs.tagType; line = bs.line;}


