// My CSV Records


#pragma once
#include "Archive.h"
#include "CSVLex.h"


class CSVRcdXB {
protected:
int    putI;
int    getI;

public:

  CSVRcdXB() : putI(0), getI(0) { }
  virtual ~CSVRcdXB() { }

  virtual void clear() {putI = getI = 0;}

  virtual bool load(CSVLex& lex);

  virtual void display();

  virtual void store(Archive& ar);

  virtual CSVRcdXB& operator= (CSVRcdXB& r) {copy(r); return *this;}

  virtual bool operator>= (CSVRcdXB& r) {return false;}
  virtual bool operator== (CSVRcdXB& r) {return false;}

protected:

  virtual void    put(TCchar* p) { }

  virtual String* get() {return 0;}

  virtual void    copy(CSVRcdXB& r) {putI = r.putI; getI = r.getI;}
  };


