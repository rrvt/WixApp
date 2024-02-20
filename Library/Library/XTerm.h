// Process (i.e. remove from stream) XTerm characters


#pragma once


struct XCmd {
Tchar ch;
Tchar prefix;
long val;
Tchar prefix2;
Tchar suffix;

  XCmd() {clearAll();}
 ~XCmd() {}

  void clearAll() {ch = prefix = prefix2 = suffix = 0; val = -1;}
  void clear() {if (suffix != ';') {prefix = prefix2 = 0;}   ch = suffix = 0; val = -1;}
  };


class XTerm {

enum State   {BeginState, EscapeSeen, LeftSqBkt, CollectInt};
enum ChClass {nulChClass, CtrlChar, Digit, Letter, SemiColon, EscapeChar, OtherChar};

static State  state;

public:

static bool   eolSeen;

  XTerm() {state = BeginState;}
 ~XTerm() {}

  static bool putChar(Tchar ch, XCmd& xCmd);    // If return v is true, xCmd.ch is a character
                                               // else sp.u.code.suffix is a code, v is optional
  };
