// Trim blanks from both ends of String
// (C) Copyright Software Design & Engineering
// All rights reserved
// rrvt 6/26/95


#pragma once


// Trim blanks from both ends of String

Tchar*  trim(Tchar* p);
//String trim(String& s);


// Remove whitespace from end of String, return number of bytes in String

int trimEnd(Tchar* s);

