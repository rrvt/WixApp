// Uses a Windows dialog to obtain a path to a file


#pragma once

class PathDlgDsc;



bool createFileDlg(PathDlgDsc& dsc, String& path);

bool getOpenDlg(   PathDlgDsc& dsc, String& path);
bool getIncOpenDlg(PathDlgDsc& dsc, String& path);


bool getSaveAsPathDlg( PathDlgDsc& dsc, String& path);      // Overwrites existing file
bool getIncSavePathDlg(PathDlgDsc& dsc, String& path);      // Allows adding to existing file

bool getDirPathDlg(TCchar* title, String& path);


