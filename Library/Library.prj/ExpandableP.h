// ExpandableP:  Array of Pointers Extended(only)
// Copyright Software Design & Engineering, Robert R. Van Tuyl, 2023.  All rights reserved.

/*
Datum is a class, e.g. class Datum {...}, and requires the following methods:
  -- Constructor with no argument, e.g. Datum(), which initializes all components of the class
  -- Copy Constructor, e.g. Datum(const Datum& d), which copies all components of object d to
     object *this
  -- Assignment operator, e.g. Datum& operator= (Datum& d), which copies all components of object d to
     object *this and returns *this
  -- Destructor, e.g. ~Datum() that releases objects obtained from the heap and zeros all data
     components

Furthermore, if the Datum class contains a pointer to an object, then the copy operator must be
considered to be a move operator.  This means that the Datum destructor must do no more than place
a zero in the pointer.  In the event that there are a mix of pointers and say Strings (which have
allocated content) then the Strings must be allowed to destruct normally and the pointers must just be
zeroed.

The DatumPtrT template defines a pointer to the Datum.  A DatumPtr is the item that is stored in the
array.  This leads to some efficiency since when moving items in the array, only a pointer is moved.
However, it also introduces some complexity.  For the most part, ExpandableP manages allocating and
deallocating the storage used for the Datum but there is room for the user to allocate and deallocate
when this is required.  This is the declaration needed to define the pointer object:

  -- typedef DatumPtrT<Datum, Key> DatumP;

This extended version of ExpandableP includes a key class in the definitions of the DatumPtrT object and
the ExpandableP object.  This then requires a class with a minimal set of boolean operations to perform
searches, sorts and binary searches.

The following methods are required in Datum if the corresponding Expandable data method is needed:
 -- bool operator>= (Datum& d);     // Required for data = datum (Insertion Sort)
 -- bool operator<= (Datum& d);     // Required for qsort
 -- bool operator>  (Datum& d);     // Required for qsort

 -- bool operator== (Key  key);     // Required for linear(Key key) and bSearch(Key key)
 -- bool operator>  (Key  key);     // Required for bSearch(Key key)
 -- bool operator<  (Key  key);     // Required for bSearch(Key key)

Interator

The easiest way to loop through all the elements in the array is to use an iterator.  It IterT.h file
contains a template for creating an iterator for Expandable files.  First one must declare the Iterator
class so that an object may be created somewhere that it is going to be used.  The Datum class is a record
that is stored in an expandable array which is housed in class Xyz

  class Xyz;
  typedef IterT<Xyz, Datum> XyzIter;                        // Iterator for the Xyz

Now that is not all there is to do.  The Xyz class must implement the following:

  int   nData()            -- returns number of data items in array
  Data* datum(int i)       -- returns either a pointer to data (or datum) at index i in array or zero
  void  removeDatum(int i) -- if i in bounds, removes and deallocates record
  friend typename DataIter -- required to give access to private area of Xyz.


class Xyz {
  ooo
private:

  // returns either a pointer to data (or datum) at index i in array or zero

  Datum* datum(int i) {return 0 <= i && i < nData() ? data[i].p : 0;}

  int    nData()      {return data.end();}                     // returns number of data items in array
                                                               // not necessarily private
  void   removeDatum(int i) {if (0 <= i && i < nData()) data.del(i);}

  friend typename XyzIter;
  };

Once the iterator class is defined then the following is how it would be used:

   XyzIter iter(xyz);                          // Where: Xyz xyz;  (i.e. xyz is an object of class Xyz
   Datum*  datum;                               // May be iter(*this) in an Xyz method

     for (datum = iter(); datum; datum = iter++) {
       <Use datum as a pointer to the record, it is guaranteed to be non-zero>
       }

The operations supported by an ExpandableP array where the declaration is:

  typedef DatumPtrT<Datum, KeyClass> DatumPtr;                // Usually defined just before iterator

  class Xyz {
  ExpandableP<Datum, KeyClass, DatumPtr, 2> data;
  Datum                                     datum;
        ,,,

the operations supported are:

  data1    = data;                  // Copies all the current elements in data to data1, data is unchanged
  datum    = data[i];               // where 0 <= i < endN
  data[i]  = datum;                 // array expands to encompass i
  data.clear();                     // content is ignored but number of elements is set to zero
  data     = datum;                 // datum is copied into the sorted array at the correct position
                                    // Requires datum >= datum1
  data     = &datum;                // A pointer is considered to be an allocated Datum object
  data     += &datum;               // A pointer to a datum is considered to be an already allocated
                                    // (see allocate() below) Datum and is appended to the array
  data     += datum;                // A record is assumed to be a local variable and not already
                                    // allocated in the heap.  So a record is allocated and datum is
                                    // copied to the new record.  The new record is appended to array
                                    // (at new last element)
  Datum& d = data.nextData();       // A new record is allocated and stored in the new last element of
                                    // array.  A reference to the new record is returned.  It may used
                                    // as shown or immedialy with a dot operator or even as a target of
  data.nextData() = datum;          // an assignment (where a Datum operator= is defined)
  Datum& d = data.getData(i);       // return a reference to a record at index i allocating a record if
                                    // necessary
  data(i, datum);                   // datum is inserted at index i, the contents at i and above are moved
                                    // up one element.  A new record is allocated and datum copied into
                                    // the new record.
  data(i, &datum);                  // The pointer indicates that the record has already been allocated
                                    // and it is inserted at index i, the contents at i and above being
                                    // moved up one lement.
  data.del(i);                      // The datum at index i is deleted and the elements above are moved
                                    // down to fill in the hole.  The number of elements in the array is
                                    // reduced by one
  data.find(key)                    // Performs a linear search for key.  Requires datum == Key object,
                                    // where Key is a type found in Datum object
  data.bsearch(key)                 // Performs a binary search for key.  Requires a sorted array and the
                                    // boolean operations datum > key, datum < key, datum == key
  Datum* d = data.allocate();       // Allocate one record and return a pointer to it -- use in moderation
  data.deallocate(&datum);          // Deallocate one record

  DatumPtr* datumX = data.getDatumPtr(i);
                                    // Returns a pointer to a DatumPtr class.  This should be used
                                    // sparingly if at all.  I used it once to deal with deallocating a
                                    // case where the pointer in DatumPtr contained a base pointer with
                                    // two varients.  Thus, the standard deallocation scheme failed to
                                    // release all the memory since it only know about the base class.
*/


