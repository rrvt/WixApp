// Capture Under Line Position in Archive Output and create an " ---" underline


#pragma once


class NoteUndrLn {

int    beginPos;                          // Underline begining position when >= 0
int    endPos;                            // Underline ending position
String s;

public:

          NoteUndrLn() : beginPos(0), endPos(0) { }
         ~NoteUndrLn() { }

  void    begin(int pos) {beginPos = pos;}
  void    end(  int pos) {endPos   = pos;}

  bool    isEmpty() {return endPos == beginPos;}

  String& operator() ();
  };

