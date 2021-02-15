// Subclass of CWinAppEx


#include "stdafx.h"
#include "CApp.h"
#include "MessageBox.h"
#include "Resources.h"


CApp* theCApp;


CApp::CApp(CApp* app) : doc(0), view(0) {
ResourceData res;

  theCApp = app;

  res.getVersion(version);

  if (res.getAppID(appID)) SetAppID(appID);   else messageBox(_T("Unable to create an appID"));

  EnableHtmlHelp();
  }


CDocument* CApp::getDoc() {

  if (doc) return doc;

  POSITION      pos = GetFirstDocTemplatePosition(); if (!pos) return 0;
  CDocTemplate* t   = GetNextDocTemplate(pos);       if (!t)   return 0;

  pos = t->GetFirstDocPosition();                    if (!pos) return 0;
  doc = t->GetNextDoc(pos);   return doc;
  }


CView* CApp::getView() {
POSITION   pos;

  if (view) return view;

  if (!getDoc()) return 0;

  pos  = doc->GetFirstViewPosition();        if (!pos)  return 0;
  view = doc->GetNextView(pos);   return view;
  }


HANDLE CApp::defDevMode() {
PRINTDLG pd;

  pd.lStructSize = (DWORD) sizeof(PRINTDLG);

  return GetPrinterDeviceDefaults(&pd) ? pd.hDevMode : 0;
  }


/*
Find memory leaks with the CRT Library - Visual Studio | Microsoft
  https://docs.microsoft.com/en-us/visualstudio/debugger/finding-memory-leaks-using-the-crt-library?view=vs-2019

Memory leaks are incidious.  Here is some help for finding them.  Look at stdafx.h for turning on the
facilities needed for the following to work.  Must be turned on in all projects.

  #ifdef DebugMemoryLeaks
  #define _CRTDBG_MAP_ALLOC
  #include <crtdbg.h>
  #endif

If your app doesn't define _CRTDBG_MAP_ALLOC , _CrtDumpMemoryLeaks displays a memory-leak report that
looks like:

  Detected memory leaks!
  Dumping objects ?>
  {18} normal block at 0x00780E80, 64 bytes long.
   Data: < > CD CD CD CD CD CD CD CD CD CD CD CD CD CD CD CD
  Object dump complete.

If your app defines _CRTDBG_MAP_ALLOC , the memory-leak report looks like:

  Detected memory leaks!
  Dumping objects ?>
  c:\users\username\documents\projects\leaktest\leaktest.cpp(20) : {18}
  normal block at 0x00780E80, 64 bytes long.
   Data: < > CD CD CD CD CD CD CD CD CD CD CD CD CD CD CD CD
  Object dump complete.

The second report shows the filename and line number where the leaked memory is first allocated.

Whether or not you define _CRTDBG_MAP_ALLOC , the memory-leak report displays:

  * The memory allocation number, which is 18 in the example
  * The block type, normal in the example.
  * The hexadecimal memory location, 0x00780E80 in the example.
  * The size of the block, 64 bytes in the example.
  * The first 16 bytes of data in the block, in hexadecimal form.

Memory block types are normal, client, or CRT. A normal block is ordinary memory allocated by your
program. A client block is a special type of memory block used by MFC programs for objects that require
a destructor. The MFC new operator creates either a normal block or a client block, as appropriate for
the object being created.

A CRT block is allocated by the CRT library for its own use. The CRT library handles the deallocation
for these blocks, so CRT blocks won't appear in the memory-leak report unless there are serious problems
with the CRT library.

There are two other types of memory blocks that never appear in memory-leak reports. A free block is
memory that has been released, so by definition isn't leaked. An ignore block is memory that you've
explicitly marked to exclude from the memory-leak report.

// Just a quick note:  The module NewAllocator defines some macros for displaying the location of each
// allocation of memory performed by the program.  The bug that required this note and additional
// debugging capability came about because the programmed initialized a string twice throwing away the
// memory allocated for the first initialization.  This was very hard to find since the location was not
// recorded but the allocation number (count of the number of allocations) was recorded and that allowed
// the problem to be found.  It was easy to fix, hard to find.

Setting Breakpoints on a memory allocation number

The memory allocation number tells you when a leaked memory block was allocated. A block with a memory
allocation number of 18, for example, is the 18th block of memory allocated during the run of the app.
The CRT report counts all memory-block allocations during the run, including allocations by the CRT
library and other libraries such as MFC. Therefore, memory allocation block number 18 probably isn't the
18th memory block allocated by your code.

You can use the allocation number to set a breakpoint on the memory allocation.

To set a memory-allocation breakpoint using the Watch window:

  1. Set a breakpoint near the start of your app, and start debugging.

  2. When the app pauses at the breakpoint, open a Watch window by selecting Debug > Windows > Watch 1
    (or Watch 2, Watch 3, or Watch 4).

  3. In the Watch window, type _crtBreakAlloc in the Name column.

     If you're using the multithreaded DLL version of the CRT library (the /MD option), add the context
     operator:  {,,ucrtbased.dll}_crtBreakAlloc

     Make sure that debug symbols are loaded. Otherwise _crtBreakAlloc will be reported as unidentified.

  4. Press Enter.

     The debugger evaluates the call and places the result in the Value column. This value will be -1 if
     you have not set any breakpoints on memory allocations.

  5. In the Value column, replace the value with the allocation number of the memory allocation where
     you want the debugger to break.

After you set a breakpoint on a memory-allocation number, continue to debug. Make sure to run under the
same conditions, so the memory-allocation number doesn't change. When your program breaks at the
specified memory allocation, use the Call Stack window and other debugger windows to determine the
conditions under which the memory was allocated. Then, you can continue execution to observe what happens to the object and determine why it isn't correctly deallocated.

Setting a data breakpoint on the object might also be helpful. For more information, see Using
breakpoints.

You can also set memory-allocation breakpoints in code. You can set:

  _crtBreakAlloc = 18;

or

  _CrtSetBreakAlloc(18);
*/


int CApp::ExitInstance() {

  #ifdef DebugMemoryLeaks
    _CrtDumpMemoryLeaks();
  #endif

  return CWinAppEx::ExitInstance();
  }
