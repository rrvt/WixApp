// File Data
// Copyright Software Design & Engineering, Robert R. Van Tuyl, 2013.  All rights reserved.

#pragma once

#include "Node.h"
#include "NoteNmbr.h"
#include "NoteTab.h"


enum NoteOrdr {NilNO,         LeftMgnNO,  RightMgnNO,
               FontFaceNO,    FontSizeNO, BoldNO,        ItalicNO, UnderlineFontNO, StrikeOutNO,
               PrevFontNO,    ClrTabsNO,  SetTabNO,
               TabNO,         CenterNO,   RightInLineNO,
               BegUndrlineNO, LineNO,     NmbrNO,         EndUndrlineNO,
               CrlfNO,        EndPageNO,  DebugNO
               };


class Note : public Node {
public:

double   leftMgn;                   // Left Margin in character widths
double   rightMgn;                  // Right Margin in char widths

String   fFace;                     // font Face
double   fSize;                     // Set new font size
bool     bold;                      // Set font to Bold
bool     italic;                    // Set font to italic
bool     underline;                 // Underline font
bool     strikeOut;                 // Strike out font
bool     prevFont;                  // Restore previous font

bool     clrTabs;                   // clear all tabs
NoteTab  noteTab;

bool     tabSeen;                       // move cursor to next tab position
bool     center;                    // center all text to crlf in window
bool     right;                     // right adjust text to crlf in window

bool     beginLine;                 // begin underline at current position

String   line;                      // add text to output after all of the above is performed

NoteNmbr nmbr;

bool     endLine;                   // end underline at current position
bool     crlf;                      // output crlf after text
bool     endPage;                   // end page when printing
bool     debug;

        Note();
        Note(Note& n) {copy(n);}
       ~Note() {}

  Note& operator= (Note& n) {copy(n); return *this;}

  Note* clone();
  Note* alloc() {NewAlloc(Note);   Note* p = AllocNode;   return p;}

  bool  isAfter(NoteOrdr attr);

  void  remove() {NewAlloc(Note); FreeNode(this);}

private:

  void copy(Note& n);
  };


class NoteList : public List {
public:

  NoteList() {};
 ~NoteList() {removeAll();};

  // Copy one list into another calling Note::clone to create a new node and copy data from one
  // node into another.

  NoteList& operator= (const NoteList& p) {this->List::operator=(p); return *this;}

  // Append to list

  virtual Note* append(Note* node) {return (Note*) List::append(node);}

  // add at beginning of list

  virtual Note* prefix(Note* node) {return (Note*) List::prefix(node);}

  // remove and return first node on list or returns zero if list empty

  virtual Note* removeHead() {return (Note*) List::removeHead();}

  void removeAll() {List::removeAll();}

  friend class NoteListLoop;
  };


class NoteListLoop : public ListLoop {

  NoteListLoop(NoteList& list) : ListLoop(list) {}
 ~NoteListLoop() {}

  // initialize for scan of list and return first node on the list or zero if at end
  // of list.
  virtual Note* start() {return (Note*) ListLoop::startLoop();}

  // move to next node on list and return pointer to that node or zero if at end of list
  virtual Note* nextNode() {return (Note*) ListLoop::nextNode();};

  // remove and return first node on list or returns zero if list empty

  virtual Note* removeHead() {return (Note*) ListLoop::removeHead();}

  // Remove the node returned from startLoop or nextNode from the list and return a
  // pointer to the node or zero if not pointing to a node.

  virtual Note* removeCurNode() {return (Note*) ListLoop::removeCurNode();}
  };