#pragma once
#include "NewAllocator.h"


#define ExpandableException _T("Corrupted Expandable(P) structure")


template<class Datum, class Key>
struct DatumPtrT {
Datum* p;

  DatumPtrT() : p(0) { }
 ~DatumPtrT() {p = 0;}
  DatumPtrT(DatumPtrT& x) {p = x.p;}

  DatumPtrT& operator=  (Datum& r)     {p = &r;  return *this;}
  DatumPtrT& operator=  (Datum* r)     {p =  r;  return *this;}
  DatumPtrT& operator=  (DatumPtrT& x) {p = x.p; return *this;}

  // Required for Insertion Sort, i.e. data = dtm;
  bool     operator>= (DatumPtrT& x) {return *p >= *x.p;}

  // Required for qsort
  bool     operator>  (DatumPtrT& x) {return *p >  *x.p;}
  bool     operator<= (DatumPtrT& x) {return *p <= *x.p;}

  // The rest of the conditionsals
  bool     operator== (DatumPtrT& x) {return *p == *x.p;}
  bool     operator!= (DatumPtrT& x) {return *p != *x.p;}
  bool     operator<  (DatumPtrT& x) {return *p <  *x.p;}

  // Required for Binary Search: where Key& key, datum == key, datum < key, datum > key,
  // Binary Search of Key& (Reference to the key for Datum)

  bool     operator== (Key& key) {return *p == key;}        // Also required for Linear Search
  bool     operator<  (Key& key) {return *p <  key;}
  bool     operator>  (Key& key) {return *p >  key;}

  // The rest of the conditionals
  bool     operator!= (Key& key) {return *p != key;}
  bool     operator<= (Key& key) {return *p <= key;}
  bool     operator>= (Key& key) {return *p >= key;}
  };



template <class Datum, class Key, class DatumPtr, const int n>
class ExpandableP {

int       endN;                         // Number of items in the array that may be occupied
int       tblN;                         // total number of elements in the table
DatumPtr* tbl;                          // Pointer to a heap object which is treated as an array of Datums

public:

  ExpandableP();                      // Constructor & Destructor
 ~ExpandableP();

 ExpandableP& operator= (ExpandableP& e);                                   // copy the whole array

  Datum*    allocate()           {NewAlloc(Datum); return AllocNode;}       // allocate a heap record
  void      deallocate(Datum* p) {NewAlloc(Datum); FreeNode(p);}            // Does not clear array entry.
  DatumPtr* getDatumPtr(int i) {return 0 <= i && i < endN ? &tbl[i] : 0;}   // Used for difficult cases

  DatumPtr& operator[] (int i);                                             // return the reference

  void      clear() {freeAllNodes();}       // Clears the number of items in array (without deleting data)

  int       end()   {return endN;}          // Returns number of items in array if inserted sequentially

  // Insert DatumPtr d into array sorted (being sure to expand it if necessary.
  // A record reference suggests that a heap node must be allocated
  // A record pointer means that the node has been allocated by the user and can be
  // inserted directly in the array.  Every record is inserted into the array

