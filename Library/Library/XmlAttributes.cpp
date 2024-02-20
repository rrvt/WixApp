// Element Modifiers


#include "pch.h"
#include "XmlAttributes.h"


XmlAttribute* XmlAttributes::add(String& name) {
XmlAttribute& attr = next();

  attr.name = name; name.clear(); return &attr;
  }


void XmlAttributes::copy(XmlAttributes& attr) {
int n = attr.nData();
int i;

  for (i = 0; i < n; i++) data[i] = attr.data[i];
  }

