// Archive File Interface


#include "pch.h"
#include "ArchFile.h"


ArchFile::ArchFile(TCchar* fileName, int mode) : opened(false), storing(false), obj(0)
                                           {opened = fil.open(fileName, mode);   setStoring(mode);}


ArchFile::ArchFile(void* arbObj, int mode) :
                                      obj(arbObj), opened(true), storing(false) {setStoring(mode);}


void ArchFile::setStoring(int mode)
  {if (mode != (FileIO::Read | FileIO::Write)) storing = mode & (FileIO::Create | FileIO::Write);}

