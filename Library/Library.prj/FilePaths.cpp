// find File somewhere in a file tree


#include "stdafx.h"
#include "FilePaths.h"
#include "Expandable.h"
#include "filesrch.h"
#include "match.h"



bool FilePaths::findDirs(TCchar* startPath, TCchar* dirWildCard, TCchar* tgtWildCard) {
RegExpr                tgt;
FileSrch               fileSrch;
int                    nDir;
Expandable<String, 16> directories;
int                    i;

  tgt.setWildCardPattern(tgtWildCard);

  fileSrch.findSubDirs(startPath, dirWildCard);

  for (nPaths = nDir = 0; fileSrch.getName(directories[nDir]); nDir++) continue;

  for (i = 0; i < nDir; i++) {
    String& pth     = directories[i];
    int     pos     = pth.findLastOf('\\');
    String  dirName = pos >= 0 ? pth.substr(pos+1) : pth;

    if (tgt.match(dirName)) {paths[nPaths] = pth; nPaths++;}

    findDirs(pth, _T("*"), tgtWildCard);
    }

  return nPaths > 0;
  }


// Find all files that match the tgt wild card at startPath and in all subdirectories of startPath

bool FilePaths::findFiles(TCchar* startPath, TCchar* tgtWildCard) {
RegExpr                tgt;
FileSrch               fileSrch;
int                    nDir;
Expandable<String, 16> directories;
int                    i;
String                 name;

  tgt.setWildCardPattern(tgtWildCard);

  fileSrch.findAllFiles(startPath);

  while (fileSrch.getName(name)) {
    int    pos      = name.findLastOf('\\');
    String fileName = pos >= 0 ? name.substr(pos+1) : name;

    if (tgt.match(fileName)) {paths[nPaths] = name; nPaths++;}
    }

  fileSrch.findSubDirs(startPath, _T("*"));

  for (nDir = 0; fileSrch.getName(directories[nDir]); nDir++) continue;

  for (i = 0; i < nDir; i++)
    findFiles(directories[i], tgtWildCard);

  return nPaths > 0;
  }


// Example Search call:
//  findfile("C:", "Program*", "AcroRd*.exe", resultingPath);

bool FilePaths::findFiles(TCchar* startPath, TCchar* dirWildCard, TCchar* tgtWildCard) {
RegExpr                tgt;
FileSrch               fileSrch;
int                    nDir;
Expandable<String, 16> directories;
int                    i;
String                 name;

  tgt.setWildCardPattern(tgtWildCard);

  fileSrch.findSubDirs(startPath, dirWildCard);

  for (nDir = 0; fileSrch.getName(directories[nDir]); nDir++) continue;

  for (i = 0; i < nDir; i++) {
    String& pth = directories[i];

    fileSrch.findAllFiles(pth);

    while (fileSrch.getName(name)) {
      int    pos      = name.findLastOf('\\');
      String fileName = pos >= 0 ? name.substr(pos+1) : name;

      if (tgt.match(fileName)) {paths[nPaths] = name; nPaths++;}
      }

    findFiles(pth, _T("*"), tgtWildCard);
    }

  return nPaths > 0;
  }

