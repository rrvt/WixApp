// Process Plain Text for eMail


#include "pch.h"
#include "PlainTxt.h"
#include "EMailUtil.h"


TCchar* PlainTxt::get() {
Tchar ch;

  loop {
    switch (state) {
      case InitPlain  : text.clear();
                        i = 0;   n = body.length();   lineLng = 0;   state = CollPlain;    continue;

      case CollPlain  : ch = nextChar();   if (!ch) return text;   // Fall Thru into GotChPln

      case GotChPln   : if (ch == _T(' ')) {state = SpcPlain;   continue;}
                        if (ch == TEolCh)  {state = EolPlain;   continue;}
                        if (ch == _T('\\')) {state = SlashPlain; continue;}

                        frag += ch;   state = CollPlain;   continue;

      case SpcPlain   : state = CollPlain;   appdFragPlus(_T(" "));   continue;

      case EolPlain   : ch = nextChar();   if (!ch) return text;
                        if (appdFragPlus(_T(" "))) {state = ch == TEolCh ? EolNPlain : GotChPln;}
                        else if (ch == TEolCh)  {state = EolNPlain;   text += TCrlf;    lineLng = 0;}
                        else                     state = GotChPln;
                        continue;

      case EolNPlain  : ch = nextChar();   if (!ch) return text;
                        text += TCrlf;   lineLng = 0;   if (ch != TEolCh) state = GotChPln;   continue;

      case SlashPlain : ch = nextChar();   if (!ch) return text;
                        state = CollPlain;   fragLng = frag.length();

                        if (ch == _T('n')) {appdFragPlus(TCrlf);   state = CollPlain;  continue;}
                        appdFragPlus(_T("\\"));   state = GotChPln;   continue;
      }
    }
  }


Tchar PlainTxt::nextChar() {

  if (i >= n) {appdFragPlus(TCrlf);   return 0;}

  return body[i++];
  }


bool PlainTxt::appdFragPlus(TCchar* tc) {
int tcLng = tc ? _tcslen(tc) : 0;
bool brk  = false;

  fragLng = frag.length();

  if (lineLng + fragLng  + tcLng >= Threshold)
                                        {text += _T(' ');    text += TCrlf;   lineLng = 0;   brk = true;}

  text += frag + tc;   lineLng += fragLng + tcLng;    frag.clear();   return brk;
  }

