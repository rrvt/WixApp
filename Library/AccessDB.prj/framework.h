// stdafx.h in the form of pch.h/framework.h ...

#pragma once

#ifdef DebugMemoryLeaks
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN                              // Exclude rarely-used stuff from Windows headers
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS        // some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#include <afxwin.h>                               // MFC core and standard components
#include <afxext.h>                               // MFC extensions

#include <afxdisp.h>                              // MFC Automation classes

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>                             // MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                               // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include "afxwinappex.h"

#include <afxdialogex.h>
#include <afxcontrolbars.h>                       // MFC support for ribbons and control bars
#include <afxdb.h>                                // Database Functionality

#include <comutil.h>
#include <string>
#include <tchar.h>

// TODO: reference additional headers your program requires here

#include "std.h"

using namespace std;

typedef basic_string<Tchar> tstring;

#include "Strings.h"
#include "Variant.h"
#include "NewAllocator.h"

