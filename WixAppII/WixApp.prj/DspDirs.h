// Display Directories


#pragma once
#include "DirDesc.h"
#include "fileIO.h"


class DspDirs {

String path;
FileIO fo;

public:

  DspDirs();
 ~DspDirs() {fo.close();}

  void output();
  void output(TCchar* title, DirStor& stor);
  };

