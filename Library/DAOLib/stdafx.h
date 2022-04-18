// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifdef DebugMemoryLeaks
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN                        // Exclude rarely-used stuff from Windows headers
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS  // some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#include <afx.h>
#include <afxwin.h>                         // MFC core and standard components
#include <afxext.h>                         // MFC extensions
#include <afxdisp.h>                        // MFC Automation classes

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>                       // MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                         // MFC support for Windows Common Controls
#endif

#include <afxcontrolbars.h>                 // MFC support for ribbons and control bars
#include <afxdialogex.h>

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



