// ExpandableP:  Array of Pointers (only)
// Copyright Software Design & Engineering, Robert R. Van Tuyl, 2020.  All rights reserved.

/*
Record is a class, e.g. class Record {...}, and requires the following methods:
  -- Constructor with no argument, e.g. RcdPtr(), which initializes all components of the class
  -- Copy Constructor, e.g. Record(const Record& d), which copies all components of object d to
     object *this
  -- Assignment operator, e.g. Record& operator= (Record& d), which copies all components of object d to
     object *this and returns *this
  -- Destructor, e.g. ~Record() that releases objects obtained from the heap and zeros all data
     components
Furthermore, if the Record class contains a pointer to an object, then the copy operator must be
considered to be a move operator.  This means that the Record destructor must do no more than place
a zero in the pointer.  In the event that there are a mix of pointers and say Strings (which have
allocated content) then the Strings must be allowed to destruct normally and the pointers must just be
zeroed.

The RcdPtrT template defines a pointer to the Record.  A RcdPtr is the item that is stored in the array.
This leads to some efficiency since when moving items in the array, only a pointer is moved.  However, it
also introduces some complexity.  For the most part, ExpandableP manages allocating and deallocating the
storage used for the Record but there is room for the user to allocate and deallocate when this is
required.

The easiest way to loop through all the elements in the array is to use an iterator.  It IterT.h file
contains a template for creating an iterator for Expandable files.  First one must declare the Iterator
class so that an object may be created somewhere that it is going to be used.  The Datum class is a record
that is stored in an expandable array which is housed in class Xyz

  class Xyz;
  typedef IterT<Xyz, Datum> DataIter;                        // Iterator for the Xyz

Now that is not all there is to do.  The Xyz class must implement the following:

The template requires two functions be part of Store:

  int   nData()            -- returns number of data items in array
  Data* datum(int i)       -- returns either a pointer to data (or datum) at index i in array or zero
  void  removeDatum(int i) -- if i in bounds, removes and deallocates record
  friend typename DataIter -- required to give access to private area of Xyz.

private:

  // returns either a pointer to data (or datum) at index i in array or zero

  Data* datum(int i) {return 0 <= i && i < nData() ? data[i].p : 0;}

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

The operations supported by an ExpandableP array where the declaration is:

  typedef RcdPtrT<Datum> DatumP;                      // Usually defined just before iterator

  class Xyz {
  ExpandableP<Datum, DatumP, 2> data;
  Datum                         datum;
      ,,,

the operations supported are:

  datum = data[i];                  // where 0 <= i < endN
  data[i] = datum;                  // array expands to encompass i
  data.clear();                     // content is ignored but number of elements is set to zero
  data = datum;                     // datum is inserted into the sorted array at the correct position
  data = &datum;                    // ">=" and "==" operators in datum must be defined
  data += &datum;                   // A pointer to a datum is considered to be an already allocated
                                    // (see allocate() below) Record and is appended to the array
  data += datum;                    // A record is assumed to be a local variable and not already
                                    // allocated in the heap.  So a record is allocated and datum is
                                    // copied to the new record.  The new record is appended to array
                                    // (at new last element)
  Datum& d = data.nextData();       // A new record is allocated and stored in the new last element of
                                    // array.  A reference to the new record is returned.  It may used
                                    // as shown or immedialy with a dot operator or even as a target of
  data.nextData() = _T("xxx");      // an assignment (where a Datum operator= is defined)
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
  Datum* d = data.allocate();       // Allocate one record and return a pointer to it -- use in moderation
  data.deallocate(&datum);          // Deallocate one record
  RcdPtr* rcdP = data.getRcdPtr(i); // Returns a pointer to a RcdPtr class.  This should be used sparingly
                                    // if at all.  I used it once to deal with deallocating a case where
                                    // the pointer in RcdPtr contained a base pointer with two varients.
                                    // Thus, the standard deallocation scheme failed to release all the
                                    // memory since it only know about the base class.


The Record needs the following functions for the Insertion Sort and Binary Sort to work.

class Record {

  o o o

  // Required for Insertion Sort, i.e. data = dtm;
  bool operator >= (Record& r) {return key >= r.key;}
  bool operator == (Record& r) {return key == r.key;}

  // Required for Binary Search
  bool operator== (TCchar* key) {return this->key == key;}
  bool operator<  (TCchar* key) {return this->key <  key;}
  bool operator>  (TCchar* key) {return this->key >  key;}
  };
*/

