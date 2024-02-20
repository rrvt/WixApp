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

