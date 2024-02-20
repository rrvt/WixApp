// EMail -- Interface to libcurl library


#pragma once
#include "curl/curl.h"
#include "Date.h"


typedef struct curl_slist CurlSList;


class EMail {

String     eMailHost;
CURL*      curl;
CurlSList* sList;
int        textX;
String     text;

public:

String     fromText;              // Required: Robert <rrvtmg@gmail.com>
String     toText;                // Required: Bob Van Tuyl <bobvantuyl8@gmail.com>
String     ccText;                // Optional
String     replyToText;           // Optional
String     subject;               // Optional
String     bodyTxt;               // Optional
String     bodyHtm;

          EMail() : curl(0), sList(0), textX(0) { }
         ~EMail();

  bool    login(TCchar* host, TCchar* port, TCchar* id, TCchar* pswd);

  void    clearRecipients();
  TCchar* from(TCchar* addr);
  TCchar* recipient(TCchar* addr);

  bool    sendMsg();

  bool    display();

private:

  bool    getText();
  Tchar   getChar();

  void    appd(bool eol, ...);

  static size_t payload_source(char* ptr, size_t size, size_t nmemb, void* userp);

  bool dspError(CURLcode res);
  };





//  TCchar* getBodyHtm();

//  bool    getPlainText(String& plainText);
//  bool    getHtmlText( String& htmlText);

