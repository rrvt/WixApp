// Get Version of exe file


#pragma once


class ResourceData {
Byte*             data;
DWORD             dataSize;           // in Bytes
DWORD             handle;
VS_FIXEDFILEINFO* fileInfo;
String            prefix;
bool              success;

public:

String path;

       ResourceData();                           // Use when in a static library
       ResourceData(void*   staticEntity);       // Need an address in the application to identify app
       ResourceData(String& path);               // Any application
      ~ResourceData();

  bool isOpened() {return success;}

  bool getAppID(String& s);

  bool getVersion(String& s);
  bool getCompanyName(     String& s) {return getStg(_T("CompanyName"),      s);}
  bool getFileDescription( String& s) {return getStg(_T("FileDescription"),  s);}
  bool getFileVersion(     String& s) {return getStg(_T("FileVersion"),      s);}
  bool getInternalName(    String& s) {return getStg(_T("InternalName"),     s);}
  bool getCopyRight(       String& s) {return getStg(_T("LegalCopyright"),   s);}
  bool getOriginalFilename(String& s) {return getStg(_T("OriginalFilename"), s);}
  bool getProductName(     String& s) {return getStg(_T("ProductName"),      s);}
  bool getProductVersion(  String& s) {return getStg(_T("ProductVersion"),   s);}
  uint getFileType();

  void threeSect(String& t);                  // Trim version (i.e. 1.2.3.4) to 3 segments (i.e. 1.2.3)
private:

  void initEntity(void* staticEntity);        // Initialize exe or dll by static address
  void initialize(TCchar* path);              // May be just the app's full name
  bool getStg(TCchar* pat, String& s);
  };


