//  Copy and/or Move (Rename) File
// rrvt 6/5/16


#pragma once


bool copyFile(TCchar*  source, String& dst);

inline bool moveFile(TCchar*  source, String& dst) {return _trename(source, dst) == 0;}

//int  backupFile(String& path, int lastBackup, int nBackups);    // Returns last backup index

void backupFile(TCchar* filePath, int noBackups);     // Moves filePath to backup name
void backupCopy(TCchar* filePath, int noBackups);     // Copies filePath to backup name

bool isFilePresent(TCchar* path);
