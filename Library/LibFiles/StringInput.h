// String Input for Lex

#pragma once
#include "LexT.h"


struct StringInput : public Input {
long   i;
long   endN;
String stg;

  StringInput() {initialize();}

  void initialize() {i = endN = 0;}

  void set(String& s) {endN = (long) s.length(); stg = s; i = 0;}

  Tchar operator() () {
    return (Tchar) (i < endN ? stg[i] : 0);
    }

  void acceptChar()  {i++;}
  void backOneChar() {i--;}
  };

