// Expandable Array
// Copyright Software Design & Engineering, Robert R. Van Tuyl, 2013.  All rights reserved.

/*
Datum is a class, e.g. class Datum {...}, and requires the following methods:
 -- Constructor with no argument, e.g. Datum(), which initializes all components of the class
 -- Copy Constructor, e.g. Datum(const Datum& d), which copies all components of object d to
    object *this
 -- Assignment operator, e.g. Datum& operator= (Datum& d), which copies all components of object d to
    object *this and returns *this
 -- Destructor, e.g. ~Datum() that releases objects obtained from the heap and zeros all data
    components

The easiest way to loop through all the elements in the array is to use an iterator.  It IterT.h file
contains a template for creating an iterator for Expandable files.  First one must declare the Iterator
class so that an object may be created somewhere that it is going to be used.  The Datum class is a record
that is stored in an expandable array which is housed in class Xyz

  class Xyz;
  typedef IterT<Xyz, Datum> DataIter;                        // Iterator for the Xyz

Now that is not all there is to do.  The Xyz class must implement the following:

The template requires two functions be part of Store:

  int    nData()            -- returns number of data items in array
  Datum* datum(int i)       -- returns either a pointer to data (or datum) at index i in array or zero
  void   removeDatum(int i) -- if i in bounds, removes and deallocates record
  friend typename DataIter -- required to give access to private area of Xyz.

private:

  // returns either a pointer to data (or datum) at index i in array or zero

  Datum* datum(int i) {return 0 <= i && i < nData() ? &data[i] : 0;}

  int   nData()      {return data.end();}                       // returns number of data items in array

  void  removeDatum(int i) {if (0 <= i && i < nData()) data.del(i);}

  friend typename DataIter;
  };

Once the iterator class is defined then the following is how it would be used:

   DataIter iter(xyz);                          // Where: Xyz xyz;  (i.e. xyz is an object of class Xyz
   Datum*   data;                               //

     for (data = iter(); data; data = iter++) {
       <Use data as a pointer to the record, it is guaranteed to be non-zero>
       }


The operations supported by an Expandable array where the declaration is:

  Expandable<Datum, 2> data;
  Datum                datum;

the operations supported are:

  datum = data[i];                  // where 0 <= i < endN
  data[i] = datum;                  // array expands to encompass i
  data.clear();                     // content is ignored but number of elements is set to zero
  data = datum;                     // datum is inserted into the sorted array at the correct position
                                    // ">=" and "==" operators in datum must be defined
  data += datum;                    // datum is appended to array (at new last element)
  Datum& d = data.nextData();       // A reference to new last element of array is returned.  It may used
                                    // as shown or immedialy with a dot operator or even as a target of
  data.nextData() = _T("xxx");      // an assignment (where a Datum operator= is defined)
  data(i, datum);                   // datum is inserted at index i, the contents at i and above are moved
                                    // up one element
  data.del(i);                      // The datum at index i is deleted and the elements above are moved
                                    // down to fill in the hole.  The number of elements in the array is
                                    // reduced by one
*/


#pragma once


#define ExpandableException _T("Corrupted Expandable(P) structure")


template <class Datum, const int n>

class Expandable {
int    endN;
int    tblN;
Datum* tbl;

public:

  Expandable() : endN(0), tblN(n > 0 ? n : 1) {NewArray(Datum); tbl = AllocArray(tblN);}

 ~Expandable() {
    if (tbl) {NewArray(Datum);  FreeArray(tbl);}
    tbl = 0; endN = tblN = 0;
    }

  Expandable& operator= (Expandable& e) {clear(); copy(e); return *this;}

  Datum& operator[] (int i) {
    if (i >= tblN) expand(i);
    if (i >= endN) endN = i+1;
    return tbl[i];
    }

  void clear() {endN = 0;}            // Clears the number of items in array (without deleting data)
  int  end()   {return endN;}         // Returns number of items in array if inserted sequentially


  // Insert Datum d into array sorted (being sure to expand it if necessary.  Note, if one use [] to
  // insert data into array, sorting is up to the user...!

  Datum* operator= (Datum& d) {
  Datum  xNode;
  Datum  nextNode;
  int    i;
  Datum* dtm;

    for (i = endN-1; i >= 0; i--)
                           {dtm = &tbl[i];   if (d >= *dtm) {if (*dtm == d) return dtm;   i++;   break;}}

    if (i < 0) i = 0;   dtm = &tbl[i];

    for (xNode = d, endN++; i < endN; i++)
                   {if (i >= tblN) expand(i);   nextNode = tbl[i];   tbl[i] = xNode;   xNode = nextNode;}
    return dtm;
    }


  // Append Datum to end of array, copies datum into array entry

  Datum* operator+= (Datum& d) {Datum& datum = (*this)[endN];  datum = d;  return &datum;}
  Datum* operator+= (Datum* d)
          {if (!d) return *this;    Datum& datum = (*this)[endN];  datum = *d;  return &datum;}

  // Return reference to next available node in array (at end)

  Datum& nextData() {return (*this)[endN];}

  // Insert data at index, moving other entries out of the way

  void operator() (int x, Datum& d) {
  int i;

    if (++endN >= tblN) expand(endN);

    for (i = endN-2; i >= x; i--) tbl[i+1] = tbl[i];

    tbl[x] = d;
    }

  void del(int x) {
  int i;

    if (endN <= 0 || x < 0 || endN <= x) return;

    Datum& d = tbl[endN-1];

    for (i = x, --endN; i < endN; i++) tbl[i] = tbl[i+1];

    d.~Datum();
    }

  template<class Key>
  Datum* bSearch(Key key) {
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


  template<class Key>
  Datum* find(Key key) {
  int i;

    for (i = 0; i < endN; i++) if (tbl[i] == key) return &tbl[i];

    return 0;
    }

private:

  void copy(Expandable& e) {

    if (e.endN > tblN) expand(e.endN);

    for (endN = 0; endN < e.endN; endN++) tbl[endN] = e.tbl[endN];
    }


  void expand(int i) {
  Datum* p      = tbl;
  Datum* q      = tbl;
  int   nItems = tblN;
  int   j;

    while (tblN <= i && tblN < INT_MAX/2) tblN = tblN ? tblN * 2 : 1;

    NewArray(Datum); tbl = AllocArray(tblN);

    for (j = 0; j < nItems; j++, p++) tbl[j] = *p;

    FreeArray(q);
    }
  };

