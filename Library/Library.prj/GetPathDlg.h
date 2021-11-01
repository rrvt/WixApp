// Uses a Windows dialog to obtain a path to a file


#pragma once



// Start a dialog box to obtain the path to a file
//   title       - the name of the file type (e.g. "Text)"
//   inifileName - a file name that will appear in the edit box of the dialog, set to 0 if not needed
//   defExt      - the default extension which is appended if not extension provided by user, 0 if not
//                 needed
//   extPat      - one or more wild card filter of the extensions requested (e.g. "*.txt;*.cpp")
//                 The All Files filter is appended to the list as another alternative



struct PathDlgDsc {
String title;         // Save As Parameters, examples:
String name;          // _T("mumble.txt")
String ext;           // _T("txt")
String pattern;       // _T("*.txt")

  PathDlgDsc() { }
  PathDlgDsc(TCchar* ttl, TCchar* nm, TCchar* e, TCchar* pat);
  PathDlgDsc(PathDlgDsc& dsc) {copy(dsc);}
 ~PathDlgDsc() { }

  virtual PathDlgDsc& operator= (PathDlgDsc& dsc) {copy(dsc); return *this;}

  virtual void copy(PathDlgDsc& dsc);
  };


bool getPathDlg(PathDlgDsc& dsc, String& path);

bool getPathDlg(TCchar* title, TCchar* iniFileName, TCchar* defExt, TCchar* extPat, String& path);


// Overwrites existing file

bool getSaveAsPathDlg(PathDlgDsc& dsc, String& path);

bool getSaveAsPathDlg(TCchar* title, TCchar* iniFileName, TCchar* defExt, TCchar* extPat, String& path);


// Allows adding to existing file

bool getIncSavePathDlg(TCchar* title, TCchar* iniFileName, TCchar* defExt, TCchar* extPat, String& path);

bool getDirPathDlg(TCchar* title, String& path);




