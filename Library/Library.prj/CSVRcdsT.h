// CSV Records

/*
The template for loading, storing and accessing CSV files is contained in several files:
  *  CSVLex   -- A Lexical Analyser (FSM) for reading CSV files
  *  CSVOut   -- Contains a class for outputing to a CSV file (managing commas and quotes withing fields)
  *  CSVRcdB  -- A base file for your CSV Record to provide basic functionality
  *  CSVRcdsT -- Contains a template for creating a container for CSV Records.  May be used as a base
                 class for extending functionality with a subclass.

This class provides a template for a container of CSV records or a base class for a container of CSV
records.  In addition there is a template to create an iterator for the accessing each record in the
container.

This container uses the RcdPtrT template which is contained in the ExpandableP.h file.  The record pointer
class allows pointers to be housed in the array and manipulated rather than the whole record being in
the array.  Ultimately this is for efficiency reasons.

Using the CSVRcdsT template is as follows:

  class Datum {               // This is the record that holds one line of CSV data
    ...
    };

  typedef RcdPtrT< Datum> CSVRcdP;
  typedef CSVRcdsT<Datum, CSVRcdP> CSVRcdsB;
  typedef CSVIterT<Datum, CSVRcdP> CSVIter;


  class CSVRcds : public CSVRcdsB {
    ...       // Additional functionality
    };
*/


#pragma once
#include "Archive.h"
#include "ExpandableP.h"


template<class CSVRcd, class CSVRcdP>
class CSVRcdsT {
protected:

ExpandableP<CSVRcd, CSVRcdP, 2> data;

public:

  CSVRcdsT() { }
 ~CSVRcdsT() {clear();}

  void clear() {data.clear();}

  void load(Archive& ar);               // load csv data from file
  void store(Archive& ar);              // store csv data in file (comman separated fields)

  void del(int x) {data.del(x);}

  int  count() {return nData();}

// returns either a pointer to data (or datum) at index i in array or zero

  CSVRcd* datum(int i) {return 0 <= i && i < nData() ? data[i].p : 0;}

  void display(NotePad& np);            // display as comman separated fields

protected:

  virtual CSVRcd* add() {return &data.nextData();}

public:

  int   nData()      {return data.end();}                       // returns number of data items in array

  void  removeDatum(int i) {if (0 <= i && i < nData()) data.del(i);}
  };


template<class Data, class CSVRcdP>
class CSVIterT : public CSVRcdsT<Data, CSVRcdP> {

int       iterX;
CSVRcdsT& store;

public:

enum Dir {Fwd, Rev};

  CSVIterT(CSVRcdsT& dataStore) : iterX(0),         store(dataStore)  { }
  CSVIterT(CSVIterT& iter)      : iterX(iter.iterX), store(iter.store) { }

  Data* operator() (Dir rev = Fwd) {iterX = rev ? store.nData() : 0; return rev ? decr() : current();}
  Data* operator++ (int) {return iterX < store.nData() ? incr() : 0;}
  Data* operator-- (int) {return iterX > 0             ? decr() : 0;}

  Data* current()        {return store.datum(iterX);}

  CSVIterT& operator= (CSVIterT& iter) {iterX = iter.iterX; store = iter.store;}

  bool  isLast()         {return iterX + 1 == store.nData();}
  bool  isFirst()        {return iterX <= 0;}

  void  remove(Dir rev = Fwd) {store.removeDatum(rev ? iterX++ : iterX--);}

private:

  Data* incr() {return iterX < store.nData() ? store.datum(++iterX) : 0;}
  Data* decr() {return iterX > 0             ? store.datum(--iterX) : 0;}

  CSVIterT() : store(*(CSVRcdsT*) 0), iterX(0) { }      // This prevents an uninitizlized iterator
  };


// load csv data from file

template<class CSVRcd, class CSVRcdP>
void CSVRcdsT<CSVRcd, CSVRcdP>::load(Archive& ar) {
CSVLex lex(ar);

//clear();            In order to merge multiple files together, do clear() explicitly

  while (lex.get_token() != EOFToken) {
    if (lex.token.code == EolToken) {lex.accept_token(); continue;}

    CSVRcd* rcd = data.allocate();   rcd->load(lex);   data += rcd;
    }
  }


template<class CSVRcd, class CSVRcdP>
void CSVRcdsT<CSVRcd, CSVRcdP>::store(Archive& ar) {
int     i;
CSVRcd* rcd;

  for (i = 0, rcd = datum(i); rcd; rcd = datum(++i)) rcd->store(ar);
  }


// display as comman separated field

template<class CSVRcd, class CSVRcdP>
void CSVRcdsT<CSVRcd, CSVRcdP>::display(NotePad& np) {
int     i;
CSVRcd* rcd;

  for (i = 0, rcd = datum(i); rcd; rcd = datum(++i)) rcd->display(np);
  }


