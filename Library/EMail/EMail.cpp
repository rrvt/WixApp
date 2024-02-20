// EMail -- Interface to libcurl library


#include "pch.h"
#include "eMail.h"
#include "EMailUtil.h"
#include "Guid.h"
#include "HtmTxt.h"
#include "NotePad.h"
#include "PlainTxt.h"




/* curl will not send the QUIT command until you call cleanup, so you should be able to reuse this
 * connection for additional messages (setting CURLOPT_MAIL_FROM and CURLOPT_MAIL_RCPT as required, and
 * calling curl_easy_perform() again. It may not be a good idea to keep the connection open for a long
 * time though (more than a few minutes may result in the server timing out the connection), and you do
 * want to clean up in the end.  */

EMail::~EMail() {clearRecipients();   curl_easy_cleanup(curl);   curl = 0;}


bool EMail::login(TCchar* host, TCchar* port, TCchar* id, TCchar* pswd) {
String url;

  eMailHost = host;  url = _T("smtp.") + eMailHost + _T(":");   url += port;

  curl = curl_easy_init();                                              if (!curl)         return false;

  if (dspError(curl_easy_setopt(curl, CURLOPT_USE_SSL, (long) CURLUSESSL_ALL))) return false;

  ToAnsi urlA(url);     if (dspError(curl_easy_setopt(curl, CURLOPT_URL, urlA())))       return false;

  ToAnsi idA(id);       if (dspError(curl_easy_setopt(curl, CURLOPT_USERNAME, idA())))   return false;

  ToAnsi pswdA(pswd);   if (dspError(curl_easy_setopt(curl, CURLOPT_PASSWORD, pswdA()))) return false;

  return true;
  }


TCchar*  EMail::from(TCchar* addr)
      {ToAnsi addrA(addr);   dspError(curl_easy_setopt(curl, CURLOPT_MAIL_FROM, addrA()));   return addr;}


/* Add two recipients, in this particular case they correspond to the
* To: and Cc: addressees in the header, but they could be any kind of
* recipient. */

TCchar*  EMail::recipient(TCchar* addr)
                         {ToAnsi addrA(addr);   sList = curl_slist_append(sList, addrA());   return addr;}


/* Note that the From option is not strictly required, omitting it will result in libcurl sending the
 * MAIL FROM command with empty sender data. All autoresponses should have an empty reverse-path, and
 * should be directed to the address in the reverse-path which triggered them. Otherwise, they could cause
 * an endless loop. See RFC 5321 Section 4.5.5 for more details.  */

bool EMail::sendMsg() {
String guid;   getGuid(guid);
Date date;     date.getToday();

  if (!getText()) return false;

  if (dspError(curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, sList))) return false;

  /* We are using a callback function to specify the payload (the headers and
   * body of the message). You could just use the CURLOPT_READDATA option to
   * specify a FILE pointer to read from. */

  if (dspError(curl_easy_setopt(curl, CURLOPT_READFUNCTION, &payload_source))) return false;
  if (dspError(curl_easy_setopt(curl, CURLOPT_READDATA,     this)))            return false;
  if (dspError(curl_easy_setopt(curl, CURLOPT_UPLOAD,       1)))               return false;

  textX = 0;

  if (dspError(curl_easy_perform(curl))) return false;

  return true;
  }


size_t EMail::payload_source(char* ptr, size_t size, size_t nmemb, void* classPtr) {
EMail*  eMail = (EMail*) classPtr;
String& text  = eMail->text;
int&    textX = eMail->textX;
int     lng   = text.length() - textX;
int     max   = size * nmemb;   if (lng < max) max = lng;

  if (max <= 0) return 0;

  for (int i = 0; i < max; i++, textX++) *ptr++ = (char)text[textX];

  return max;
  }


// Clear Body between eMails

void EMail::clearRecipients() {toText.clear();  ccText.clear();   curl_slist_free_all(sList);  sList = 0;}


bool EMail::dspError(CURLcode res) {

  if (!res) return false;

  notePad << "curl_easy_perform() failed: %s\n" << curl_easy_strerror(res) << nCrlf; return true;
  }


bool EMail::display() {
String t;
String s;
int pos;

  if (!getText()) return false;

  for (t = text, pos = t.find(TCrlf); pos >= 0; pos = t.find(TCrlf)) {
    s = t.substr(0, pos);   t = t.substr(pos+2);
    notePad << s << nCrlf;
    }

  if (!t.isEmpty()) notePad << t << nCrlf;
  return true;
  }
                                                    // multipart/mixed   /alternative

