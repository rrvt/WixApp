// Search for File Names given a path
// rrvt - Software Design & Engineering, Inc.
// Modified for Visual C/C++


#include "stdafx.h"
#include "filesrch.h"
#include "trim.h"


// Returns true if any directories are found matching dirwildCard (which may contain wildcards
// characters *?, etc.)  Use getName to retrieve the names one by one into your character array
// All names must be retrieved before find... may be called again.

bool FileSrch::findSubDirs(   TCchar* path, TCchar* dirWildCard) {
  return findAll(true, dirWildCard, path, false);
  }


bool FileSrch::findAllSubDirs(TCchar* path) {
  return findAll(true, 0, path, true);
  }


// Call getName to retrieve full path

bool FileSrch::findFiles(TCchar* path, TCchar* nameWildCard) {
  return findAll(false, nameWildCard, path, false);
  }


// find all files at path

bool FileSrch::findAllFiles(  TCchar* path) {
  return findAll(false, 0, path, true);
  }


// Searches a single directory given by path for files or subdirectories.  No subdirectories are
// searched (i.e. no recursion).  Returns true if any files or directories are found matching
// wildCardName (which may contain wildcards characters *?, etc.)  Use getName to retrieve the full
// path to entities found.  Names are stored in the object and must be retrieved by getName before
// subsequent calls of find... functions.  Returns names of all subdirectories or file when all is
// true (i.e. no matching the wildCardName).

bool FileSrch::findAll(bool directory, TCchar* wildCardName, TCchar* path, bool all) {
String          curPath  = path;     addPathSepCh(curPath);
String          srchPath = curPath;  srchPath += _T("*");
RegExpr         regExpr;
WIN32_FIND_DATA findData;
HANDLE          h;
BOOL            rslt;
NewAlloc(FSNode);

  tgtNames.removeAll();

  if (wildCardName && !regExpr.setWildCardPattern(wildCardName)) return false;

  #ifndef WinXP
    void* oldValue;   Wow64DisableWow64FsRedirection(&oldValue);
  #endif

  h = FindFirstFile(srchPath, &findData);

  for (rslt = h != INVALID_HANDLE_VALUE; rslt; rslt = FindNextFile(h, &findData)) {

    bool isDirectory = (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;

    if (directory) {
      if (isDirectory) {

        if (!_tcscmp(findData.cFileName, _T(".")))  continue;
        if (!_tcscmp(findData.cFileName, _T(".."))) continue;

        if (all || regExpr.match(findData.cFileName)) {
          FSNode* fnd = AllocNode;
          fnd->name = curPath; fnd->name += findData.cFileName; fnd->name += pathSepChar;
          tgtNames.append(fnd);
          }
        }
      }

    else if (!isDirectory) {
      if (all || regExpr.match(findData.cFileName)) {
        FSNode* fnd = AllocNode;
        fnd->name = curPath; fnd->name += findData.cFileName;
        tgtNames.append(fnd);
        }
      }
    }

  FindClose(h);

  #ifndef WinXP
    Wow64RevertWow64FsRedirection(oldValue);
  #endif

  return !tgtNames.isEmpty();
  }


bool FileSrch::getName(String& name) {
FSNode* node = tgtNameLoop.removeHead();

  if (node) {NewAlloc(FSNode); name = node->name; FreeNode(node); return true;}

  return false;
  }


void FileSrch::addPathSepCh(String& path) {
int lng;

  path.trim();  lng = path.length();

  if (path[lng-1] != pathSepChar) path += pathSepChar;
  }


FSNode* FSNode::clone() {NewAlloc(FSNode);  FSNode* node = AllocNode;  node->name = name; return node;}



void FSNode::remove() {NewAlloc(FSNode); FreeNode(this);}

