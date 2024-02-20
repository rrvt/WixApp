// Process Html Text for eMail



#include "pch.h"
#include "HtmTxt.h"
#include "EMailUtil.h"



static TCchar* NBlnkSpc   = _T("&nbsp; ");     // Non break space, only added when two spaces occur
static int     NBlnkSpcLng= 7;
static TCchar* EoParaHtm  = _T("<br>");        // End of Paragraph
int            EolParaLng = 4;

// _T("<a href=\"mailto:membership@sjraces.org\">membership@sjraces.org</a>")"


TCchar* HtmTxt::get() {
Tchar ch;

  loop {
    switch (state) {
      case InitHtml : i = 0;   n = body.length();   state = CollHtml;    continue;

      case CollHtml : ch = nextChar();   if (!ch) return text;   // Fall Thru into GotChHtml

      case GotChHtml: if (ch == _T(' '))  {state = SpcHtml;   continue;}
                      if (ch == TEolCh)   {state = EolHtml;   continue;}
                      if (ch == _T('\\')) {state = SlashHtml; continue;}

                      frag += ch;   state = CollHtml;   continue;

      case SpcHtml  : ch = nextChar();   if (!ch) return text;
                      if (ch == _T(' ')) {state = Spc2Html;   continue;}
                      appdFragPlus(_T(" "));   state = GotChHtml;   continue;

      case Spc2Html : appdFragPlus(NBlnkSpc);   state = CollHtml;   continue;

      case EolHtml  : ch = nextChar();   if (!ch) return text;

                      if (ch == TEolCh) {state = EolNHtml;   continue;}

                      appdFragPlus(_T(" ")); state = GotChHtml;   continue;

      case EolNHtml : state = CollHtml;   appdFragPlus(EoParaHtm);   addNewLine();

                      appdFragPlus(EoParaHtm);   addNewLine();   continue;

      case SlashHtml: ch = nextChar();   if (!ch) return text;

                      if (ch == _T('n')) {appdFragPlus(EoParaHtm);   addNewLine();   state = CollHtml;}
                      else               {appdFragPlus(_T("\\"));    state = GotChHtml;}
                      continue;
      }
    }
  }


Tchar HtmTxt::nextChar() {
  if (i >= n) {appdFragPlus(EoParaHtm);   addNewLine();   return 0;}

  return body[i++];
  }


void HtmTxt::substitute(TCchar* txt, TCchar* forTxt, String& body) {
String s;
int    pos;
int    len;

  if (!forTxt || !(len = _tcslen(forTxt))) return;

  for (pos = body.find(forTxt); pos >= 0; pos = body.find(forTxt))
                                       {s = body.substr(0, pos);   body = s + txt + body.substr(pos+len);}
  }


static TCchar* HtmlTab    = _T("    ");
static int     HtmlTabLng = 4;


bool HtmTxt::appdFragPlus(TCchar* tc) {
int tcLng = tc ? _tcslen(tc) : 0;
bool brk  = false;

  fragLng = frag.length();

  if (lineLng + fragLng  + tcLng >= Threshold) {addNewLine();   brk = true;}

  if (!lineLng) {text += HtmlTab;   lineLng = HtmlTabLng;}

  text += frag + tc;   lineLng += fragLng + tcLng;    frag.clear();   return brk;
  }


void HtmTxt::addNewLine() {text += TCrlf;   lineLng = 0;}

