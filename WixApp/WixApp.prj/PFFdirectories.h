// Directory Fragment


#pragma once
#include "DirStore.h"


extern TCchar* PffExt;


class Feature;



class PFFdirectories {

public:

DirStor stor;                         // c:\ProgramFiles subdirectories into which apps are placed

DirDesc* appDir;

           PFFdirectories() : stor(PffExt) {}
          ~PFFdirectories() { }

 void      readWixData()  {stor.readWixData();}
 void      writeWixData() {stor.writeWixData();}

  DirDesc* find(String& id)      {return stor.findItem(id);}

  DirDesc* add(String& fullPath) {return stor.add(fullPath);}
  DirDesc* getDefault()          {return stor.getDefault();}

  String   fullPath(String& id);

  void     begOutput(int& tab);
  void     finOutput(int tab);
  void     outputSubs(String& parent, int tab) {stor.outputSubs(parent, tab);}

  void     saveData(Archive& ar);
  };


extern PFFdirectories pffDirectories;



