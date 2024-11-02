// Utility Functions


#include "pch.h"
#include "SftpUtilities.h"
#include "FileName.h"




static TCchar* asciiExt[] = {_T("htm"), _T("html"), _T("css"), _T("dwt"), _T("txt"), _T("cpp"), _T("h")};
static TCchar* AsciiTyp   = _T("A");
static TCchar* BinaryTyp  = _T("I");


// Ascii files are copied as text, the rest are copied as images

SSLFileType sftpFileType(TCchar* webFile) {
String    ext = getExtension(webFile);
int       i;
int       n = noElements(asciiExt);

  for (i = 0; i < n; i++) if (ext == asciiExt[i]) return AsciiFlTyp;

  return ImageFlTyp;
  }


TCchar* sftpFileType(SSLFileType ft) {
  switch (ft) {
    case AsciiFlTyp : return AsciiTyp;
    case ImageFlTyp : return BinaryTyp;
    }
  return 0;
  }