#pragma once


#define ExpandableException _T("Corrupted Expandable(P) structure")


template<class Record>
struct RcdPtrT {
Record* p;

  RcdPtrT() : p(0) { }
 ~RcdPtrT() {p = 0;}
  RcdPtrT(RcdPtrT& x) {p = x.p;}

  RcdPtrT& operator=  (Record& r)  {p = &r;  return *this;}
  RcdPtrT& operator=  (Record* r)  {p =  r;  return *this;}
  RcdPtrT& operator=  (RcdPtrT& x) {p = x.p; return *this;}

  // Required for Insertion Sort, i.e. data = dtm;
  bool     operator== (RcdPtrT& x) {return *p == *x.p;}
  bool     operator>= (RcdPtrT& x) {return *p >= *x.p;}

  // Required for qsort along with operator== above
  bool     operator>  (RcdPtrT& x) {return *p >  *x.p;}
  bool     operator<= (RcdPtrT& x) {return *p <= *x.p;}

  // The rest of the conditionsals
  bool     operator!= (RcdPtrT& x) {return *p != *x.p;}
  bool     operator<  (RcdPtrT& x) {return *p <  *x.p;}

  // Required for Binary Search
  // Binary Search of TCchar* (LCD for Strings) and other pointers
  template<class Key>
  bool     operator== (Key* key) {return *p == key;}

  template<class Key>
  bool     operator<  (Key* key) {return *p <  key;}

  template<class Key>
  bool     operator>  (Key* key) {return *p >  key;}

  // Binary Search using integers as the key...
  bool     operator== (long key) {return *p == key;}
  bool     operator<  (long key) {return *p <  key;}
  bool     operator>  (long key) {return *p >  key;}
  };



template <class Record, class RcdPtr, const int n>
class ExpandableP {

int     endN;                         // Number of items in the array that may be occupied
int     tblN;                         // total number of elements in the table
RcdPtr* tbl;                          // Pointer to a heap object which is treated as an array of Datums

public:

  ExpandableP();                      // Constructor & Destructor
 ~ExpandableP();

  Record*  allocate()            {NewAlloc(Record); return AllocNode;}    // allocate a record on the heap
  void     deallocate(Record* p) {NewAlloc(Record); FreeNode(p);}         // Does not clear array entry.
  RcdPtr*  getRcdPtr(int i) {return 0 <= i && i < endN ? &tbl[i] : 0;}    // Used for difficult cases

  ExpandableP& operator= (ExpandableP& e);                                // copy the whole array

  RcdPtr& operator[] (int i);                                             // return the reference

  void clear() {freeAllNodes();}          // Clears the number of items in array (without deleting data)

  int  end()   {return endN;}             // Returns number of items in array if inserted sequentially

  // Insert RcdPtr d into array sorted (being sure to expand it if necessary.
  // A record reference suggests that a heap node must be allocated
  // A record pointer means that the node has been allocated by the user and can be
  // inserted directly in the array.  Every record is inserted into the array

  Record* operator= (Record& d) {Record* p = allocate();  *p = d; return insertSorted(p);}

  Record* operator= (Record* p) {return insertSorted(p);}

  // Append record at end of array, first allocating a record and then copies data from r into node
  // Returns a pointer to the record in the vector

  Record* operator+= (Record& r);

  // Append an allocated record on end of array, places pointer at end of array
  // Returns a pointer to the record in the vector

  Record* operator+= (Record* r);

  Record& nextData();     // Allocate a node and put it at end of array and return reference to node

  Record& getData(int i); // Get a node at index i or the end of the vector, allocating it if necessary