static TCchar* MIME        = _T("MIME-Version: 1.0");
static TCchar* MultiPart   = _T("Content-Type: multipart/alternative;");
static TCchar* BoundaryDcl = _T(" boundary=");
static TCchar* Boundary    = _T("------------nqEkFMiY4N0FAmg2Or4yfvD2");
static TCchar* Format      = _T("This is a multi-part message in MIME format.");
static TCchar* PlainType   = _T("Content-Type: text/plain; charset=UTF-8");
static TCchar* Flowing     = _T("; format=flowed");
static TCchar* Encoding    = _T("Content-Transfer-Encoding: 8bit");
static TCchar* Quote       = _T("\"");
static TCchar* Language    = _T("Content-Language: en-GB");


static TCchar* HtmlType  = _T("Content-Type: text/html; charset=UTF-8");
static TCchar* DocType   = _T("<!DOCTYPE html>");
static TCchar* Html      = _T("<html>");
static TCchar* HtmlCls   = _T("</html>");
static TCchar* Head      = _T("<head>");
static TCchar* Meta      = _T("<meta http-equiv=\"content-type\" content=\"text/html; charset=UTF-8\">");
static TCchar* HeadCls   = _T("</head>");
static TCchar* Body      = _T("<body>");
static TCchar* BodyCls   = _T("</body>");
static TCchar* HtmTab    = _T("  ");


bool EMail::getText() {
String   guid;      getGuid(guid);
Date     date;      date.getToday();
PlainTxt plainTxt(bodyTxt);
HtmTxt   htmTxt(bodyHtm);

  if (toText.isEmpty() || fromText.isEmpty()) return false;

  text.clear();

  appd(true, MultiPart, 0);
  appd(true, BoundaryDcl, Quote, Boundary, Quote, 0);
  appd(true, _T("Message-ID: <"), guid.str(), _T("@"), eMailHost.str(), _T(">"), 0);

  text += date.format(_T("Date: %a, %e %b %Y %T %z\r\n"));
  appd(true, MIME, 0);

  if (!replyToText.isEmpty()) appd(true, _T("Reply-To: "), replyToText.str(), 0);

  appd(true, Language, 0);

  appd(true, _T("To: "), toText.str(), 0);

  appd(true, _T("From: "), fromText.str(), 0);

  if (!ccText.isEmpty()) appd(true, _T("Cc: "), ccText.str(), 0);

  if (!subject.isEmpty()) appd(true, _T("Subject: "), subject.str(), 0);

  text += TCrlf;

  appd(true, Format, 0);
  appd(true, _T("--"), Boundary, 0);

  appd(true, PlainType, Flowing, 0);
  appd(true, Encoding, 0);
  text += TCrlf;

  if (!bodyTxt.isEmpty()) appd(true, plainTxt.get(), 0);

  appd(true, _T("--"), Boundary, 0);
  appd(true, HtmlType, 0);
  appd(true, Encoding, 0);

  appd(true, DocType, 0);
  appd(true, Html, 0);
  appd(true, HtmTab, Head, 0);
  appd(true, 0);
  appd(true, HtmTab, HtmTab, Meta, 0);
  appd(true, HtmTab, HeadCls, 0);
  appd(true, HtmTab, Body, 0);

  if (!bodyHtm.isEmpty()) appd(false, htmTxt.get(), 0);

  appd(true, HtmTab, BodyCls, 0);
  appd(true, HtmlCls, 0);

  appd(true, _T("--"), Boundary, _T("--"), 0);

  return true;
  }


void EMail::appd(bool eol, ...) {
va_list args;
TCchar* tc;

  va_start(args, eol);

  for (tc = va_arg(args, TCchar*); tc; tc = va_arg(args, TCchar*)) text += tc;

  va_end(args);

  if (eol) text += TCrlf;
  }







#if 0
notePad << nClrTabs << nSetRTab(20) << nCrlf << _T("Payload") << nCrlf;
notePad << _T("Size:") << nTab << size << nCrlf;
notePad << _T("nmemb:") << nTab << nmemb << nCrlf;
notePad << _T("Lng:") << nTab << lng << nCrlf;
notePad << _T("max:") << nTab << max << nCrlf;
#endif


#if 0



enum HtmState {BolSt, CopySt, EolSt, SpcSt};


TCchar* EMail::getBodyHtm() {
int      i;
int      n;
Tchar    ch;
HtmState state = BolSt;

  bodyHtm.clear();

  for (n = body.length(), i = 0; i < n; i++) {
    ch = body[i];

    switch (state) {
      case BolSt  : bodyHtm += _T("    ");   state = CopySt;
//                  if (ch == _T('\r')) {state = EolSt; bodyHtm += BrHtm; break;}
//                  if (ch == _T(' '))  {state = SpcSt; continue;}
//                  break;

      case CopySt : if (ch == _T(' '))  {state = SpcSt; continue;}
                    if (ch == _T('\r')) {state = EolSt; bodyHtm += BrHtm;}
                    break;

      case SpcSt  : if (ch == _T('\r')) {state = EolSt;  break;}
                    state = CopySt;
                    if (ch == _T(' '))  {bodyHtm += Period;   break;}
                    bodyHtm += _T(' ');  break;

      case EolSt  : if (ch == _T('\n')) {state = BolSt; break;}
                    state = CopySt; break;
      }

    bodyHtm += ch;
    }

  bodyHtm += BrHtm;   bodyHtm += TCrlf;   return bodyHtm;
  }
