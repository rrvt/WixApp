// CSV Output (takes care of internal commas, quotes, etc...

/*
The template for loading, storing and accessing CSV files is contained in several files:
  *  CSVLex   -- A Lexical Analyser (FSM) for reading CSV files
  *  CSVOut   -- Contains a class for outputing to a CSV file (managing commas and quotes withing fields)
  *  CSVRcdB  -- A base file for your CSV Record to provide basic functionality
  *  CSVRcdsT -- Contains a template for creating a container for CSV Records.  May be used as a base
                 class for extending functionality with a subclass.

This class provides a simple mechansim for outputing to a CSV file.  It allos the use of the "<<"
(streaming) operator and handles comma and quotes withing the items output.  Each item output is
considered to be a separate field in the CSV file.
*/


#pragma once
#include "Archive.h"
#include "Date.h"


extern TCchar Comma;


class CSVOut;

typedef ManipT<CSVOut> CSVManip;

class CSVOut {
Archive& ar;

public:

  CSVOut(Archive& a) : ar(a) {initialize();}

  CSVOut& operator<< (String&    s)     {ar.write(quotes(s));              return *this;}
  CSVOut& operator<< (TCchar*    p)     {ar.write(quotes(p));              return *this;}
  CSVOut& operator<< (Tchar     ch)     {ar.write(ch);                     return *this;}
  CSVOut& operator<< (int        x)     {String s = x;        ar.write(s); return *this;}
  CSVOut& operator<< (Date&     dt)     {String s; dt >> s;   ar.write(s); return *this;}

  CSVOut& operator<< (CSVManip& m)      {return m.func(*this);}

  void    crlf()                        {ar.crlf();}

private:

  static String& quotes(TCchar* p);

  void initialize();

  static CSVOut& doCrlf(CSVOut& n) {n.crlf(); return n;}

  CSVOut() : ar(*( Archive*) 0) { }
  };


extern CSVManip vCrlf;       // add to stream to terminate a line on display: ar << "xyz" << vCrlf;

