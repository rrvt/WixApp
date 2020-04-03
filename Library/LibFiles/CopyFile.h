//  Copy and/or Move (Rename) File
// rrvt 6/5/16


#pragma once


bool copyFile(String& source, String& dst);

inline bool moveFile(String& source, String& dst) {return _trename(source, dst) == 0;}

//int  backupFile(String& path, int lastBackup, int nBackups);    // Returns last backup index

void backupFile(String& filePath, int noBackups);

bool isFilePresent(String& path);