#endif

#if 0
// "Date: Sun, 21 Jan 2024 00:20:00 +1100\r\n

void EMail::addToday() {
Date date;

  date.getToday();   text += date.format(_T("Date: %a, %e %b %y %T %z\r\n"));
  }


void EMail::addGuid() {
String guid;

  getGuid(guid);

  text += _T("Message-ID: <") + guid + _T('@') + eMailHost + _T("\r\n");
  }
#endif
  //static TCchar  Funny[9]    = {0xf0, 0x9f, 0x98, 0x82, 0xf0, 0x9f, 0x98, 0x82, 0};
#if 0
  appd(true, Funny, 0);
  text += TEol;
#endif

#if 0
  if (toText.isEmpty() || fromText.isEmpty()) return false;

  text.clear();
  text += date.format(_T("Date: %a, %e %b %y %T %z\r\n"));
  text += _T("From: ")     + fromText    + TEol;
  text += _T("To: ")       + toText      + TEol;
  if (!ccText.isEmpty())      text += _T("Cc: ")       + ccText      + TEol;
  if (!replyToText.isEmpty()) text += _T("Reply-To: ") + replyToText + TEol;
  text += _T("Message-ID: <") + guid + _T('@') + eMailHost + TEol;
  if (!subject.isEmpty())     text += _T("Subject: ")  + subject     + TEol;
  text += TEol;
  if (!body.isEmpty()) text += body + TEol;

  ToAnsi fromA(from);   if (dspError(curl_easy_setopt(curl, CURLOPT_MAIL_FROM, fromA()))) return false;

  /* Add two recipients, in this particular case they correspond to the
   * To: and Cc: addressees in the header, but they could be any kind of
   * recipient. */

//  ToAnsi toA(to);   recipients = curl_slist_append(recipients, toA());
//  if (!cc.isEmpty()) {ToAnsi ccA(cc);   recipients = curl_slist_append(recipients, ccA());}
#endif
#if 0
                        fragLng = frag.length();
                        if (lineLng + fragLng  + 1 < Threshold)
                          {text += frag + _T(' ');   lineLng += fragLng + 1;   frag.clear();   continue;}

                        text += _T(' ');    text += TCrlf;
                        text += frag + _T(' ');   lineLng = fragLng + 1;    frag.clear();    continue;
#endif
#if 1
#else
                        fragLng = frag.length();
                        if (lineLng + fragLng + 1 >= Threshold) {
                          text += _T(' ');   text += TCrlf;
                          text += frag + _T(' ');   lineLng = fragLng + 1;   frag.clear();
                          state = ch == TEolCh ? EolNPlain : GotChPln;   continue;
                          }


                          text += frag + _T(' ');   lineLng += fragLng + 1;    frag.clear();
                          if (ch == TEolCh) {state = EolNPlain;   text += TCrlf;    lineLng = 0;}
                          else               state = GotChPln;
                          continue;

                        text += _T(' ');   text += TCrlf;
                        text += frag + _T(' ');   lineLng = fragLng + 1;   frag.clear();
                        state = ch == TEolCh ? EolNPlain : GotChPln;   continue;
#endif
#if 1
#else
                      fragLng = frag.length();   state = GotChHtml;
                      if (lineLng + fragLng  + 1 < Threshold)
                        {text += frag + _T(' ');   lineLng += fragLng + 1;   frag.clear();   continue;}

                      text += _T(' ');    addNewLine();
                      text += frag + _T(' ');   lineLng += fragLng + 1;    frag.clear();    continue;
#endif
#if 1
#else
                      fragLng = frag.length();
                      if (lineLng + fragLng + NBlnkSpcLng >= Threshold) {addNewLine();}

                        text += frag + NBlnkSpc;;
                        lineLng += fragLng + NBlnkSpcLng;   frag.clear();   continue;
#endif
#if 1
#else
                      fragLng = frag.length();
                      if (lineLng + fragLng + NBlnkSpcLng >= Threshold) {addNewLine();}

                        text += frag + NBlnkSpc;;
                        lineLng += fragLng + NBlnkSpcLng;   frag.clear();   continue;
#endif
#if 1
#else
                      fragLng = frag.length();

                      if (lineLng + fragLng + EolParaLng >= Threshold) addNewLine();

                      text += frag + EoParaHtm;  frag.clear();
#endif

