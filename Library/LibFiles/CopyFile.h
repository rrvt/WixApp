//  Copy and/or Move (Rename) File
// rrvt 6/5/16


#pragma once


bool copyFile(String& source, String& dst);

inline bool moveFile(String& source, String& dst) {return _trename(source, dst) == 0;}

int backupFile(String& path, int lastBackup = 0, int nBackups = 0);    // Returns last backup index

bool isFilePresent(String& path);
