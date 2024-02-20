// Debug version of new -- hopefully to find memory leaks


// This package provides some help in locating memory leaks (along with the vs17 internal debug tracking
// of allocated nodes.  Three things must happen to effect the outcome:
// -- Debug must be in force
// -- all new must be replaced with two macros, one to create a specialized allocator of memory for
//    a class or type that simulates the "new" c++ operator.
// -- Everywhere "new" appears for the specific the class designator is removed and AllocNode replaces new
// An example:
//   struct Node {
//   String s;
//     Node() {s = _T("OK");}
//    ~Node() {}
//    };
//
//    ooo
//
//    NewAlloc(Node); Node* p = AllocNode;            // An allocation object is created and one node
//                                                    // is created and the constructor called...
//
// Arrays are a bit more trouble in that the number of nodes must be explicitly mentioned
//
//   NewArray(Node); Node* p = AllocArray(3);         // Three entries in an array of Nodes are created
//                                                    // and the Node's constructor called for each entry
// Freeing the nodes must be done with the FreeNode or FreeArray macros.  The destructor for each
// Node is called and then the block of memory is freed.


#pragma once



#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

#define NewAlloc(T)                 NewAllocatorT<T> MyAllocator
#define AllocNode                   MyAllocator(                _NORMAL_BLOCK, __FILE__, __LINE__)
#define AllocNodeI1(i1)             MyAllocator(i1,             _NORMAL_BLOCK, __FILE__, __LINE__)
#define AllocNodeI2(i1, i2)         MyAllocator(i1, i2,         _NORMAL_BLOCK, __FILE__, __LINE__)
#define AllocNodeI3(i1, i2, i3)     MyAllocator(i1, i2, i3,     _NORMAL_BLOCK, __FILE__, __LINE__)
#define AllocNodeI4(i1, i2, i3, i4) MyAllocator(i1, i2, i3, i4, _NORMAL_BLOCK, __FILE__, __LINE__)

#define NewArray(T)      NewArrayAllocatorT<T> MyArrayAlloc
#define AllocArray(n)    MyArrayAlloc(n,   _NORMAL_BLOCK, __FILE__, __LINE__)

#else

#define NewAlloc(T)                 NewAllocatorT<T> MyAllocator
#define AllocNode                   MyAllocator()
#define AllocNodeI1(i1)             MyAllocator(i1)
#define AllocNodeI2(i1, i2)         MyAllocator(i1, i2)
#define AllocNodeI3(i1, i2, i3)     MyAllocator(i1, i2, i3)
#define AllocNodeI4(i1, i2, i3, i4) MyAllocator(i1, i2, i3, i4)

#define NewArray(T)      NewArrayAllocatorT<T> MyArrayAlloc
#define AllocArray(n)    MyArrayAlloc(n)

#endif

#define FreeNode(p)     MyAllocator.freeNode(p)
#define FreeArray(p)    MyArrayAlloc.freeArray(p)


//_malloc_dbg(s, _NORMAL_BLOCK, __FILE__, __LINE__)   from crtdbg.h

template<typename T> class NewAllocatorT {
public:

  NewAllocatorT() { }

#if _DEBUG

  T* operator() (int blk, const char* file, int line) {
    T* p = (T*) _malloc_dbg(sizeof(T), blk, file, line);   if (p) {new(p) T();}

    return p;
    }

  template<class I1>
  T* operator() (I1& i1, int blk, const char* file, int line) {

    T* p = (T*) _malloc_dbg(sizeof(T), blk, file, line);

    if (p) {new(p) T(i1);}

    return p;
    }

  template<class I1, class I2>
  T* operator() (I1& i1, I2& i2, int blk, const char* file, int line) {

    T* p = (T*) _malloc_dbg(sizeof(T), blk, file, line);

    if (p) {new(p) T(i1, i2);}

    return p;
    }

  template<class I1, class I2, class I3>
  T* operator() (I1& i1, I2& i2, I3& i3, int blk, const char* file, int line) {

    T* p = (T*) _malloc_dbg(sizeof(T), blk, file, line);

    if (p) {new(p) T(i1, i2, i3);}

    return p;
    }

  template<class I1, class I2, class I3, class I4>
  T* operator() (I1& i1, I2& i2, I3& i3, I4& i4, int blk, const char* file, int line) {

    T* p = (T*) _malloc_dbg(sizeof(T), blk, file, line);

    if (p) {new(p) T(i1, i2, i3, i4);}

    return p;
    }

  void freeNode(T* p) {if (!p) return;   p->~T();   _free_dbg(p, _NORMAL_BLOCK);}

#else

  T* operator() () {T* p = (T*) malloc(sizeof(T));   if (p) {new(p) T();}   return p;}

  template<class I1>
  T* operator() (I1& i1)
                        {T* p = (T*) malloc(sizeof(T));   if (p) {new(p) T(i1);}   return p;}

  template<class I1, class I2>
  T* operator() (I1& i1, I2& i2)
                        {T* p = (T*) malloc(sizeof(T));   if (p) {new(p) T(i1, i2);}   return p;}

  template<class I1, class I2, class I3>
  T* operator() (I1& i1, I2& i2, I3& i3)
                        {T* p = (T*) malloc(sizeof(T));   if (p) {new(p) T(i1, i2, i3);}   return p;}

  template<class I1, class I2, class I3, class I4>
  T* operator() (I1& i1, I2& i2, I3& i3, I4& i4)
                        {T* p = (T*) malloc(sizeof(T));   if (p) {new(p) T(i1, i2, i3, i4);}   return p;}

  void freeNode(T* p) {if (!p) return;   p->~T();   free(p);}

#endif
  };



template<typename T> class NewArrayAllocatorT{
public:

  NewArrayAllocatorT() { }

#if _DEBUG

  T* operator() (int n, int blk, const char* file, int line) {
    int* p   = (int*) _malloc_dbg(sizeof(T)*n + sizeof(int), blk, file, line);   if (!p) return 0;
        *p++ = n;
    T*   q   = (T*) p;
    T*   r;
    int  iii;

    for (iii = 0, r = q; iii < n; iii++, r++) new(r) T();

    return q;
    }

  void freeArray(T* p) {
     if (!p) return;

    int* pn = (int*) p;
    int  n  = *--pn;
    T*   q;
    int  iii;

    for (iii = 0, q = p; iii < n; iii++, q++) q->~T();

    _free_dbg(pn, _NORMAL_BLOCK);
    }

#else

  T* operator() (int n) {
    int* p   = (int*) malloc(sizeof(T)*n + sizeof(int));   if (!p) return 0;
        *p++ = n;
    T*   q   = (T*) p;
    T*   r;
    int  i;

    for (i = 0, r = q; i < n; i++, r++) new(r) T();    return q;
    }

  void freeArray(T* p) {
     if (!p) return;

    int* pn = (int*) p;
    int  n  = *--pn;
    T*   q;
    int  i;

    for (i = 0, q = p; i < n; i++, q++) q->~T();   free(pn);
    }

#endif
  };

