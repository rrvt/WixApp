// Standard Definitions
// rrvt 8/25/91
// Copyright (C) 1991 by Software Design & Engineering, Robert R. Van Tuyl, All rights reserved.
//$(SolutionDir)..\..\..\Sources\Library\LibFiles\
//D:\Sources\Library\LibFiles

#pragma once



typedef unsigned short      ushort;
typedef unsigned long       ulong;
typedef unsigned int        uint;


typedef unsigned char       Byte;
typedef const    char       Cchar;
typedef          TCHAR      Tchar;
typedef const    TCHAR      TCchar;
typedef const   _TUCHAR     TUCchar;

typedef          int64_t    int64;
typedef          uint64_t   uint64;
typedef          LONG64     long64;
typedef          ULONG64    ulong64;
typedef        __time64_t   Time64;

// Platform dependent integers (same size as void*)
#ifdef _M_IX86
typedef int                 intP;
typedef uint                uintP;
typedef long                longP;
typedef ulong               ulongP;
#else
typedef int64               intP;
typedef uint64              uintP;
typedef long64              longP;
typedef ulong64             ulongP;
#endif

#define loop for (;;)

#define noElements(array)     (sizeof(array) / sizeof(array[0]))

#define pathSepChar _T('\\')
#define pathSepStg  _T("\\")
#define uniSepChar  _T('/')


