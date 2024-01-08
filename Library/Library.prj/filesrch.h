


#pragma once
#include "Node.h"
#include "RegExpr.h"



class FSNode : public Node {
public:

String name;

  FSNode() {}
 ~FSNode() {}

  FSNode* clone();

  void     remove();
  };


class FSNameList : public List {
public:

  FSNameList() {};
 ~FSNameList() {};

  // Copy one list into another calling FSNode::clone to create a new node and copy data from one
  // node into another.

  FSNameList& operator =(const FSNameList& p) {this->List::operator=(p); return *this;}

  // Append to list

  virtual FSNode* append(FSNode* node) {return (FSNode*) List::append(node);}

  // add at beginning of list

  virtual FSNode* prefix(FSNode* node) {return (FSNode*) List::prefix(node);}

  // remove and return first node on list or returns zero if list empty

  virtual FSNode* removeHead() {return (FSNode*) List::removeHead();}

  void removeAll() {List::removeAll();}

  friend class FSNameListLoop;
  };


class FSNameListLoop : public ListLoop {
public:

  FSNameListLoop(FSNameList& list) : ListLoop(list) {}
 ~FSNameListLoop() {}

  // initialize for scan of list and return first node on the list or zero if at end
  // of list.
  virtual FSNode* start() {return (FSNode*) ListLoop::startLoop();}

  // move to next node on list and return pointer to that node or zero if at end of list
  virtual FSNode* nextNode() {return (FSNode*) ListLoop::nextNode();};

  // remove and return first node on list or returns zero if list empty

  virtual FSNode* removeHead() {return (FSNode*) ListLoop::removeHead();}

  // Remove the node returned from startLoop or nextNode from the list and return a
  // pointer to the node or zero if not pointing to a node.

  virtual FSNode* removeCurNode() {return (FSNode*) ListLoop::removeCurNode();}
  };


class FileSrch {

FSNameList     tgtNames;
FSNameListLoop tgtNameLoop;

public:

  FileSrch() : tgtNames(), tgtNameLoop(tgtNames) {}
 ~FileSrch() {tgtNames.removeAll();}

  // Searches a single directory given by path for files or subdirectories.  No subdirectories are
  // searched (i.e. no recursion).  Returns true if any files or directories are found matching
  // dirwildCard (which may contain wildcards characters *?, etc.)  Use getName to retrieve the full
  // path to entities found.  Names are stored in the object and must be retrieved by getName before
  // subsequent calls of find... functions.

  bool findSubDirs(   TCchar* path, TCchar* dirWildCard);
  bool findAllSubDirs(TCchar* path);

  bool findFiles(     TCchar* path, TCchar* nameWildCard);      // find files that match nameWC at path
  bool findAllFiles(  TCchar* path);                            // find all files at path

  bool  getName(String& path);                                  // Returns path to full file path

private:

//  bool  findAllDirectories(     TCchar* dirName,  TCchar* path, bool subDirs);
//  bool  findAllFiles(           TCchar* fileName, TCchar* path, bool subDirs);

  bool  findAll(bool directory, TCchar* name,     TCchar* path, bool all);

  void addPathSepCh(String& path);
  };

