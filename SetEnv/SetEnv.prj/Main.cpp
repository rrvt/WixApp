// Main -- Get started with arguments and do everything by calling others


#include "stdafx.h"
#include "main.h"
#include "Registry.h"
#include "Resources.h"
#include "Utilities.h"





Main::Main(int argc, TCchar* argv[], TCchar* envp[]) : nArgs(argc), argX(1), args(argv),
                                                                                     env(envp) {init();}


void Main::init() {operation = NilOp; isSystemEnv = true; name.clear(); value.clear();}


// Process the command line arguments

bool Main::processArgs() {

  if (nArgs < 2) {Usage();  return false;}             // Did they specify enough arguments?
  if (argX >= nArgs)        return false;

  init();

  for (; argX < nArgs; argX++) {

    TCchar*& arg = args[argX];

    if ((arg[0] != _T('-')) && (arg[0] != _T('/'))) break;  // Switch?

    if (_tcschr(arg, _T('u'))) isSystemEnv = false;     // User variable

    if (_tcschr(arg, _T('s'))) operation = SetOP;        // Set key to value

    if (_tcschr(arg, _T('a'))) operation = AppendOp;     // Add variable / value

    if (_tcschr(arg, _T('d'))) operation = DeleteOp;     // Delete variable / value

    if (_tcschr(arg, _T('p'))) operation = PrePendOp;

    if (_tcschr(arg, _T('v'))) operation = ViewOp;

    if (_tcschr(arg, _T('c'))) operation = CopyOp;

    if (_tcschr(arg, _T('z'))) operation = SanitizeOp;

    if (_tcschr(arg, _T('h')) || _tcschr(arg, _T('?'))) {Usage(); argX++; return true;} // Help

    if (_tcschr(arg, _T('e')))                   {ShowExamples(); argX++; return true;} // Examples
    }

  if (argX < nArgs) name  = args[argX];  argX++;  trim(name);

  if (operation == ViewOp || operation == SanitizeOp) return true;

  if (operation == DeleteOp && args[argX][0] == _T('-')) return true;

  if (argX < nArgs) value = args[argX];  argX++;  trim(value);

  return true;
  }


bool Main::setEnv() {
Registry reg(isSystemEnv ? LocalMachine : CurrentUser, _T(';'));

  if (!reg.openTgt(name)) return false;

  if (isSystemEnv && operation != ViewOp) restart.elevate();

  switch (operation) {
    case SetOP      : return reg.setValue(value);
    case AppendOp   : return reg.appendValue(value);
    case DeleteOp   : return reg.deleteValue(value);
    case PrePendOp  : return reg.prefixValue(value);
    case ViewOp     : reg.displayTgt(); return true;
    case CopyOp     : return reg.copyTo(value);             // copy open variable to new key
    case SanitizeOp : return reg.sanitizeValue();           // ensure no duplicates
    default         : return reg.setValue(value);
    }

  reg.closeBase();
  }



static TCchar* usage[] = {
  _T("Allows you to set/update/delete User or System Environment Variables, e.g. PATH."),
  _T("For Windows XP/Win7/..."),
  _T(""),
  _T("Usage: SetEnv [-<options>] <name> [<value>]"),
  _T(""),
  _T("Options (Default is System variable, add 'u' for the User Environment):"),
  _T(""),
  _T("\tSetEnv       name value \tSet the system variable to the specified value"),
  _T("\tSetEnv -[u]s name value \tSet the [user] variable to the specified value"),
  _T("\tSetEnv -[u]a name value \tAppend the [user] variable to the specified value"),
  _T("\tSetEnv -[u]p name value \tPrepend a value to an [user] variable"),
  _T("\tSetEnv -[u]d name\t\tDelete the [user] variable"),
  _T("\tSetEnv -[u]d name value\t\tDelete the value from the [user] variable"),
  _T("\tSetEnv -[u]c name toVariable\tCopy the [user] variable 'name' to the variable [user] 'toVariable'"),
  _T("\tSetEnv -[u]v name \t\tDisplay name and value of environment variable"),
  _T("\tSetEnv -e\t\t\tShow examples of using SetEnv"),
  _T(""),
  _T("Notes:"),
  _T("\tAn \"expanded\" variable attribute is established for the variable if there is"),
  _T("\ta '%' character anywhere in the new value."),
  _T(""),
  _T("\tUse double quotes around any value which contains spaces.")
  };



// Display the application's usage information
// Now supports dynamic mode.
// Sets the new variable to be equal to the value of the %MEDIADIR% variable even if the MEDIADIR
// variable changes:
// E.g
//    setenv -a MEDIADIR D:\Media           // MEDIADIR = D:\Media
//    setenv -a MUSICDIR ~MEDIADIR~\Music   // MUSICDIR = D:\Media\Music (i.e. %MEDIADIR%\Music)
//    setenv -a MEDIADIR C:\Media           // MUSICDIR = C:\Media\Music

void Main::Usage() {
int    i;
int    n = noElements(usage);
String s;

  ShowCopyright();

  for (i = 0; i < n; i++) {s = usage[i]; s += _T("\n");  Output(s);}

  Output(_T("\nInput Enter"));  cin.get();
  }


static Tchar* examples[] = {
          _T("Examples:"),
          _T(""),
          _T("SetEnv -us INSTALLPATH C:\\SetEnv "),
          _T("\tSets the value of INSTALLPATH to \"C:\\SetEnv\" in the USER environment."),
          _T(""),
          _T("SetEnv -a PATH %D:\\Bin "),
          _T("\tAppends \"D:\\Bin\" to the current PATH in the MACHINE environment, will"),
          _T("\tcreate PATH if necessary."),
          _T(""),
          _T("SetEnv -p Path C:\\foo\\goo "),
          _T("\tPrepends (prefix) the value of Path to \"C:\\foo\\goo\" in the MACHINE environment."),
          _T("")
          };


void Main::ShowExamples() {
int    i;
int    n = noElements(examples);
String s;

  ShowCopyright();

  for (i = 0; i < n; i++) {
    s = examples[i]; s += _T("\n"); Output(s);
    }
  }



void Main::ShowCopyright() {
ResourceData res;
String       cr;
String       line = getNameVer(res);

  if (res.getCopyRight(cr)) line += _T(", Copyright ") + cr + _T("\n");
  Output(line);

  line = _T("Based on code written by Jonathan Wilkes around 2007\n\n");
  Output(line);
  }


String Main::getNameVer(ResourceData& res) {
String nameVer;
String version;

  if (!res.getProductName(nameVer)) nameVer = _T("Product");

  nameVer.trim();

  #ifdef WinXP
    nameVer += _T(" (WinXP)");
  #elif defined UNICODE
    nameVer += _T(" (UNI 7)");
  #else
    nameVer += _T(" (Win 7)");
  #endif

  if (!res.getVersion(version)) version = _T("0/0/0");

  res.threeSect(version);

  nameVer += _T(", Version ") + version;

  return nameVer.trim();
  }
