/* filenames
// (C) Copyright Software Design & Engineering, 3/1/95
// All rights reserved
// rrvt 3/1/95    */


#pragma once


String removePath(TCchar* name);

String getPath(TCchar* fullPath);

String getMainName(TCchar* fullPath);

String getExtension(TCchar* fullPath);

String getDirName(TCchar* fullPath);                // i.e. last name between \\ and \\ or eol

String incVersionNo(TCchar* filePath);              // Removes path and extension, adds and
                                                    // increments a version number to main name
bool   change_extension(String& name, TCchar* ext);
bool   change_extension(Tchar* name, int dSize, TCchar* ext);

void   add_extension(Tchar* name, int dSize, TCchar* ext);

void   appendSepChar(Tchar* name, int dSize);

bool   renameFile(Tchar* orig, Tchar* newName, int dSize);

bool   legal_extension(TCchar* name, String* ext, int nExt);

bool   isExplicitExt(TCchar* fn);           // Returns true if extension does not contain a '?' or an '*'

void   removeFile(TCchar* name);

String noBlanks(TCchar* name);

void   removeOneDir(String& t);

