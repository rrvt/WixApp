// Command Line Info


#pragma once


class CommandLineInfo : public CCommandLineInfo {

public:

  CommandLineInfo() : CCommandLineInfo() { }
 ~CommandLineInfo() { }

  virtual void ParseParam(const TCHAR* pszParam, BOOL bFlag, BOOL bLast);
  };

