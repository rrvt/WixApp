// Uses a Windows dialog to obtain a path to a file


#pragma once



// Start a dialog box to obtain the path to a file
//   title       - the name of the file type (e.g. "Text)"
//   inifileName - a file name that will appear in the edit box of the dialog, set to 0 if not needed
//   defExt      - the default extension which is appended if not extension provided by user, 0 if not
//                 needed
//   extPat      - one or more wild card filter of the extensions requested (e.g. "*.txt;*.cpp")
//                 The All Files filter is appended to the list as another alternative



class PathDlgDsc {
String path;
public:
String title;         // Save As Parameters, examples:
String name;          // _T("mumble.txt") or including initial path _T("D:\Sources\Data\mumble.txt")
String ext;           // _T("txt")
String pattern;       // _T("*.txt")

  PathDlgDsc() { }
  PathDlgDsc(TCchar* ttl, TCchar* nm, TCchar* e, TCchar* pat);
  PathDlgDsc(PathDlgDsc& dsc) {copy(dsc);}
 ~PathDlgDsc() { }

  PathDlgDsc& operator= (PathDlgDsc& dsc) {copy(dsc); return *this;}
  PathDlgDsc& operator() (TCchar* ttl, TCchar* nm, TCchar* e, TCchar* pat)
                            {title = ttl; name = nm ? nm : _T(""); ext = e; pattern = pat; return *this;}
  TCchar* getPath();

protected:

  void copy(PathDlgDsc& dsc);
  };


bool getOpenDlg(   PathDlgDsc& dsc, String& path);
bool getIncOpenDlg(PathDlgDsc& dsc, String& path);


bool getSaveAsPathDlg( PathDlgDsc& dsc, String& path);      // Overwrites existing file
bool getIncSavePathDlg(PathDlgDsc& dsc, String& path);      // Allows adding to existing file

bool getDirPathDlg(TCchar* title, String& path);


