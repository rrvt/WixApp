// Command Line Info


#include "pch.h"
#include "CommandLineInfo.h"
#include "NotePad.h"


void CommandLineInfo::ParseParam(const TCHAR* pszParam, BOOL bFlag, BOOL bLast) {
  notePad << pszParam << nCrlf;
  notePad << bFlag << nCrlf;
  notePad << bLast << nCrlf;
  }

