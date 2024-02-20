// XML Parser -- Data goes into the StoreX (see StoreX.h)


#pragma once
#include "Archive.h"
#include "XmlAttributes.h"

class XmlElement;

enum XMLType {NilXml, CntnXml, SmStrtXml, EnStrtXml, EndTgXml, OneTgXml, DeclXml, EofXml};

enum TagTy   {NilTagTy, SmplTagTy, EnhcTagTy, EndSlsTy, EnhEndSlsTy};


// Used to find all the tags in a line

class Tag {
TCchar* p;

public:

  Tag(String& s) :p(s.str()) { }                    // line on which to work

  XMLType next(String& name);

private:

  Tag() : p(0){ }
  };



class XMLparser {

Archive&      ar;
bool          getNext;
Tchar         tch;
int           cnt;

String        frag;
String        tag;

XmlAttributes attributes;

public:

          XMLparser(Archive& archive) : ar(archive), getNext(true), cnt(0) { }
         ~XMLparser() { }

  void    load(XmlElement& elem);

private:

  XMLType next();
  TagTy   getTag();
  void    getAttributes();
  Tchar   getEntity();
  void    nextChar();//      {if (getNext) {if (!ar.read(tch)) tch = 26;}   getNext = false;}
  void    acceptChar()    {getNext = true;}

          XMLparser() : ar(*(Archive*)0) { }
  };


