// My CSV Records


#pragma once
#include "Archive.h"
#include "ExpandableP.h"
#include "qsort.h"


/*

This template provides an array of records and the methods to load it from a Comma Separated Value (CSV)
file.  The record must be a sub-class of CSVRcdXB.  The array holds a pointer to the record object but
that is more or less hidden from the user.

The typedef defines the class and an object must be created to use it.  Here I've declared a global
object, csvRcdsX, which must be defined in a cpp file somewhere (usually with the same name as the header
file name).

Note, that the record does all the grunt work.  The functions in the template manages the array and call
the record for each action.

typedef RcdPtrT<CSVRcd> CSVRcdP;

typedef CSVRcdsXT<CSVRcd, CSVRcdP> CSVRcdsX;

extern CSVRcdsX csvRcdsX;

Note, that most of the methods in the CSVRcdsX class are decleared to be virtual so a sub-class may be
derived from it and more functions added or the the virtual class redefined.

typedef CSVRcdsXT<CSVRcd, CSVRcdP> CSVRcdsXB;

class CSVRcdsX : public CSVRcdsXB {

public:

  void newMethod() {...}
  void display() {... CSVRcdsXB::display(); ...}   // Add code before and/or after standard display
...

  };

extern CSVRcdsX csvRcdsX

Iterating through the array is performed using a template to define an iterator.

typedef CVSIterXT<CSVRcd, CSVRcdP> CVSIter;

Then when an iterator is needed the object is defined by naming the class that holds the array.  The loop
is constructed as shown below

CVSIter iter(cvsRcdsX>;
CSVRcd* rcd;

  for (rcd = iter(); rcd; rcd = iter++) { <use rcd as a pointer to the current record in the loop> }

*/


template<class CSVRcd, class CSVRcdP>
class CSVRcdsXT {
protected:

ExpandableP<CSVRcd, CSVRcdP, 2> data;

public:

  CSVRcdsXT() { }
 ~CSVRcdsXT() {clear();}

  virtual void clear() {data.clear();}

  virtual void load(Archive& ar);               // load csv data from file
  virtual void store(Archive& ar);              // store csv data in file (comman separated fields)

  virtual CSVRcd* get(int x) {return datum(x);}
  virtual CSVRcd* add() {return &data.nextData();}

  virtual CSVRcd* allocate() {return data.allocate();}

  virtual void operator() (int x, CSVRcd& d) {data(x, d);}
  virtual void del(int x) {data.del(x);}

  virtual void sort() {qsort(&data[0], &data[data.end()-1]);}

  virtual int  count() {return nData();}

  virtual void display();                       // display as comman separated fields


// returns either a pointer to data (or datum) at index i in array or zero

  CSVRcd* datum(int i) {return 0 <= i && i < nData() ? data[i].p : 0;}

  int     nData()      {return data.end();}                       // returns number of data items in array

  void    removeDatum(int i) {if (0 <= i && i < nData()) data.del(i);}
  };


template<class CSVRcd, class CSVRcdP>
class CVSIterXT : public CSVRcdsXT<CSVRcd, CSVRcdP> {

int        iterX;
CSVRcdsXT& store;

public:

enum Dir {Fwd, Rev};

  CVSIterXT(CSVRcdsXT& dataStore) : iterX(0),          store(dataStore)  { }
  CVSIterXT(CVSIterXT& iter)      : iterX(iter.iterX), store(iter.store) { }

  CSVRcd* operator() (Dir rev = Fwd) {iterX = rev ? store.nData() : 0; return rev ? decr() : current();}
  CSVRcd* operator++ (int) {return iterX < store.nData() ? incr() : 0;}
  CSVRcd* operator-- (int) {return iterX > 0             ? decr() : 0;}

  CSVRcd* current()        {return store.datum(iterX);}

  CVSIterXT& operator= (CVSIterXT& iter) {iterX = iter.iterX; store = iter.store;}

  bool  isLast()         {return iterX + 1 == store.nData();}
  bool  isFirst()        {return iterX <= 0;}

  void  remove(Dir rev = Fwd) {store.removeDatum(rev ? iterX++ : iterX--);}

private:

  CSVRcd* incr() {return iterX < store.nData() ? store.datum(++iterX) : 0;}
  CSVRcd* decr() {return iterX > 0             ? store.datum(--iterX) : 0;}

  CVSIterXT() : store(*(CSVRcdsXT*) 0), iterX(0) { }      // This prevents an uninitizlized iterator
  };


// load csv data from file

template<class CSVRcd, class CSVRcdP>
void CSVRcdsXT<CSVRcd, CSVRcdP>::load(Archive& ar) {
CSVLex lex(ar);

  while (lex.get_token() != EOFToken) {
    if (lex.token.code == EolToken) {lex.accept_token(); continue;}

    CSVRcd& rcd = data.nextData(); rcd.load(lex);
    }
  }


template<class CSVRcd, class CSVRcdP>
void CSVRcdsXT<CSVRcd, CSVRcdP>::store(Archive& ar) {
int     i;
CSVRcd* rcd;

  for (i = 0, rcd = datum(i); rcd; rcd = datum(++i)) rcd->store(ar);
  }


// display as comman separated field

template<class CSVRcd, class CSVRcdP>
void CSVRcdsXT<CSVRcd, CSVRcdP>::display() {
int     i;
CSVRcd* rcd;

  notePad.clear();

  notePad << _T("No Entries = ") << nData() << nCrlf << nCrlf;

  for (i = 0, rcd = datum(i); rcd; rcd = datum(++i)) rcd->display();
  }


