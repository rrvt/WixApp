// Uses a Windows dialog to obtain a path to a file


#pragma once

bool getPathDlg(TCchar* title, TCchar* iniFileName, TCchar* defExt, TCchar* ext, String& path);

bool getSaveAsPathDlg(TCchar* title, TCchar* iniFileName, TCchar* defExt, TCchar* ext, String& path);


bool getDirPathDlg(TCchar* title, String& path);




