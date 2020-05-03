// SetEnv.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "SetEnv.h"
#include "Main.h"
#include "RestartAsAdmin.h"
#include "Utilities.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;




int _tmain(int argc, TCchar* argv[], TCchar* envp[]) {
HMODULE hModule = ::GetModuleHandle(NULL);
Main    main(argc, argv, envp);

  if (!hModule) {_tprintf(_T("Fatal Error: GetModuleHandle failed\n")); return 1;}

  if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
                                    {_tprintf(_T("Fatal Error: MFC initialization failed\n")); return 1;}
  restart.init(argc, argv);

  while (main.processArgs()) main.setEnv();

  return 0;
  }


