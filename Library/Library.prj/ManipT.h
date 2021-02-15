// Manipulator for << processing


#pragma once;


template<class MyClass> struct ManipT {

typedef MyClass& (*Func)(MyClass&);

MyClass* n;
Func     func;

  ManipT()                            : n(0),       func(0)    {}

 ~ManipT() {func = 0;}

  void set(MyClass* myClass, Func func) {n = myClass; func = func;}
  };


template<class MyClass> struct ManipIntT {

typedef MyClass& (*Func)(MyClass&, int);

Func func;
int  v;

  ManipIntT()                : func(0), v(0)   {}

 ~ManipIntT() {func = 0; v = 0;}

  void set(Func f, int val) {func = f; v = val;}
  };


template<class MyClass> struct ManipStgT {

typedef MyClass& (*Func)(MyClass&, String&);

Func   func;
String v;

  ManipStgT()                  : func(0)       { }

 ~ManipStgT() {func = 0;}

  void set(Func f, TCchar* s) {func = f; v = s;}
  };


template<class MyClass> struct ManipDblT {

typedef MyClass& (*Func)(MyClass&, double);

Func   func;
double v;

  ManipDblT()                   : func(0), v(0.0)   {}

 ~ManipDblT() {func = 0; v = 0.0;}

  void set(Func f, double val) {func = f; v = val;}
  };

