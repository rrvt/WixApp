// find File somewhere in a file tree


#pragma once
#include "Expandable.h"



// Searches for a directory or file recursively in all subdirectories of the startPath that match the
// dirWildCard (which can contain file system wild card chararcters: '*', '?', etc.).  Attempts to find
// a specific tgtWildCard name (either a directory or file name).

// The number found is returned by nofound (which can trigger a boolean statment).

// StartLoop and nextPath may be used to scan the names found, for example:
// String* path;
// String  successPath[100];
//
//   for (path = filePaths.startLoop(), i = 0; path; path = nextPath(), i++) {
//     successPath[i] = *path;        // or whatever one needs to do (this not being a good example)
//     }
//
// clear will initialize the internal data structures so that another find... may be called.  The inherent
// recursion prevents

class FilePaths {
int                    i;
int                    nPaths;
Expandable<String, 16> paths;

public:

  FilePaths() : i(0), nPaths(0) { }
 ~FilePaths() { }

  void    clear() {for (i = 0; i < nPaths; i++) paths[i].clear();  i = nPaths = 0;}

  bool    findDirs( TCchar* startPath, TCchar* dirWildCard, TCchar* tgtWildCard);

  bool    findFiles(TCchar* startPath, TCchar* tgtWildCard);

  bool    findFiles(TCchar* startPath, TCchar* dirWildCard, TCchar* tgtWildCard);

  int     noFound() {return nPaths;}

  String* startLoop() {i = 0; return nextPath();}

  String* nextPath() {return i < nPaths ? &paths[i++] : 0;}
  };

