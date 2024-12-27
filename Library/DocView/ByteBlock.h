// A block of bytes allocated from the heap on demand


#pragma once

class Archive;


struct ByteBlock {

ulong n;
Byte* p;

          ByteBlock() : n(0), p(0) { }
         ~ByteBlock() {clear();}

          void      clear();
          void      expunge();
          bool      isEmpty() {return !n;}

          bool      alloc(ulong nBytes);

  virtual ByteBlock& operator= (ByteBlock& b);

  virtual bool      load( Archive& ar);
  virtual bool      store(Archive& ar);

  virtual void      display(TCchar* title);

private:

  bool isMod10(ulong i) {return ((i+1) % 10) == 0;}
  };

