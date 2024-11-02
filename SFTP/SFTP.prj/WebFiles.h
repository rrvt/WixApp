// Web folder -- Content of one directory at the web site


#pragma once
#include "Expandable.h"
#include "IterT.h"


enum WebItemType {WebNilType, WebDirType, WebFileType};

class WebNode;
class Archive;


class WebItem {
public:
WebItemType typ;
String      name;
int         size;
WebNode*    node;

  WebItem() : typ(WebNilType), size(0), node(0) { }
  WebItem(WebItem& wi) {copy(wi);}
 ~WebItem();


  WebItem& operator= (WebItem& wi)  {copy(wi); return *this;}

  // Required for Insertion Sort, i.e. data = dtm;
  bool operator >= (WebItem& wi) {return name >= wi.name;}
  bool operator == (WebItem& wi) {return name == wi.name;}

  // Required for Binary Search
  bool operator== (TCchar* key) {return this->name == key;}
  bool operator<  (TCchar* key) {return this->name <  key;}
  bool operator>  (TCchar* key) {return this->name >  key;}

private:

  void copy(WebItem& wi);
  };


typedef IterT<WebNode, WebItem> WebNodeIter;


class WebNode {

Expandable<WebItem, 8> data;

public:

String path;

           WebNode() { }
           WebNode(WebNode& wn) {copy(wn);}
          ~WebNode() {clear();}

  void     clear();

  WebNode& operator= (WebNode& wn) {copy(wn); return *this;}

 int       nData()      {return data.end();}                     // returns number of data items in array

private:

  void copy(WebNode& wn) {path = wn.path; data = wn.data;}

  // returns either a pointer to data (or datum) at index i in array or zero

  WebItem* datum(int i) {return 0 <= i && i < nData() ? &data[i] : 0;}       // or data[i].p

  void     removeDatum(int i) {if (0 <= i && i < nData()) data.del(i);}

  friend class    WebFiles;
  friend typename WebNodeIter;
  };


class WebFiles {

WebNode rootNode;
bool    loaded;
int     indent;

public:

           WebFiles() : loaded(false), indent(0) { }
          ~WebFiles() { }

  void     clear() {rootNode.clear();   loaded = false;   indent = 0;}

  void     modified() {loaded = false;}

  void    start();                                    // Start Thread to load web directory into rootNode

  LRESULT finishMsg(WPARAM wparam, LPARAM lParam);    // Finish the Thread and display the directory

  bool     load(TCchar* root);                        // Load directory into rootNode, must called from a
                                                      // thread
  WebNode& root() {return rootNode;}

  void     display();

private:

  bool     isLocked(bool prevent = false);

  bool     loadOne(TCchar* path, WebNode& webNode);
  bool     parse(String& line, TCchar* path, WebItem& tem);

  void     displayOne(WebNode& webNode, int tab);
  };


extern WebFiles webFiles;

