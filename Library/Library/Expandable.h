// Expandable Array
// Copyright Software Design & Engineering, Robert R. Van Tuyl, 2013.  All rights reserved.

/*
Datum is a class, e.g. class Datum {...}, and requires the following methods:
 -- Constructor with no argument, e.g. Datum(), which initializes all components of the class
 -- Copy Constructor, e.g. Datum(const Datum& d), which copies all components of object d to
    object *this
 -- Assignment operator, e.g. Datum& operator= (Datum& d), which copies all components of object d
    to object *this and returns *this
 -- Destructor, e.g. ~Datum() that releases objects obtained from the heap and zeros all data
    components

The following methods are required in Datum if the corresponding Expandable data method is needed:
 -- bool operator>= (Datum& d);     // Required for data = datum (Insertion Sort)
 -- bool operator<= (Datum& d);     // Required for qsort
 -- bool operator>  (Datum& d);     // Required for qsort

 -- bool operator== (Key  key);     // Required for linear(Key key) and bSearch(Key key)
 -- bool operator>  (Key  key);     // Required for bSearch(Key key)
 -- bool operator<  (Key  key);     // Required for bSearch(Key key)

Interator

The easiest way to loop through all the elements in the array is to use an iterator.  It IterT.h
file contains a template for creating an iterator for Expandable files.  First one must declare
the Iterator class so that an object may be created somewhere that it is going to be used.  The
Datum class is a record that is stored in an expandable array which is housed in class Xyz

  class Xyz;
  typedef IterT<Xyz, Datum> XyzIter;                        // Iterator for the Xyz

Now that is not all there is to do.  The Xyz class must implement the following:

// int    nData()            -- returns number of data items in array
// Datum* datum(int i)       -- returns either a pointer to data (or datum) at index i in array or
                                zero
// void   removeDatum(int i) -- if i in bounds, removes and deallocates record
// friend typename DataIter  -- required to give access to private area of Xyz.

class Xyz {
  ooo
private:

  // returns either a pointer to datum at index i in array or zero

  Datum* datum(int i) {return 0 <= i && i < nData() ? &data[i] : 0;}

  int    nData()      {return data.end();}                // Returns number of data items in array,
                                                          // not necessarily private
  void   removeDatum(int i) {if (0 <= i && i < nData()) data.del(i);}

  friend typename XyzIter;
  };

Once the iterator class is defined then the following is how it would be used:

   XyzIter iter(xyz);                       // Where: Xyz xyz;  (i.e. xyz is an object of class Xyz
   Datum*  datum;                           // May be iter(*this) in an Xyz method

     for (datum = iter(); datum; datum = iter++) {
       <Use datum as a pointer to the record, it is guaranteed to be non-zero>
       }


The operations supported by an Expandable array where the declaration is:

  Expandable<Datum, 2> data;
  Datum                datum;

the operations supported are:

  data1    = data;                  // Copies all the current elements in data to data1, data is
                                    // unchanged
  datum    = data[i];               // where 0 <= i < endN
  data[i]  = datum;                 // array expands to encompass i
  data.clear();                     // content is ignored but number of elements is set to zero
  data     = datum;                 // datum is copied into the sorted array at the correct
                                    // position.  Requires datum >= datum1
  data    += datum;                 // datum is appended to array (at new last element)
  Datum& d = data.nextData();       // A reference to new last element of array is returned.  It
                                    // may used as shown or immedialy with a dot operator or even
                                    // as a target of an assignment (where a Datum operator= is
  data.nextData() = datum;          // defined)
  data(i, datum);                   // datum is inserted at index i, the contents at i and above
                                    // are moved up one element
  data.del(i);                      // The datum at index i is deleted and the elements above are
                                    // moved down to fill in the hole.  The number of elements in
                                    // the array is reduced by one
  data.find(key)                    // Performs a linear search for key.
                                    // Requires datum == Key object,
                                    // where Key is a type found in Datum object
  data.bsearch(key)                 // Performs a binary search for key.  Requires a sorted array
                                    // and the boolean operations datum > key, datum < key,
                                    // datum == key
*/


#pragma once


#define ExpandableException _T("Corrupted Expandable(P) structure")


template <class Datum, const int n>

class Expandable {
int    endN;                          // Number of items in the array that may be occupied
int    tblN;                          // total number of elements in the table
Datum* tbl;                           // Pointer to a heap object which is treated as an array of
                                      // Datums
public:

  Expandable();                                 // Constructor & destructor
 ~Expandable();

