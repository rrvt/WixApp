// Linked List Base class
// rrvt 2/20/02
// Copyright Software Design & Engineering, Robert R. Van Tuyl, 2013.  All rights reserved.

// This base class must be first base class in derived class
// Uses only one line in each node and allows appending on end of list and prefixing to
// beginning of list cheaply.

// Computes a pointer to a phantom node of class/struct "type" that contains a link at
// linkaddress

#include "stdafx.h"
#include "node.h"



List::List(const List& h) {
Node* node;

  initialize();

  for (node = h.head; node; node = node->link) append(node->clone());
  }


List& List::operator=(const List& h) {
Node* node;

  if (&h != this) {
    removeAll();
    for (node = h.head; node; node = node->link) append(node->clone());
    }

  return *this;
  }



Node* List::removeHead() {
Node* node = head;

  if (head) {head = head->link;  if (!head) tail = phantomNode(Node, link, &head);}

  if (node) node->link = 0;

  return node;
  }


Node* ListLoop::removeHead() {
Node* node = lst.head;

  if (lst.head)
    {lst.head = lst.head->link;  if (!lst.head) lst.tail = phantomNode(Node, link, &lst.head);}

  if      (prev  == node)         {prev = 0; rover = 0;}
  else if (rover == node && prev) rover = prev->link;

  if (node) node->link = 0;

  return node;
  }



Node* ListLoop::removeCurNode() {
Node* node = rover;

  if (prev) {
    if (rover) {prev->link = rover->link; rover = prev->link;}
    else       {lst.tail = prev; prev->link = 0; prev = 0; rover = 0;}
    }

  if (node) {node->link = 0; nodeRemoved = true;}

  return node;
  }


void List::removeAll() {
Node* node;

  while (node = removeHead()) node->remove();
  }





Node* ListIter::removeHead() {
Node* node = lst.head;

  if (lst.head)
    {lst.head = lst.head->link;  if (!lst.head) lst.tail = phantomNode(Node, link, &lst.head);}

  if      (prev  == node)         {prev = 0; rover = 0;}
  else if (rover == node && prev) rover = prev->link;

  if (node) node->link = 0;

  return node;
  }



Node* ListIter::removeCurNode() {
Node* node = rover;

  if (prev) {
    if (rover) {prev->link = rover->link; rover = prev->link;}
    else       {lst.tail = prev; prev->link = 0; prev = 0; rover = 0;}
    }

  if (node) {node->link = 0; nodeRemoved = true;}

  return node;
  }

