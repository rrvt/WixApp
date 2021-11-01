// Linked List Base class
// rrvt 2/20/02
// Copyright Software Design & Engineering, Robert R. Van Tuyl, 2013.  All rights reserved.

// This base class must be first base class in derived class
// Uses only one line in each node and allows appending on end of list and prefixing to
// beginning of list cheaply.

// Computes a pointer to a phantom node of class/struct "type" that contains a link at
// linkaddress


#pragma once
#include "stddef.h"


#define phantomNode(type, link, linkAddress) \
                  ((type*) (void*) ((char*) linkAddress - offsetof(type, link)))


// Base class for manipulating nodes as linked lists, must be first base class!

class Node {

Node* link;

public:

           Node() {link   = 0;}

  // Forces class to be abstract.  User must create a class with this as a public base class:
  //   class OrigCsvData : public Node {...}

  virtual ~Node()         = 0;

  // Create a new node and copy the contents of this node into it.  Return a pointer to the
  // new node.  Abstact virtual function so that user must create the same function in their
  // node class

  virtual  Node* clone()  = 0;

  // Remove node from use returning it to wherever it came from (e.g. free storage)
  // implemented as abstract so that user must create the same function in their node class

  virtual  void  remove() = 0;

  friend class List;
  friend class ListLoop;
  friend class ListIter;
  friend class Debug;
  };


class List {
protected:

Node* head;
Node* tail;

public:

           List() : head(0), tail(0) {initialize();}

           List(const List& h);

  virtual ~List() = 0;

  virtual List& operator=(const List& p);

  // Append to list

  virtual Node* append(Node* node) {node->link = tail->link; tail->link = node; tail = node; return node;}

  // add at beginning of list

  virtual Node* prefix(Node* node)
                                  {if (!head) tail = node;   node->link = head; head = node; return node;}

  // True if list is empty
  bool isEmpty() {return !head;}

  // remove and return first node on list or returns zero if list empty

  virtual Node* removeHead();

  // Remove all nodes from list.  Relies on abstract virtual function remove in Node.

  void removeAll();

protected:

  void initialize() {head = 0; tail = phantomNode(Node, link, &head);}

  friend class ListLoop;
  friend class ListIter;
  };


class ListLoop {
List& lst;
Node* prev;
Node* rover;
bool  nodeRemoved;

public:
           ListLoop(List& list) : lst(list), rover(0), prev(0) {nodeRemoved = false;}
  virtual ~ListLoop() = 0;

 // initialize for scan of list and return first node on the list or zero if at end
 // of list.

  virtual Node* startLoop() {
    prev = phantomNode(Node, link, &lst.head); rover = lst.head; nodeRemoved = false;
    return rover;
    }

  // move to next node on list and return pointer to that node or zero if at end of list

  virtual Node* nextNode() {
    if (!nodeRemoved) {prev = rover; if (rover) rover = rover->link;}

    nodeRemoved = false; return rover;
    }

  // remove and return first node on list or returns zero if list empty and adjust rover and prev

  virtual Node* removeHead();

  // Remove the node returned from startLoop or nextNode from the list and return a
  // pointer to the node or zero if not pointing to a node.

  virtual Node* removeCurNode();
  };




class ListIter {
List& lst;
Node* prev;
Node* rover;
bool  nodeRemoved;

public:
           ListIter(List& list) : lst(list), rover(0), prev(0) {nodeRemoved = false;}
  virtual ~ListIter() { }

 // initialize for scan of list and return first node on the list or zero if at end
 // of list.

  Node* operator() () {
    prev = phantomNode(Node, link, &lst.head); rover = lst.head; nodeRemoved = false;
    return rover;
    }

  // move to next node on list and return pointer to that node or zero if at end of list

  Node* operator++ (int) {
    if (!nodeRemoved) {prev = rover; if (rover) rover = rover->link;}

    nodeRemoved = false; return rover;
    }

  // remove and return first node on list or returns zero if list empty and adjust rover and prev

  virtual Node* removeHead();

  // Remove the node returned from startLoop or nextNode from the list and return a
  // pointer to the node or zero if not pointing to a node.

  virtual Node* removeCurNode();
  };


inline List::~List() {removeAll();}


inline Node::~Node() {link = 0;}


inline ListLoop::~ListLoop() { }
