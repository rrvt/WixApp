// Sample Data XmlStore


#pragma once
#include "XmlElement.h"

class XmlStoreIter;


// This is the main XmlStore.  I think of it as a permanent home for the data while the program is
// running.  The store (in this case) is a sort of tree where the top of the tree is elem.  An element
// may have many sub-entities, some of which are Attribures and some are Elements.  When a sub-entity
// is an XmlElement a new tree leaf is created.  A tree node may have many sub-tree nodes.


class XmlStore {

XmlElement elem;

public:

bool   firstAttr;
int    tab;

              XmlStore() : /*mssnNo(0),*/ firstAttr(false) { }
             ~XmlStore() { }

  void        clear() {elem.clear(); }

  void        load( Archive& ar);

  XmlElement* find(    XmlStoreIter& iter, TCchar* tgtTag);
  XmlElement* findNext(XmlStoreIter& iter, TCchar* tgtTag);

  void        remove(TCchar* tag);
  void        removeEmptyElem();
  void        makeOneTags();
  void        consolidate();

  void        startDisplay() {firstAttr = false; tab = 0;}
  void        display(XmlBase& base);

  XmlElement& getTop() {return elem;}

private:

  void     remTag(XmlStoreIter& iter, TCchar* tag, TCchar* endTag);

  void     doTab(int n);
  };


extern XmlStore xmlStore;                                   // Sometimes there only needs to one object


// This allows an iterator to be declared which will traverse the entire tree from top to bottom and left
// to right (i.e. in the order the nodes were created).

class XmlStoreIter {
XmlElement& elem;
int         stkX;
ElIter*     data[16];

public:

  XmlStoreIter(XmlElement& element) : elem(element), stkX(-1) { }
 ~XmlStoreIter();

  XmlBase* operator() ();             // initialize the iterator and return the first node
  XmlBase* operator++ (int);          // increment the iterator and return the next node

  void     remove();                  // Remove the current node

private:

  ElIter* push(XmlElement* element);     // Create and Push the current Expandable Iterator on a stack
  ElIter* pop();                      // Release the current Expandable Iterator and pop an iterator
  };                                  // from the stack



//int     display();
#if 0
#include "Archive.h"
#include "Date.h"
#include "Device.h"
#include "Wrap.h"
#endif
#if 0

//String name;
//Date   dt;
//int    mssnNo;

  void     setName(String& s);
  String   date()           {return dt.getDate();}
  String   time()           {return dt.getTime();}
  int      missionNo();
#endif

//  void    add(String& stg);                               // Parse the data into the record
#if 0
// This is the Datum record.  It can be a complex as necessary.
// Set will accept any reasonable group of entities and then will divide up the input into fields in
// the data.
// Get will return any grouping of data desired from the record, or there might be multiple get methods
// that will retrieve invidual or groups of fields.
// Note, the copy constructor and operator often call a private function that will do all the copying.

class Datum {

String s;
//Wrap   wrp;

public:

  Datum()        { }                                       // Copy constructor: Datum a = b;
  Datum(Datum& d) {s = d.s;}
 ~Datum()        { }

  void    set(String& s) {this->s = s;}
  String& get() {return s;}

//  int     noLines() {return wrp.noLines();}

  Datum&  operator= (Datum& d) {s = d.s; return *this;}   // Copy operator: a = b;

  bool    operator== (Datum& d) {return s == d.s;}
  bool    operator!= (Datum& d) {return s != d.s;}
  bool    operator>  (Datum& d) {return s >  d.s;}
  bool    operator<  (Datum& d) {return s <  d.s;}
  bool    operator>= (Datum& d) {return s >= d.s;}
  bool    operator<= (Datum& d) {return s <= d.s;}
  };
#endif

