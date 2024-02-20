// Message Boxes with Various Inputs
// Translate a system error valude into a text string


#pragma once


void getError(DWORD v, String& err);              // Turn a GetLastError() value into a string

void messageBox(String s);

void messageBox(TCchar* s);

void messageBox(TCchar* prefix, TCchar* s);

void messageBox(CString& s);

void debug(TCchar* s);

void debug(TCchar* s, ulong v);

void debug(TCchar* s, String v);

void messageBox(TCchar* lbl, void* p, int n);


