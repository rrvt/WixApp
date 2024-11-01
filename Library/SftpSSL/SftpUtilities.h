// Utility Functions


#pragma once

enum SSLFileType {NilFlTyp, AsciiFlTyp, ImageFlTyp};            // Read/Write File Types
enum SSLrslt     {SSLfalse, SSLtrue, SSLtimeout};


SSLFileType sftpFileType(TCchar* webFile);  // Returns an enum value
TCchar*     sftpFileType(SSLFileType ft);   // which is translated to a TCchar* pointer to a string

