// Restart as Administrator
// A program cannot just raise their capability to admin level, so just restart with the same
// arguments


#include "pch.h"
#include "RestartAsAdmin.h"
#include "Utilities.h"


RestartAsAdmin restart;


RestartAsAdmin::RestartAsAdmin() : nArgs(0), args(0) {}


void RestartAsAdmin::init(int argc, TCchar* argv[]) {nArgs = argc; args = argv;}


bool RestartAsAdmin::elevate() {

  try {
    if (!isAdmin()) {

      Tchar szPath[MAX_PATH];

      if (GetModuleFileName(NULL, szPath, ARRAYSIZE(szPath))) {

        String s = getArgString();

        SHELLEXECUTEINFO sei = {sizeof(sei)};                          // Launch itself as admin
        sei.lpVerb       = _T("runas");
        sei.lpFile       = szPath;
        sei.lpParameters = s;
        sei.hwnd         = NULL;
        sei.nShow        = SW_NORMAL;

      if (!ShellExecuteEx(&sei)) {
        if (GetLastError() == ERROR_CANCELLED) return false;
        dspErr(_T("Unable to run as administrator, Error:"), GetLastError());
        Sleep(10000); exit(1);
        }
      exit(0);
      }
    }
  }
  catch(...)
           {dspErr(_T("Exception error determining if running as Administrator"), GetLastError());}

  return false;
  }



bool RestartAsAdmin::isAdmin() {
BOOL                     fIsRunAsAdmin        = FALSE;
DWORD                    dwError              = ERROR_SUCCESS;
PSID                     pAdministratorsGroup = NULL;
SID_IDENTIFIER_AUTHORITY NtAuthority          = SECURITY_NT_AUTHORITY;
                                                                // Allocate and initialize a SID of
                                                                // the administrators group.
  if (!AllocateAndInitializeSid(&NtAuthority,
                                2,
                                SECURITY_BUILTIN_DOMAIN_RID,
                                DOMAIN_ALIAS_RID_ADMINS,
                                0, 0, 0, 0, 0, 0,
                                &pAdministratorsGroup
                                )) {dwError = GetLastError(); goto Cleanup;}

  // Determine whether the SID of administrators group is enabled in
  // the primary access token of the process.

  if (!CheckTokenMembership(NULL, pAdministratorsGroup, &fIsRunAsAdmin))
                                                          {dwError = GetLastError(); goto Cleanup;}

Cleanup:                                  // Centralized cleanup for all allocated resources.

  if (pAdministratorsGroup) {FreeSid(pAdministratorsGroup); pAdministratorsGroup = NULL;}

  // Throw the error if something failed in the function.

  if (dwError != ERROR_SUCCESS) throw dwError;

  return fIsRunAsAdmin != 0;
  }



String RestartAsAdmin::getArgString() {
String arg;
String s;
int    i;

  for (i = 1; i < nArgs; i++) {
    if (i > 1) s += _T(' ');
    arg = args[i];   trim(arg);

    if (arg.find(_T(' ')) >= 0) arg = _T("\" ") + arg + _T(" \"");

    s += arg;
    }

  return s;
  }

