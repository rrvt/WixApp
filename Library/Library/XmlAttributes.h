// Element Modifiers


#pragma once
#include "Expandable.h"
#include "IterT.h"


struct XmlAttribute {
String name;
String value;

  XmlAttribute() { }
  XmlAttribute(XmlAttribute& mod) {copy(mod);}
 ~XmlAttribute() { }

  void clear() {name.clear(); value.clear();}

  XmlAttribute& operator= (XmlAttribute& mod) {copy(mod); return *this;}

private:

  void copy(XmlAttribute& mod) {name = mod.name; value = mod.value;}
  };


class XmlAttributes;
typedef IterT<XmlAttributes, XmlAttribute> AttrIter;


class XmlAttributes {

Expandable<XmlAttribute, 2> data;

public:

  XmlAttributes() { }
 ~XmlAttributes() { }

  void clear() {data.clear();}
  int  nData() {return data.end();}                // returns number of data items in array

  XmlAttribute* add(String& name);

  XmlAttribute& next() {return data.nextData();}

  XmlAttributes& operator= (XmlAttributes& attr) {copy(attr); return *this;}

private:

  void copy(XmlAttributes& attr);

  // returns either a pointer to data (or datum) at index i in array or zero

  XmlAttribute* datum(int i) {return 0 <= i && i < nData() ? &data[i] : 0;}  // or data[i].p

  void         removeDatum(int i) {if (0 <= i && i < nData()) data.del(i);}

  friend typename AttrIter;
  };

