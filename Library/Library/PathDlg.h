// Rework of PathDlgDsc and GetPathDlg


#pragma once


class PathDlg {
public:

String title;         // Title for dialog upper boarder
String iPath;         // Initial full path
String ext;           // File Extension without the period (e.g. "cpp")
String filterTtl;     // One or two Words that identifies the file type
String pattern;       // Filter for file type based on extension (e.g. "*.cpp")


  PathDlg() { }
  PathDlg(TCchar* dlgTtl, TCchar* iniPath, TCchar* extension, TCchar* fltrTtl, TCchar* pat);
  PathDlg(TCchar* dlgTtl, TCchar* iniPath) : title(dlgTtl), iPath(iniPath) { }
  PathDlg(PathDlg& dsc) {copy(dsc);}
 ~PathDlg() { }

  PathDlg& operator= (PathDlg& dsc) {copy(dsc); return *this;}
  PathDlg& operator() (TCchar* ttl, TCchar* nm, TCchar* e, TCchar* fltrTtl, TCchar* pat);

  bool folder(String& path);                  // find folder from title and initial path (in name)

  bool createFile(    String& path);

  bool openFile(       String& path) {return open(false, path);}
  bool incOpenFile(    String& path) {return open(true,  path);}

  bool saveFileAs( String& path)   {return save(true, path);}    // Overwrites existing file
  bool incSaveFileAs(String& path) {return save(false, path);}   // Adding to existing file

protected:

  void copy(PathDlg& dsc);

private:

  bool open(bool overwrt, String& path);
  bool save(bool overwrt, String& path);
  bool doDlg(bool open, DWORD flags, String& path);
  };

