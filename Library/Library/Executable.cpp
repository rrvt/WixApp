// Manipulate and exe process


#include "pch.h"
#include "Executable.h"
#include "MessageBox.h"
#include <processenv.h>


static BOOL CALLBACK EnumWindowsProcMy(HWND hwnd, LPARAM targetId);


/*
BOOL CreateProcess(
  LPCSTR                lpApplicationName,
  LPSTR                 lpCommandLine,
  LPSECURITY_ATTRIBUTES lpProcessAttributes,
  LPSECURITY_ATTRIBUTES lpThreadAttributes,
  BOOL                  bInheritHandles,
  DWORD                 dwCreationFlags,
  LPVOID                lpEnvironment,
  LPCSTR                lpCurrentDirectory,
  LPSTARTUPINFOA        lpStartupInfo,
  LPPROCESS_INFORMATION lpProcessInformation
  );
*/

// D:\Sources\PreOutpost\Debug\OpAddr.exe


bool Executable::start(int nCmdArgs, ...) {
int         i;
va_list     vaList;
TCchar*     tc;
String      arg;
String      cmdLine;
WCHAR*      wCmdArgs;
int         wSize;
STARTUPINFO info={sizeof(info)};

  for (i = 0, va_start(vaList, nCmdArgs), tc = va_arg(vaList, TCchar*); i < nCmdArgs;
                                                                      i++, tc = va_arg(vaList, TCchar*)) {
    arg = tc;   addQuotes(arg);

    if (i) cmdLine += _T(' ');   cmdLine += arg;
    }
  va_end(vaList);

  wSize = cmdLine.length() + nCmdArgs * 5;

  NewArray(WCHAR); wCmdArgs = AllocArray(wSize);
  copy(cmdLine, wCmdArgs, wSize);

  if (CreateProcess(0, wCmdArgs, 0, 0, true, 0, 0, 0, &info, &procInfo))
                                                                    {FreeArray(wCmdArgs);   return true;}
  FreeArray(wCmdArgs);

  String err;   getError(GetLastError(), err);    messageBox(err);  return false;
  }



String& Executable::addQuotes(String& s) {
int pos;

  if (s[0] == _T('"')) return s;

  pos = s.length() - 1;

  if (s[pos] == _T('\\')) s = s.substr(0, pos);

  s = _T('"') + s + _T('"');

  return s;
  }


WCHAR* Executable::copy(TCchar* src, WCHAR* vector, int nChars) {
int i;
int n;

  if (!src) return 0;   n = _tcslen(src);   if (n >= nChars) return 0;

  for (i = 0; i < n; i++) vector[i] = *src++;   vector[i] = 0;

  return vector;
  }



void Executable::wait(DWORD timeout) {WaitForSingleObject(procInfo.hProcess, timeout);}


bool Executable::stop() {

  if (EnumWindows(EnumWindowsProcMy, procInfo.dwProcessId)) return true;

  String err;   getError(GetLastError(), err);    messageBox(err);  return false;
  }


// Called for each top-level window on the screen
// Turns out that an executable may have two or more HWNDs active when we wish to stop it.
// Messages only go to windows, so better send it to all windows with the target ID!

BOOL CALLBACK EnumWindowsProcMy(HWND hwnd, LPARAM targetId) {
DWORD processID;

  GetWindowThreadProcessId(hwnd, &processID);

  if (processID == targetId) SendMessage(hwnd, WM_CLOSE, 0, 0);

  return true;
  }


void Executable::procArgs(TCchar* m_lpCmdLine) {
int    i;
String s;

  for (i = 0, p = m_lpCmdLine; isArg(); i++) procArg(data[i]);
  }


bool Executable::isArg() {
Tchar ch;

  for (ch = *p; ch; ch = *++p) if (ch != _T(' ')) return true;

  return false;
  }


bool Executable::procArg(String& s) {
ExeState st = Begin;
Tchar    ch;

  s.clear();

  for (ch = *p; ch; ch = *++p) {
    switch (st) {
      case Begin: if (ch == _T('"')) {st = QArg; continue;}
                  s += ch; st = Arg; continue;

      case Arg:   if (ch == _T(' ')) return true;
                  if (ch == _T('\\')) {backSlash(s); continue;}
                  s += ch; continue;

      case QArg:  if (ch == _T('"'))  {p++; return true;}
                  if (ch == _T('\\')) {backSlash(s);  continue;}
                  s += ch; continue;
      }
    }

  return !s.isEmpty();
  }


void Executable::backSlash(String& s) {
Tchar next = p[1];

  if (next == _T('\\') || next == _T('"')) {s += next; p++; return;}

  s += _T('\\');
  }


