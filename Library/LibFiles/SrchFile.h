// Search File


#pragma once


class SrchFile : public CStdioFile {

BOOL           opened;
CFileException err;

public:

CString line;

  SrchFile() : opened(false) {}
  SrchFile(TCchar* path);

  bool isOpen() {return opened == TRUE;}
  bool forPat(TCchar* pat);
  bool forValOf(TCchar* key, CString& val);              // lines have form:  Key=val
  };
