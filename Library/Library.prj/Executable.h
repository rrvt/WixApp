// Manipulate and exe process


#pragma once
#include "Expandable.h"
#include "IterT.h"


class Executable;
typedef IterT<Executable, String> ArgIter;


class Executable {

enum    ExeState {Begin, Arg, QArg, Escp};
TCchar* p;
Expandable<String, 4> data;

PROCESS_INFORMATION procInfo;
LPWSTR*   szArglist;

public:

          Executable() { }
         ~Executable() { }

  // Use the following three functions to recover arguments sent to the CWinApp application

  void    procArgs(TCchar* m_lpCmdLine);    // Typically called from InitInstance of a Windows Application
                                            // m_lpCmdLine is a member of CWinApp
                                            // Note:  The application path is not contained in the
                                            // m_lpCmdLine string
  String* getArg(int i) {return datum(i);}  // Return a specific argument by the index or zero
  int     nArgs()       {return nData();}   // Number of arguments found

// Use the following three functions from the calling process to start another process passing arguments
// Arguments will be enclosed in quotes by the start function.  The first argument must be the name or
// path of the executable that is to be started. Here is an example:
//
//   Executable exe;  exe.start(3, _T("myApp.exe"), _T("Arg1"), _T("Arg2"));

  bool    start(int nCmdArgs, ...);             // nCmdArgs indicates the number of optional arguments
                                                // All the optional arguments must be of type TCchar*
//bool    start(TCchar* cmd, TCchar* args);     // Each argument must be enclosed in double quotes,
                                                // it is optional to enclose cmd in double quotes
  void    wait(DWORD timeout = INFINITE);
  bool    stop();

private:

  String& addQuotes(String& s);       // Adds quotes if needed avoiding terminal backslash with a space
  WCHAR*  copy(TCchar* src, WCHAR* vector, int nChars);

  bool    isArg();
  bool    procArg(String& s);
  void    backSlash(String& s);

  // returns either a pointer to data (or datum) at index i in array or zero

  String* datum(int i) {return 0 <= i && i < nData() ? &data[i] : 0;}       // or data[i].p

  int    nData()      {return data.end();}                       // returns number of data items in array

  void   removeDatum(int i) {if (0 <= i && i < nData()) data.del(i);}

  friend typename ArgIter;
  };

