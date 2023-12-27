// Definition of an Attribute and an XmlElement, both based on XmlBase


#pragma once
#include "ExpandableP.h"
#include "IterT.h"
#include "NotePad.h"
#include "XmlAttributes.h"


enum DatumType {NilDtm, ElementDtm, ContentDtm};
enum TagType   {NilTag, CntnTag, SmplTag, EnhcTag, EndTag, ElemTag};


struct XmlBase {

DatumType dtmType;
String    tag;
TagType   tagType;
String    line;

  XmlBase() : dtmType(NilDtm) { }
  XmlBase(XmlBase& bs) {copy(bs);}
 ~XmlBase() {clear();}

  void clear() {dtmType = NilDtm; tagType = NilTag;}

  XmlBase& operator= (XmlBase& bs) {copy(bs); return *this;}

  void     display() {notePad << line << nCrlf;}

private:

  void copy(XmlBase& bs);
  };



struct XmlContent : public XmlBase {

  XmlContent() : XmlBase() { }
  XmlContent(XmlContent& a) {copy(a);}
 ~XmlContent() {}

//virtual void   clear() {line.clear();}

  XmlContent& operator= (XmlContent& a) {copy(a); return *this;}

private:

  void copy(XmlContent& a) {*(XmlBase*) this = (XmlBase&) a;}
  };


typedef DatumPtrT<XmlBase, int> XmlBasePB;


class XmlbaseP : public XmlBasePB {

public:

  XmlbaseP() : XmlBasePB() { }
 ~XmlbaseP() {clear();}

  void clear();
  void del();
  };


class XmlElement;
typedef IterT<XmlElement, XmlBase> ElIter;                   // IterT<XmlElement, XmlBase>


// An XmlElement has a beginning and end one of these three forms
//   * <tag> ... </tag>
//   * <tag>
//       ...
//     </tag>
//   * <tag ... />
//
// In the first two case the first tag is in an XmlElement, say x.  The data is contained in the
// data vector.  Data will by of DatumType DtmAttrib.  Another tag will be of DtmElement type.
// The end tag will be of type DtmEndTag
//
// The third case above will be represented by a DtmElement where the data array is empty.
//
// The actual line will appear in line in at attribute and in tag in an element.  An end tag will
// be an attribute with the type DtmEndTag.

class XmlElement : public XmlBase {

XmlElement* upLink;

ExpandableP<XmlBase, int, XmlbaseP, 2> data;

public:

XmlAttributes attributes;

  XmlElement() : XmlBase(), upLink(0) { }
  XmlElement(XmlElement& el) {copy(el);}
 ~XmlElement() {clear();}

  void        clear();

  XmlElement& operator= (XmlElement& el) {copy(el); return *this;}

  void        addContent(String& frag, String& tg, TagType tagTyp);

  XmlElement& addElement(TagType tagTyp, String& tg, XmlAttributes& attributes, String& frag);

  void        consolidate();

  bool        isEmpty()
    {return (tagType == SmplTag || tagType == EnhcTag) && nData() == 1 && datum(0)->tagType == EndTag;}

  bool        isConsolidatable();
  int         nData()     {return data.end();}                    // returns number of data data in array

  void        removeEndTag(int i) {if (datum(i)->tagType == EndTag) removeDatum(i);}

  XmlElement* parent() {return upLink;}

private:

  void clrData();

  void copy(XmlElement& el);


  // returns either a pointer to data (or datum) at index i in array or zero
  XmlBase* datum(int i) {return 0 <= i && i < nData() ? data[i].p : 0;}

  void     removeDatum(int i);

  friend typename ElIter;
  };




//  void     output(Archive& ar);
/*
//  *p == *q       required for qSort
//  *p >  *q
//  *p <= *q
//  *s =  *p

  // Required for Insertion Sort, i.e. data = dtm;
  bool operator >= (Record& r) {return key >= r.key;}
  bool operator == (Record& r) {return key == r.key;}

  // Required for Binary Search
  bool operator== (TCchar* key) {return this->key == key;}
  bool operator<  (TCchar* key) {return this->key <  key;}
  bool operator>  (TCchar* key) {return this->key >  key;}
*/

//  void   output(Archive& ar) {ar.write(line); ar.crlf();}

//  bool   find(TCchar* s) {return line.find(s) >= 0;}

