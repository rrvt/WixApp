// Process Html Text for eMail


#pragma once



class HtmTxt {
enum State {InitHtml, CollHtml, GotChHtml, SpcHtml, Spc2Html, EolHtml, EolNHtml, SlashHtml};
String  body;
State   state;
String  frag;
int     fragLng;
int     i;
int     n;
int     lineLng;
String  text;

public:

  HtmTxt(String& bodyHtm) : body(bodyHtm), state(InitHtml), i(0), n(0), lineLng(0) { }
 ~HtmTxt() { }

  TCchar* get();
  void    addNewLine();

private:

  Tchar nextChar();
  void  substitute(TCchar* txt, TCchar* forTxt, String& body);

  bool  appdFragPlus(TCchar* tc);

  HtmTxt() : body(*(String*)0) { }
  };