 void clear();                                  // Destructs each element in array
 int  end()   {return endN;}                    // Returns number of elements in array

  Expandable& operator=  (Expandable& e);       // Copy from one array to another
  Expandable& operator+= (Expandable& e);       // Appends the data from the e array to "this"
                                                // array
  Expandable& operator-= (Expandable& e);       // Move from e to "this"

  Datum& operator[] (int i);                    // Index into array, returns a reference

  // Insert every Datum d into array sorted (being sure to expand it if necessary.
  // Requires datum >= datum1 method.   Note, if one use [] to insert data into array, sorting is
  // up to the user...!

  Datum* operator= (Datum& d);

  Datum* operator+= (Datum& d);                 // Append Datum to end of array, copies datum into
                                                // array entry

  Datum& nextData() {return (*this)[endN];}     // Return reference to next available node in array
                                                // (at end)
  Datum& getData(int i);                        // Returns reference to a node at index i or the
                                                // end of the vector, allocating it if necessary
  bool   operator() (int x, Datum& d);          // Insert data at index, moving other entries out
                                                // of the way

  void   push(Datum& d) {*this += d;}           // Push node onto the end of the table
  bool   pop(Datum& d);                         // Copy the node at the end of the array into d
                                                // and remove node from the end of the array

  bool   del(int x);                            // Delete datum at index x, adjust array to fill
                                                // gap, clear last entry

  template<class Key> Datum* find(Key key);     // Linear search. Requires datum == key method

  template<class Key> Datum* bSearch(Key key);  // Binary search (only works on sorted array,
                                                // Requires datum > key, datum < key, datum == key

  void   probe(TCchar* title);                  // Display internal information on notePad

private:

  void append(Expandable& e);                   // Append all elements of array e to array a

  void expand(int i);                           // Expand array when required
  };


// Constructor

template <class Datum, const int n>
Expandable<Datum, n>::Expandable() : endN(0), tblN(n > 0 ? n : 1)
                                                         {NewArray(Datum); tbl = AllocArray(tblN);}

// Destructor

template <class Datum, const int n>
Expandable<Datum, n>::~Expandable()
                         {if (tbl) {NewArray(Datum);  FreeArray(tbl);}   tbl = 0; endN = tblN = 0;}


template <class Datum, const int n>
void Expandable<Datum, n>::clear() {for (int i = 0; i < endN; i++) tbl[i].~Datum();   endN = 0;}



// copy data from one array to another

template <class Datum, const int n>
Expandable<Datum, n>& Expandable<Datum, n>::operator= (Expandable& e)
                                                                {clear(); append(e); return *this;}



// Append data from on array to another

template <class Datum, const int n>
Expandable<Datum, n>& Expandable<Datum, n>::operator+= (Expandable& e) {append(e);  return *this;}
                                              // Appends the data from the e array to "this" array

template <class Datum, const int n>
Expandable<Datum, n>& Expandable<Datum, n>::operator-= (Expandable& e)    // Move from e to "this"
                                               {clear();   append(e);   e.clear();   return *this;}



// Index into array, returns a reference

template <class Datum, const int n>
Datum& Expandable<Datum, n>::operator[] (int i) {
  if (i >= tblN) expand(i);
  if (i >= endN) endN = i+1;
  return tbl[i];
  }


// Insert every Datum d into array sorted (being sure to expand it if necessary.  Note, if one
// use [] to insert data into array, sorting is up to the user...!

template <class Datum, const int n>
Datum* Expandable<Datum, n>::operator= (Datum& d) {
Datum  xNode;
Datum  nextNode;
int    i;
Datum* dtm;

  for (i = endN-1; i >= 0; i--) {dtm = &tbl[i];   if (d >= *dtm) {i++;   break;}}

  if (i < 0) i = 0;   dtm = &tbl[i];

  for (xNode = d, endN++; i < endN; i++)
             {if (i >= tblN) expand(i);   nextNode = tbl[i];   tbl[i] = xNode;   xNode = nextNode;}
  return dtm;
  }


// Append Datum to end of array, copies datum into array entry

template <class Datum, const int n>
Datum* Expandable<Datum, n>::operator+= (Datum& d)
                                        {Datum& datum = (*this)[endN];  datum = d;  return &datum;}

// Insert data at index, moving other entries out of the way