  // Insert data at index, moving other entries out of the way and allocating a new record at x

  bool operator() (int x, Record& d);

  // Insert an allocated record at index, moving other entries out of the way

  bool operator() (int x, Record* d);

  bool del(Record* p);  // Find Record with ptr, p, in the array and delete it, see del(int x)

  bool del(int x);      // Delete node at x, free node, adjust array to fill gap, clear last entry

  template<class Key> Record* bSearch(Key key);   // Binary search -- only works on sorted array

  template<class Key> Record* find(Key key);      // Linear Search

private:

  Record* insertSorted(Record* p);              // Insert every record into array using an insertion sort

  void freeAllNodes();                            // Free all Nodes

  void copy(ExpandableP& e);                      // Copy array e to this array

  void expand(int i);                             // Expand array
  };


// Constructor

template <class Record, class RcdPtr, const int n>
ExpandableP<Record, RcdPtr, n>::ExpandableP() : endN(0), tblN(n > 0 ? n : 1)
                     {NewArray(RcdPtr); tbl = AllocArray(tblN);  ZeroMemory(tbl, tblN * sizeof(RcdPtr));}


// We have placed ptrs to nodes in the array.  But now we need to free the nodes and clear the pointers

template <class Record, class RcdPtr, const int n>
ExpandableP<Record, RcdPtr, n>::~ExpandableP()
                         {freeAllNodes();  NewArray(RcdPtr); FreeArray(tbl);   tbl = 0; tblN = 0;}


// copy the whole array

template <class Record, class RcdPtr, const int n>
ExpandableP<Record, RcdPtr, n>& ExpandableP<Record, RcdPtr, n>::operator= (ExpandableP& e)
                                                                        {clear(); copy(e); return *this;}


// return the reference

template <class Record, class RcdPtr, const int n>
RcdPtr& ExpandableP<Record, RcdPtr, n>::operator[] (int i) {
  if (i >= tblN) expand(i);
  if (i >= endN) endN = i+1;
  return tbl[i];
  }


// Append already allocated record on end of array, places pointer at end of array
// Returns a pointer to the record in the vector

template <class Record, class RcdPtr, const int n>
Record* ExpandableP<Record, RcdPtr, n>::operator+= (Record* r)
                           {if (!r) return 0;   RcdPtr& rcdP = (*this)[endN]; rcdP.p = r; return rcdP.p;}


// Append record at end of array, first allocating a record and then copies data from r into node
// Returns a pointer to the record in the vector

template <class Record, class RcdPtr, const int n>
Record* ExpandableP<Record, RcdPtr, n>::operator+= (Record& r) {
  RcdPtr& rcdP = (*this)[endN];
  Record* node = allocate();    *node = r;   rcdP.p = node;   return node;
  }


// Allocate a node and put it at end of array and return reference to node

template <class Record, class RcdPtr, const int n>
Record& ExpandableP<Record, RcdPtr, n>::nextData()
                  {RcdPtr& rcdP = (*this)[endN]; Record* node = allocate(); rcdP.p = node; return *node;}


// Get a node at index i or the end of the vector, allocating it if necessary

template <class Record, class RcdPtr, const int n>
Record& ExpandableP<Record, RcdPtr, n>::getData(int i) {
  if (i < 0 || endN < i) i = endN;
  RcdPtr& rcdP = (*this)[i];   if (rcdP.p)  return *rcdP.p;
  Record* node = allocate(); rcdP.p = node; return *node;
  }


// Insert data at index, moving other entries out of the way and allocating a new record at x

template <class Record, class RcdPtr, const int n>
bool ExpandableP<Record, RcdPtr, n>::operator() (int x, Record& d)
                                       {Record* node = allocate();  *node = d; return (*this)(x, node);}


// Insert an an allocated record at index, moving other entries out of the way

template <class Record, class RcdPtr, const int n>
bool ExpandableP<Record, RcdPtr, n>::operator() (int x, Record* d) {
int i;

  if (x < 0 || endN < x) return false;

  if (++endN >= tblN) expand(x);

  for (i = endN-2; i >= x; i--) tbl[i+1] = tbl[i];

  tbl[x].p = d;   return true;
  }


