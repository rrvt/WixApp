// Debug Module for outputing print statements in a Message Box
// rrvt 05/12/2015


#pragma once

class Debug {
String hdr;
String text;

public:

int    count;
bool   printing;

       Debug() : count(0), printing(false) {}
      ~Debug() {}

  void title(String& h);
  void title(Tchar*  h);

  void stg(String& t);
  void stg(Tchar*  t);

  void num(String& t, int n);
  void num(Tchar*  t, int n);
  void num(String& t, double n);
  void num(Tchar*  t, double n);

  bool isPrinting()  {return printing;}
  void printingNow() {printing = true;}
  void notPrinting() {printing = false;}

private:

  void messageBox() {MessageBox(0, text, hdr, S_OK);}
  };


extern Debug dbg;