template <class Datum, const int n>
bool Expandable<Datum, n>::operator() (int x, Datum& d) {
int i;

  if (x < 0 || endN < x) return false;

  if (++endN >= tblN) expand(x);

  for (i = endN-2; i >= x; i--) tbl[i+1] = tbl[i];

  tbl[x] = d;   return true;
  }


// Returns reference to a node at index i or the end of the vector, allocating it if necessary

template <class Datum, const int n>
Datum& Expandable<Datum, n>::getData(int i) {

  if (i < 0 || endN < i) i = endN;   return (*this)[i];
  }


// Copy the node at the end of the array into d and remove node from the end of the array

template <class Datum, const int n>
bool   Expandable<Datum, n>::pop(Datum& d) {
int    i = endN - 1;    if (i < 0) return false;

  d = tbl[i];   tbl[i].~Datum();   endN--;   return true;
  }


// Linear search

template <class Datum, const int n>
template<class Key> Datum* Expandable<Datum, n>::find(Key key) {
int i;

  for (i = 0; i < endN; i++) if (tbl[i] == key) return &tbl[i];

  return 0;
  }


// Binary search (only works on sorted array

template <class Datum, const int n>
template<class Key> Datum* Expandable<Datum, n>::bSearch(Key key) {
int beg  = 0;
int end  = endN;
int last = -1;
int i;

  for (i = (beg+end)/2; i < endN && i != last; last = i, i = (beg+end)/2) {

    Datum& rcd = tbl[i];

    if (rcd <  key) {beg = i; continue;}
    if (rcd >  key) {end = i; continue;}
    if (rcd == key) return &rcd;
    throw ExpandableException;
    }

  return 0;
  }


// Delete datum at index x, move higher elements up one

template <class Datum, const int n>
bool Expandable<Datum, n>::del(int x) {
int i;

  if (endN <= 0 || x < 0 || endN <= x) return false;

  Datum& d = tbl[endN-1];

  for (i = x, --endN; i < endN; i++) tbl[i] = tbl[i+1];

  d.~Datum();   return true;
  }


// Copy data from e to this array

template <class Datum, const int n>
void Expandable<Datum, n>::append(Expandable& e) {
int lng = endN + e.endN;
int n   = e.endN;
int i;

  if (lng > tblN) expand(lng);

  for (i = 0; i < n; i++) tbl[endN++] = e.tbl[i];
  }


// Expand array when required

template <class Datum, const int n>
void Expandable<Datum, n>::expand(int i) {
Datum* p      = tbl;
Datum* q      = tbl;
int    nItems = tblN;
int    j;

  while (tblN <= i && tblN < INT_MAX/2) tblN = tblN ? tblN * 2 : 1;

  NewArray(Datum); tbl = AllocArray(tblN);

  for (j = 0; j < nItems; j++, p++) tbl[j] = *p;

  FreeArray(q);
  }


#define DebugAlloc
#ifdef DebugAlloc
#include "NotePad.h"
#endif

#ifdef DebugAlloc
template <class Datum, const int n>
void Expandable<Datum, n>::probe(TCchar* title) {
String s;
String t;
int    i;
int    nonEmpty = 0;

  for (i = endN; i < tblN; i++) if (!tbl[i].isEmpty()) nonEmpty++;
  s.format(_T(" -- endN: %i, tblN: %i"), endN, tblN);                 notePad << title << s;
  if (nonEmpty) {t.format(_T(", *** Non Empty: %i ***"), nonEmpty);   notePad << t;}
  notePad << nCrlf;
  }
#endif




/////--------------

#if 0
//#define DebugAlloc

#ifdef DebugAlloc
#include "MessageBox.h"
#endif
#ifdef DebugAlloc
int n = tblN * sizeof(Datum) + sizeof(int);
  if (n == 324) {
    messageBox(_T("Expandable 324"));
    }
#endif
#ifdef DebugAlloc
int n = tblN * sizeof(Datum) + sizeof(int);
  if (n == 260) {
    messageBox(_T("Expandable"));
    }
#endif
#endif
#if 0
template <class Datum, const int n>
Datum* Expandable<Datum, n>::operator+= (Datum* d)
              {if (!d) return 0;    Datum& datum = (*this)[endN];  datum = *d;  return &datum;}
#endif
#if 0
// Copy all elements of array e into a this array

template <class Datum, const int n>
void Expandable<Datum, n>::copy(Expandable& e) {

  if (e.endN > tblN) expand(e.endN);

  for (endN = 0; endN < e.endN; endN++) tbl[endN] = e.tbl[endN];
  }
#endif