  Datum* operator= (Datum& d) {Datum* p = allocate();  *p = d; return insertSorted(p);}

  Datum* operator= (Datum* p) {return insertSorted(p);}

  // Append record at end of array, first allocating a record and then copies data from r into node
  // Returns a pointer to the record in the vector

  Datum* operator+= (Datum& r);

  // Append an allocated record on end of array, places pointer at end of array
  // Returns a pointer to the record in the vector

  Datum* operator+= (Datum* r);

  Datum& nextData();     // Allocate a node and put it at end of array and return reference to node

  Datum& getData(int i); // Get a node at index i or the end of the vector, allocating it if necessary

  // Insert data at index, moving other entries out of the way and allocating a new record at x

  bool   operator() (int x, Datum& d);

  // Insert an allocated record at index, moving other entries out of the way

  bool   operator() (int x, Datum* d);

  bool   del(Datum* p);   // Find Datum with ptr, p, in the array and delete it, see del(int x)

  bool   del(int x);      // Delete node at x, free node, adjust array to fill gap, clear last entry

  void   decrementEndN() {endN = endN > 0 ? endN-1 : 0;}   // Implement del outside of Expandable

  Datum* find(Key& key);      // Linear Search

  Datum* bSearch(Key& key);   // Binary search -- only works on sorted array

private:

  Datum* insertSorted(Datum* p);                // Insert every record into array using an insertion sort

  void   freeAllNodes();                        // Free all Nodes

  void   copy(ExpandableP& e);                  // Copy array e to this array

  void   expand(int i);                         // Expand array
  };


// Constructor

template <class Datum, class Key, class DatumPtr, const int n>
ExpandableP<Datum, Key, DatumPtr, n>::ExpandableP() : endN(0), tblN(n > 0 ? n : 1)
                  {NewArray(DatumPtr); tbl = AllocArray(tblN);  ZeroMemory(tbl, tblN * sizeof(DatumPtr));}


// We have placed ptrs to nodes in the array.  But now we need to free the nodes and clear the pointers

template <class Datum, class Key, class DatumPtr, const int n>
ExpandableP<Datum, Key, DatumPtr, n>::~ExpandableP()
                               {freeAllNodes();  NewArray(DatumPtr); FreeArray(tbl);   tbl = 0; tblN = 0;}


// copy the whole array

template <class Datum, class Key, class DatumPtr, const int n>
ExpandableP<Datum, Key, DatumPtr, n>& ExpandableP<Datum, Key, DatumPtr, n>::operator= (ExpandableP& e)
                                                                        {clear(); copy(e); return *this;}


// return the reference

template <class Datum, class Key, class DatumPtr, const int n>
DatumPtr& ExpandableP<Datum, Key, DatumPtr, n>::operator[] (int i) {
  if (i >= tblN) expand(i);
  if (i >= endN) endN = i+1;
  return tbl[i];
  }


// Append already allocated record on end of array, places pointer at end of array
// Returns a pointer to the record in the vector

template <class Datum, class Key, class DatumPtr, const int n>
Datum* ExpandableP<Datum, Key, DatumPtr, n>::operator+= (Datum* r)
                          {if (!r) return 0;   DatumPtr& rcdP = (*this)[endN]; rcdP.p = r; return rcdP.p;}


// Append record at end of array, first allocating a record and then copies data from r into node
// Returns a pointer to the record in the vector

template <class Datum, class Key, class DatumPtr, const int n>
Datum* ExpandableP<Datum, Key, DatumPtr, n>::operator+= (Datum& r) {
  DatumPtr& rcdP = (*this)[endN];
  Datum* node = allocate();    *node = r;   rcdP.p = node;   return node;
  }


// Allocate a node and put it at end of array and return reference to node

template <class Datum, class Key, class DatumPtr, const int n>
Datum& ExpandableP<Datum, Key, DatumPtr, n>::nextData()
                  {DatumPtr& rcdP = (*this)[endN]; Datum* node = allocate(); rcdP.p = node; return *node;}


// Get a node at index i or the end of the vector, allocating it if necessary

template <class Datum, class Key, class DatumPtr, const int n>
Datum& ExpandableP<Datum, Key, DatumPtr, n>::getData(int i) {
  if (i < 0 || endN < i) i = endN;
  DatumPtr& rcdP = (*this)[i];   if (rcdP.p)  return *rcdP.p;
  Datum* node = allocate(); rcdP.p = node; return *node;
  }


// Insert data at index, moving other entries out of the way and allocating a new record at x

