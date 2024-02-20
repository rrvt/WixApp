// CSV Record Base Class

/*
The template for loading, storing and accessing CSV files is contained in several files:
  *  CSVLex   -- A Lexical Analyser (FSM) for reading CSV files
  *  CSVOut   -- Contains a class for outputing to a CSV file (managing commas and quotes withing fields)
  *  CSVRcdB  -- A base file for your CSV Record to provide basic functionality
  *  CSVRcdsT -- Contains a template for creating a container for CSV Records.  May be used as a base
                 class for extending functionality with a subclass.

This class provides a base class for a record defined by the user.  Nearly all the virtual classes must
be provided in a subclass.
*/


#pragma once
#include "Archive.h"
#include "CSVLex.h"


class CSVRcdB {
protected:
int    putI;                                // Intended to be used only by the put function
int    getI;                                // Intended to be used only by the get function

public:

  CSVRcdB() : putI(0), getI(0) { }
  virtual ~CSVRcdB() { }

  virtual void clear() {putI = getI = 0;}

  virtual bool load(CSVLex& lex);

  virtual int  display(NotePad& np);

  virtual void store(Archive& ar);

  virtual CSVRcdB& operator= (CSVRcdB& r) {copy(r); return *this;}

  virtual bool     operator== (CSVRcdB& r) {return false;}
  virtual bool     operator!= (CSVRcdB& r) {return false;}
  virtual bool     operator>  (CSVRcdB& r) {return false;}
  virtual bool     operator<  (CSVRcdB& r) {return false;}
  virtual bool     operator>= (CSVRcdB& r) {return false;}
  virtual bool     operator<= (CSVRcdB& r) {return false;}

protected:

  virtual void    put(TCchar* p) { }

  virtual String* get() {return 0;}

  virtual void    copy(CSVRcdB& r) {putI = r.putI; getI = r.getI;}
  };


