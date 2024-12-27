// A block of Byte data allocated from the heap.  Used in an interface (i.e. CNG) to
// Cryptography API : Next Generation (CNG)


#pragma once
#include "ByteBlock.h"


struct CNGblock : public ByteBlock {

            CNGblock() { }
           ~CNGblock() {clear();}

  CNGblock& operator= (TCchar*  tc);

  bool      setKey(TCchar* key);

            operator TCchar*() const {return (TCchar*) p;}
  };


