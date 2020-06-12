// Manipulator for << processing


#pragma once;


template<class MyClass> struct ManipT {

typedef MyClass& (*Func)(MyClass&);

MyClass* n;
Func     func;

  ManipT()                            : n(0),       func(0)    {}
  ManipT(MyClass* myClass, Func func) : n(myClass), func(func) {}
  };


template<class MyClass> struct ManipValT {

typedef MyClass& (*Func)(MyClass&, int);

Func func;
int  v;

  ManipValT()                : func(0), v(0)   {}
  ManipValT(Func f, int val) : func(f), v(val) {}
  };

