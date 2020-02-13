// Decouple from main program


#pragma once
#include "Archive.h"


enum FileType {GenericType};
class Device;
class DataFile;
class BuildDataDoc;




//bool getDspHandle(DataFile*& f);
//bool getDataHandle(DataFile*& f);

DataFile* getDspHandle();
DataFile* getDataHandle();

String getObjName();


void clrData();

void dataOut(TCchar* fmt, ...);



// Call to start read from a disk file to a ConfigFile
bool doOpen(Device* dev, DataFile* cnfgFile, String& path);

// call to start save from a ConfigFile to disk
bool  doSave(Device* dev, DataFile* cnfgFile, String& path);

String& getOutputPath();


void writeFile(DataFile* cf, Archive& ar);

