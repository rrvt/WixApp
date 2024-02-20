// Process Plain Text for eMail


#pragma once


class PlainTxt {
enum State {InitPlain, CollPlain, GotChPln, SpcPlain, EolPlain, EolNPlain, SlashPlain};
String  body;
State   state;
String  frag;
int     fragLng;
int     i;
int     n;
int     lineLng;
String  text;

public:

  PlainTxt(String& input) : body(input), state(InitPlain), i(0), n(0), lineLng(0) { }
 ~PlainTxt() { }

  TCchar* get();

private:

  Tchar nextChar();
  bool  appdFragPlus(TCchar* tc);

  PlainTxt() : body(*(String*) 0) {  }
  };


