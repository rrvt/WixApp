// Status Update CSV Data


#pragma once

#include "Csv.h"
#include "Expandable.h"
#include "DisplayFns.h"


class CSVfield {
public:
String name;

  CSVfield() {}
  CSVfield(CSVfield& fd) {copy(fd, *this);}

 ~CSVfield() {}

  void      clear()                 {name.clear();}
  bool      isEmpty()               {return name.length() == 0;}
  void      replaceBlanks(TCchar ch) {int lng = name.length();
                                     int i;
                                       for (i = 0; i < lng; i++) if (name[i] == ' ') name[i] = ch;
                                       }
  CSVfield& operator<< (String& s)  {name += s;     return *this;}

  CSVfield& operator=(CSVfield& fd) {copy(fd, *this); return *this;}

private:

  void copy(CSVfield& src, CSVfield& dst) {dst.name = src.name;}
  };


struct CSVrecord {
int                         i;
Expandable <CSVfield*, 128> fields;

  CSVrecord() : i(0) {}
 ~CSVrecord() {}

  void      startStore() {fields.clr();}

  int       nFields() {return fields.end();}

  CSVfield* nextDatum() {CSVfield* d; fields[fields.end()] = d = new CSVfield; return d;}
  int       notEmptyNo() {int i = fields.end(); return fields[i-1]->isEmpty() ? 0 : i;}

  void dspFields() {dspFields(fields.end());}

  void dspFields(int n) {
  int       i;
  CSVfield* d;

    for (d = startLoop(), i = 0; d && i < n; d = nextFld(), i++) displayStg(_T("%s,"), d->name.str());

    crlfDsp();
    }

  CSVfield* startLoop() {i = 0; return i < nFields() ? fields[i] : 0;}
  CSVfield* nextFld()   {i++;   return i < nFields() ? fields[i] : 0;}
  };
