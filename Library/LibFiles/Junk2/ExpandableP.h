// ExpandableP:  Array of Pointers (only)
// Copyright Software Design & Engineering, Robert R. Van Tuyl, 2020.  All rights reserved.

// Data is a class, e.g. class Data {...}, and requires the following methods:
//   -- Constructor with no argument, e.g. Data(), which initializes all components of the class
//   -- Copy Constructor, e.g. Data(const Data& d), which copies all components of object d to
//      object *this
//   -- Assignment operator, e.g. Data& operator= (Data& d), which copies all components of object d to
//      object *this and returns *this
//   -- Destructor, e.g. ~Data() that releases objects obtained from the heap and zeros all data
//      components
// Furthermore, if the Data class contains a pointer to an object, then the copy operator must be
// considered to be a move operator.  This means that the Data destructor must do no more than place
// a zero in the pointer.  In the event that there are a mix of pointers and say Strings (which have
// allocated content) then the Strings must be allowed to destruct normally and the pointers must just be
// zeroed.


#pragma once


template <class Data, const int n>

class ExpandableP {
typedef Data* DataP;

int    endN;
int    tblN;
DataP* tbl;

public:

  ExpandableP() : endN(0), tblN(n > 0 ? n : 1)
                       {NewAlloc(DataP); tbl = AllocArray(tblN);  ZeroMemory(tbl, tblN * sizeof(DataP));}

// We have placed ptrs to nodes in the array.  But now we need to free the nodes and clear the pointers

 ~ExpandableP() {freeAllNodes();  NewAlloc(DataP); FreeArray(tbl);   tbl = 0; endN = tblN = 0;}


  Data*  allocate()        {NewAlloc(Data); return AllocNode;}
  void deallocate(Data* p) {NewAlloc(data); FreeNode(p);}            // Does not clear array entry.


  ExpandableP& operator= (ExpandableP& e) {clr(); copy(e); return *this;}


  DataP& operator[] (int i) {
    if (i >= tblN) expand(i);
    if (i >= endN) endN = i+1;
    return tbl[i];
    }

  void clear() {freeAllNodes(); endN = 0;}  // Clears the number of items in array (without deleting data)


  int  end()   {return endN;}               // Returns number of items in array if inserted sequentially


  // Insert Data d into array sorted (being sure to expand it if necessary.  Note, if one use [] to insert
  // data into array, sorting is up to the user...!

  ExpandableP& operator= (Data& d) {
  DataP xNode;
  DataP nextNode;
  int  i;

    for (i = 0; i < endN; i++) if (tbl[i] && *tbl[i] >= d) {if (*tbl[i] == d) return *this; break;}

    for (xNode = &d, endN++; i < endN; i++)
                       {if (i >= tblN) expand(i);   nextNode = tbl[i]; tbl[i] = xNode; xNode = nextNode;}
    return *this;
    }


  // Insert data at index, moving other entries out of the way

  void operator() (int x, Data& d) {
  int i;

    if (++endN >= tblN) expand(endN);

    for (i = endN-2; i >= x; i--) tbl[i+1] = tbl[i];

    tbl[x] = &d;
    }


  // Delete node at x, free node, adjust array to fill gap, clear last entry

  void del(int x) {
  int i;

    if (endN <= 0 || x < 0 || endN <= x) return;

    Data* p = tbl[x];

    for (i = x, --endN; i < endN; i++) tbl[i] = tbl[i+1];

    NewAlloc(Data);  FreeNode(p);   tbl[endN] = 0;
    }

private:

  void freeAllNodes() {
    if (!tbl) return;

    for (int i = 0; i < endN; i++) {NewAlloc(Data);   FreeNode(tbl[i]);  tbl[i] = 0;}
    }


  void copy(ExpandableP& e) {

    if (e.endN > tblN) expand(e.endN);

    for (endN = 0; endN < e.endN; endN++) {tbl[endN] = e.tbl[endN];  e.tbl[endN] = 0;}
    }


  void expand(int i) {
  DataP* p      = tbl;
  DataP* q      = tbl;
  int    nItems = tblN;
  int    j;

    while (tblN <= i && tblN < INT_MAX/2) tblN = tblN ? tblN * 2 : 1;

    NewAlloc(DataP); tbl = AllocArray(tblN);

    for (j = 0; j < nItems; j++, p++) {tbl[j] = *p; *p = 0;}
    for (     ; j < tblN;   j++) tbl[j] = 0;

    FreeArray(q);
    }
  };

