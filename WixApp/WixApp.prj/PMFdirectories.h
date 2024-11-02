// Directory Fragment


#pragma once
#include "DirStore.h"


extern TCchar* PmfExt;


class Feature;



class PMFdirectories {

public:

DirStor stor;                       // StartMenu folders (directories) into which links are placed
bool    startMenuSeen;
bool    desktopDirSeen;
bool    startupDirSeen;

//DirDesc appDir;

  PMFdirectories() : stor(PmfExt), startMenuSeen(false), desktopDirSeen(false),
                                                                          startupDirSeen(false) {}
 ~PMFdirectories() { }

  void     clearMarks() {stor.clearMarks();}

  void     initFixedDirs() { startMenuSeen = desktopDirSeen = startupDirSeen = false;}
  void     setStartMenuSeen(bool seen) {startMenuSeen  |= seen;}
  void     setDeskTopSeen(bool seen)   {desktopDirSeen |= seen;}
  void     setStartupSeen(bool seen)   {startupDirSeen |= seen;}

  DirDesc* find(String& id) {return stor.findItem(id);}

  DirDesc* add(String& fullPath) {return stor.add(fullPath);}
  DirDesc* getDefault() {         return stor.getDefault();}

  String   fullPath(String& id);

  void     readWixData()  {stor.readWixData();}
  void     writeWixData() {stor.writeWixData();}

  void     output(int& tab);
  void     outputSubs(String& parent, int tab) {stor.outputSubs(parent, tab);}

  void     outputRemoves(int tab) {stor.outputRemoves(tab);}

  void     prepareUninstalls(Feature* ftr) {stor.prepareUninstalls(ftr);}

  void     saveData(Archive& ar);
  };

extern TCchar* DeskTopDir;
extern TCchar* StartupDir;

extern PMFdirectories pmfDirectories;