template <class Datum, class Key, class DatumPtr, const int n>
bool ExpandableP<Datum, Key, DatumPtr, n>::operator() (int x, Datum& d)
                                       {Datum* node = allocate();  *node = d; return (*this)(x, node);}


// Insert an an allocated record at index, moving other entries out of the way

template <class Datum, class Key, class DatumPtr, const int n>
bool ExpandableP<Datum, Key, DatumPtr, n>::operator() (int x, Datum* d) {
int i;

  if (x < 0 || endN < x) return false;

  if (++endN >= tblN) expand(x);

  for (i = endN-2; i >= x; i--) tbl[i+1] = tbl[i];

  tbl[x].p = d;   return true;
  }


// Find Datum in array given a pointer to the record and delete it, see del(int x)

template <class Datum, class Key, class DatumPtr, const int n>
bool ExpandableP<Datum, Key, DatumPtr, n>::del(Datum* p) {
int i;

  if (!p || !tbl) return false;

  for (i = 0; i < endN; i++) if (tbl[i].p == p) return del(i);

  return false;
  }


// Delete node at x, free node, adjust array to fill gap, clear last entry

template <class Datum, class Key, class DatumPtr, const int n>
bool ExpandableP<Datum, Key, DatumPtr, n>::del(int x) {
int i;

  if (endN <= 0 || x < 0 || endN <= x) return false;

  Datum* p = tbl[x].p;

  for (i = x, --endN; i < endN; i++) tbl[i] = tbl[i+1];

  NewAlloc(Datum);  FreeNode(p);   tbl[endN] = 0;   return true;
  }


// Binary search -- only works on sorted array

template <class Datum, class Key, class DatumPtr, const int n>
Datum* ExpandableP<Datum, Key, DatumPtr, n>::bSearch(Key& key) {
int     beg  = 0;
int     end  = endN;
int     last = -1;
int     i;

  for (i = (beg+end)/2; i < endN && i != last; last = i, i = (beg+end)/2) {

    DatumPtr& rcd = tbl[i];   if (!rcd.p) throw ExpandableException;

    if (rcd <  key) {beg = i; continue;}
    if (rcd >  key) {end = i; continue;}
    if (rcd == key) return rcd.p;
    throw ExpandableException;
    }

  return 0;
  }


// Linear Search

template <class Datum, class Key, class DatumPtr, const int n>
Datum* ExpandableP<Datum, Key, DatumPtr, n>::find(Key& key) {
int i;

  for (i = 0; i < endN; i++) {

    DatumPtr& rcd = tbl[i];    if (!rcd.p) throw ExpandableException;

    if (rcd == key) return rcd.p;
    }

  return 0;
  }


// Insert every record into array using an insertion sort

template <class Datum, class Key, class DatumPtr, const int n>
Datum* ExpandableP<Datum, Key, DatumPtr, n>::insertSorted(Datum* p) {
DatumPtr xNode;
DatumPtr nextNode;
int      i;

  if (!p) return 0;
  Datum& rcd = *p;

  for (i = endN-1; i >= 0; i--) {Datum& r = *tbl[i].p;   if (&r && rcd >= r) {i++; break;}}

  if (i < 0) i = 0;

  for (xNode = p, endN++; i < endN; i++)
                     {if (i >= tblN) expand(i);   nextNode = tbl[i]; tbl[i] = xNode; xNode = nextNode;}
  return p;
  }


// Expand array

template <class Datum, class Key, class DatumPtr, const int n>
void ExpandableP<Datum, Key, DatumPtr, n>::expand(int i) {
DatumPtr* p      = tbl;
DatumPtr* q      = tbl;
int       nItems = tblN;
int       j;

  while (tblN <= i && tblN < INT_MAX/2) tblN = tblN ? tblN * 2 : 1;

  NewArray(DatumPtr); tbl = AllocArray(tblN);

  for (j = 0; j < nItems; j++, p++) {tbl[j] = *p; p->p = 0;}
  for (     ; j < tblN;   j++) tbl[j].p = 0;

  FreeArray(q);
  }


// Copy array e to this array

template <class Datum, class Key, class DatumPtr, const int n>
void ExpandableP<Datum, Key, DatumPtr, n>::copy(ExpandableP& e) {

  if (e.endN > tblN) expand(e.endN);

  for (endN = 0; endN < e.endN; endN++) {tbl[endN] = e.tbl[endN];  e.tbl[endN] = 0;}
  }


// Free all Nodes

template <class Datum, class Key, class DatumPtr, const int n>
void ExpandableP<Datum, Key, DatumPtr, n>::freeAllNodes() {

  if (!tbl) return;

  for (int i = 0; i < endN; i++)
    {NewAlloc(Datum);   FreeNode(tbl[i].p);   tbl[i].p = 0;}

  endN = 0;
  }

