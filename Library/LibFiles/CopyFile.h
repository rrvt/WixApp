//  Copy and/or Move (Rename) File
// rrvt 6/5/16


#pragma once


bool copyFile(String& source, String& dst);

inline bool moveFile(String& source, String& dst) {return _trename(source, dst) == 0;}

//int  backupFile(String& path, int lastBackup, int nBackups);    // Returns last backup index

void backupFile(String& filePath, int noBackups);     // Moves filePath to backup name
void backupCopy(String& filePath, int noBackups);     // Copies filePath to backup name

bool isFilePresent(String& path);
