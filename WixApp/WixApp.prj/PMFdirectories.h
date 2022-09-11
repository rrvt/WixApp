// Directory Fragment


#pragma once
#include "DirectoryStore.h"


extern TCchar* PmfExt;


class Feature;



class PMFdirectories {

public:

DirStor stor;                            // StartMenu folders (directories) into which links are placed
bool    startMenuSeen;
bool    desktopDirSeen;
bool    startupDirSeen;

//DirDesc appDir;

  PMFdirectories() : stor(PmfExt), startMenuSeen(false), desktopDirSeen(false), startupDirSeen(false) {}
 ~PMFdirectories() { }

  void     initFixedDirs() { startMenuSeen = desktopDirSeen = startupDirSeen = false;}
  void     setStartMenuSeen(bool seen) {startMenuSeen  |= seen;}
  void     setDeskTopSeen(bool seen)   {desktopDirSeen |= seen;}
  void     setStartupSeen(bool seen)   {startupDirSeen |= seen;}

//  void markDir(String& id) {stor.markDir(id);}
  DirDesc* find(String& id) {return stor.find(id);}

  DirDesc* add(String& fullPath) {return stor.add(fullPath);}
  DirDesc* getDefault() {         return stor.getDefault();}

  String   fullPath(String& id);

  void     readWixData()  {stor.readWixData();}
  void     writeWixData() {stor.writeWixData();}

  void     output();
  void     outputSubs(String& parent, int tab) {stor.outputSubs(parent, tab);}

  void     outputRemoves(int tab) {stor.outputRemoves(tab);}

  void     prepareUninstalls(Feature* ftr) {stor.prepareUninstalls(ftr);}
  };

extern TCchar* DeskTopDir;
extern TCchar* StartupDir;

extern PMFdirectories pmfDirectories;





