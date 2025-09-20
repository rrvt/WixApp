// Data XMLstore for Project Groups and Lines


#pragma once
#include "Archive.h"
#include "ExpandableP.h"
#include "IterT.h"
#include "NotePad.h"
#include "XMLparser.h"

#include "MessageBox.h"


class XMLbase {
public:

XMLType typ;
String  tag;
String  sortKey;

           XMLbase()           : typ(NilTag) { }
           XMLbase(XMLType t)  : typ(t) { }
           XMLbase(XMLbase& x) {copy(x);}
  virtual ~XMLbase() {typ = NilTag;}

  virtual void     clear() {typ = NilTag; tag.clear();}

          XMLbase& operator= (XMLbase& x) {copy(x);   return *this;}

  virtual bool     isPresent(TCchar* tc)             {return false;}

  virtual XMLbase* find(TCchar* tc)                  {return 0;}
  virtual XMLbase* findParent(TCchar* s)             {return 0;}
  virtual XMLbase* find(TCchar* tc, XMLbase* parent) {return 0;}

  virtual void     remove(TCchar* tag) { }

  virtual void     setSortNames() { }
  virtual XMLbase* getParent() {return 0;}
  virtual void     reorder(XMLbase* first, XMLbase* second, XMLbase* third) { }

  virtual void     add(XMLbase* p) { }

  virtual void     display() { }

  virtual void     displayOne() { }
  virtual void     displayID() { }
          void     dsplyID();

  virtual void     output(Archive& ar) { }

  virtual bool     setKey() {return false;}
  virtual void     sort()   { }

  bool     operator== (XMLbase& x) {return _tcsicmp(sortKey, x.sortKey) == 0;}
  bool     operator>= (XMLbase& x) {return _tcsicmp(sortKey, x.sortKey) >= 0;}
  bool     operator<= (XMLbase& x) {return _tcsicmp(sortKey, x.sortKey) <= 0;}
  bool     operator>  (XMLbase& x) {return _tcsicmp(sortKey, x.sortKey) >  0;}

protected:

  void copy(XMLbase& x) {sortKey = x.sortKey;   typ = x.typ;   tag = x.tag;}
  };



class XMLleaf : public XMLbase {
public:

String line;

           XMLleaf()           : XMLbase(AttribTag) { }
           XMLleaf(XMLleaf& a) {copy(a);}
          ~XMLleaf()  {clear();}

  void     clear() {XMLbase::clear();   line.clear();}

  XMLleaf& operator= (XMLleaf& a) {copy(a); return *this;}

  bool     isEmpty() {return line.isEmpty();}

  bool     isPresent(TCchar* s) {return line.find(s) >= 0;}

  XMLbase* find(TCchar* tc)                  {return line.find(tc) >= 0 ? this   : 0;}
  XMLbase* find(TCchar* tc, XMLbase* parent) {return find(tc)           ? parent : 0;}

  bool     setKey();

  void     display()        {notePad << line << nCrlf;}

  void     output(Archive& ar) {ar   << line << aCrlf;}

private:

  void     copy(XMLleaf& a) {XMLbase::copy(a);   line = a.line;}
  };


class XMLtree;
typedef DatumPtrT<     XMLbase, String>              XMLbaseP;
typedef ExpandableP<   XMLbase, String, XMLbaseP, 2> XMLData;
typedef IterT<XMLtree, XMLbase>                      XMLiter;


class XMLtree : public XMLbase {

XMLtree* parent;                                // Parent of this tree
XMLData  data;                                  // A list of leafs and trees

public:

XMLleaf  start;                                 // BeginTag ("<tagName>") of element
XMLleaf  end;                                   // EndTag   ("</tagName>") of element
                                                // or <tagName /> or <tagName> ... </tagName>
  XMLtree() : XMLbase(ElementTag), parent(0) { }
  XMLtree(XMLtree& d) {copy(d);}
 ~XMLtree() {clear();}

  void     clear();

  void     setParent(XMLtree* p) {parent = p;}

  void     add(XMLbase* p) {data += p;}

  void     display();
  void     displayOne();            // Debugging?

  XMLtree* getParent() {return parent;}
  void     reorder(XMLbase* first, XMLbase* second, XMLbase* third);

  void     remove(TCchar* tag);
  void     remTag(XMLiter& iter, TCchar* tag, TCchar* endTag);
  void     remGrp(XMLiter& iter, TCchar* tag, TCchar* endTag);

  XMLbase* find(TCchar* s);
  XMLbase* findParent(TCchar* s);

  void     setSortNames();
  bool     setKey() {return true;}
  void     sort();

  XMLtree* getElement(int i);

  void     output(Archive& ar);

private:

  // returns either a pointer to data (or datum) at index i in array or zero
  XMLbase* datum(int i) {return 0 <= i && i < nData() ? data[i] : 0;}

  // returns number of data data in array
  int      nData()      {return data.end();}

  void     removeDatum(int i) {if (0 <= i && i < nData()) data.del(i);}

  friend typename XMLiter;
  };


class XMLstore {
XMLtree  root;                                // Header of tree
XMLtree* cur;                                   // current XMLtree

public:

String xmlVersion;

           XMLstore() : cur(&root) { }
          ~XMLstore() { }

  void     clear() {root.clear();   cur = &root;   root.typ = NilTag;   root.tag = _T("Header");}

  void     load(Archive& ar);

  XMLtree* getCurrent()   {return cur;}

  XMLtree* getElement()   {NewAlloc(XMLtree);   return AllocNode;}
  XMLleaf* getAttrib()    {NewAlloc(XMLleaf);    return AllocNode;}

  void     add(XMLleaf* p) {cur->add(p);}
  void     add(XMLtree* p) {cur->add(p);   p->setParent(cur);   cur = p;}

  void     remove(TCchar* tag);
  void     makeOneTags() { }
  void     consolidate() { }

  void     setParent() {XMLtree* parent = cur->getParent();   cur = parent ? parent : &root;}

  XMLbase* find(TCchar* s)       {return root.find(s);}
  XMLbase* findParent(TCchar* s) {return root.findParent(s);}

  void     setSortNames() {root.setSortNames();}
  void     sort()         {root.sort();}

  void     reorder(XMLbase* first, XMLbase* second, XMLbase* third);

  void     display()           {root.display();}

  void     output(Archive& ar) {root.output(ar);}
  };


////---------------

//  XMLtree* parent(XMLtree* e) {return e->parent();}
#if 0
  void     addAttrib(String& s) {
             NewAlloc(XMLleaf);  XMLleaf* p = AllocNode;   p->line = s;  data += p;
             }

  XMLtree* addElement() {
             NewAlloc(XMLtree);  XMLtree* p = AllocNode;
             p->typ = ElementTag; p->parent = this; data += p;  return p;
             }
#endif

//  XMLtree& operator= (XMLtree& d);
#if 0
  void     addAttrib(XMLtree* e, String& s)  {return e ? e->addAttrib(s) : root.addAttrib(s);}
  XMLtree* addElement(XMLtree* e)            {return e ? e->addElement() : root.addElement();}
#endif
//  void     set(TCchar* id, XMLtree* parent) {tag = id;   this->parent = parent;}
//  virtual bool     setKey() {return false;}
//          bool     setKey(String& line);

//  bool     setKey()         {return XMLbase::setKey(line);}

//  void     set(TCchar* name, TCchar* tag) {this->name = name;   startTag = tag;}
//  void     setEndTag(TCchar* tag) {endTag = tag;}
//  bool     setKey() {return XMLbase::setKey(startTag);}

