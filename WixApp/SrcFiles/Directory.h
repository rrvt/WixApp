// Directory Fragment


#pragma once
#include "DirectoryStore.h"


extern TCchar* PffExt;
extern TCchar* PmfExt;
extern TCchar* DktExt;


class Group;



class Directory {

public:

DirStor pff;                            // c:\ProgramFiles subdirectories into which apps are placed
DirStor pmf;                            // StartMenu folders (directories) into which links are placed
//DirStor dkt;                            // Desktop folders into which links are placed (usually one)

DirDesc appDir;

  Directory() : pff(PffExt), pmf(PmfExt) {}               //, dkt(DktExt)
 ~Directory() { }

  void output();
  void outputRemoves(int tab);

  void prepareUninstalls(Group* grp);
  };










extern TCchar* DeskTopDir;

extern Directory directory;





