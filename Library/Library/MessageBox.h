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

/*
MB_ABORTRETRYIGNORE The message box contains three pushbuttons: Abort, Retry, and Ignore.
MB_OK               The message box contains one pushbutton: OK.
MB_OKCANCEL         The message box contains two pushbuttons: OK and Cancel.
MB_RETRYCANCEL      The message box contains two pushbuttons: Retry and Cancel.
MB_YESNO            The message box contains two pushbuttons: Yes and No.
MB_YESNOCANCEL      The message box contains three pushbuttons: Yes, No, and Cancel.
*/

inline int msgChoiceBox(TCchar* txt) {return AfxMessageBox(txt, MB_OKCANCEL);}


