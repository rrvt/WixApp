// find File somewhere in a file tree


#pragma once
#include "Expandable.h"
#include "IterT.h"


class FilePaths;
typedef IterT<FilePaths, String> FPsIter;                        // Iterator for the StoreX


// Searches for a directory or file recursively in all subdirectories of the startPath that match the
// dirWildCard (which can contain file system wild card chararcters: '*', '?', etc.).  Attempts to find
// a specific tgtWildCard name (either a directory or file name).

// The number found is returned by nofound (which can trigger a boolean statment).

// A FPsIter object may be used to scan the names found, for example:
//
//   FPsIter iter(filePaths);
//   String* path;
//   String  successPath[100];
//
//     for (path = iter(), i = 0; path; path = iter++, i++) {
//       successPath[i] = *path;        // or whatever one needs to do (this not being a good example)
//       }
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

private:

  // returns either a pointer to data (or datum) at index i in array or zero
  String* datum(int i) {return 0 <= i && i < nData() ? &paths[i] : 0;}

  // returns number of data items in array
  int   nData()      {return paths.end();}

  friend typename FPsIter;
  };