// Find Record in array given a pointer to the record and delete it, see del(int x)

template <class Record, class RcdPtr, const int n>
bool ExpandableP<Record, RcdPtr, n>::del(Record* p) {
int i;

  if (!p || !tbl) return false;

  for (i = 0; i < endN; i++) if (tbl[i].p == p) return del(i);

  return false;
  }


// Delete node at x, free node, adjust array to fill gap, clear last entry

template <class Record, class RcdPtr, const int n>
bool ExpandableP<Record, RcdPtr, n>::del(int x) {
int i;

  if (endN <= 0 || x < 0 || endN <= x) return false;

  Record* p = tbl[x].p;

  for (i = x, --endN; i < endN; i++) tbl[i] = tbl[i+1];

  NewAlloc(Record);  FreeNode(p);   tbl[endN] = 0;   return true;
  }


// Binary search -- only works on sorted array

template <class Record, class RcdPtr, const int n>
template<class Key> Record* ExpandableP<Record, RcdPtr, n>::bSearch(Key key) {
int     beg  = 0;
int     end  = endN;
int     last = -1;
int     i;

  for (i = (beg+end)/2; i < endN && i != last; last = i, i = (beg+end)/2) {

    RcdPtr& rcd = tbl[i];   if (!rcd.p) throw ExpandableException;

    if (rcd <  key) {beg = i; continue;}
    if (rcd >  key) {end = i; continue;}
    if (rcd == key) return rcd.p;
    throw ExpandableException;
    }

  return 0;
  }


// Linear Search

template <class Record, class RcdPtr, const int n>
template<class Key> Record* ExpandableP<Record, RcdPtr, n>::find(Key key) {
int i;

  for (i = 0; i < endN; i++) {

    RcdPtr& rcd = tbl[i];    if (!rcd.p) throw ExpandableException;

    if (rcd == key) return rcd.p;
    }

  return 0;
  }


// Insert every record into array using an insertion sort

template <class Record, class RcdPtr, const int n>
Record* ExpandableP<Record, RcdPtr, n>::insertSorted(Record* p) {
RcdPtr xNode;
RcdPtr nextNode;
int    i;

  if (!p) return 0;
  Record& rcd = *p;

  for (i = endN-1; i >= 0; i--) {Record& r = *tbl[i].p;   if (&r && rcd >= r) {i++; break;}}

  if (i < 0) i = 0;

  for (xNode = p, endN++; i < endN; i++)
                     {if (i >= tblN) expand(i);   nextNode = tbl[i]; tbl[i] = xNode; xNode = nextNode;}
  return p;
  }


// Expand array

template <class Record, class RcdPtr, const int n>
void ExpandableP<Record, RcdPtr, n>::expand(int i) {
RcdPtr* p      = tbl;
RcdPtr* q      = tbl;
int     nItems = tblN;
int     j;

  while (tblN <= i && tblN < INT_MAX/2) tblN = tblN ? tblN * 2 : 1;

  NewArray(RcdPtr); tbl = AllocArray(tblN);

  for (j = 0; j < nItems; j++, p++) {tbl[j] = *p; p->p = 0;}
  for (     ; j < tblN;   j++) tbl[j].p = 0;

  FreeArray(q);
  }


// Copy array e to this array

template <class Record, class RcdPtr, const int n>
void ExpandableP<Record, RcdPtr, n>::copy(ExpandableP& e) {

  if (e.endN > tblN) expand(e.endN);

  for (endN = 0; endN < e.endN; endN++) {tbl[endN] = e.tbl[endN];  e.tbl[endN] = 0;}
  }


// Free all Nodes

template <class Record, class RcdPtr, const int n>
void ExpandableP<Record, RcdPtr, n>::freeAllNodes() {

  if (!tbl) return;

  for (int i = 0; i < endN; i++)
    {NewAlloc(Record);   FreeNode(tbl[i].p);   tbl[i].p = 0;}

  endN = 0;
  }

