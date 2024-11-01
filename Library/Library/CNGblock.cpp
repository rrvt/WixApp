// A block of Byte data allocated from the heap.  Used in an interface (i.e. CNG) to
// Cryptography API : Next Generation (CNG)


#include "pch.h"
#include "CNGblock.h"
#include "Archive.h"
#include "NotePad.h"
#include "Random.h"


CNGblock& CNGblock::operator= (TCchar*  tc) {

  if (alloc((_tcsclen(tc) + 1) * sizeof(Tchar))) memcpy_s(p, n, tc, n);

  return *this;
  }


bool CNGblock::setKey(TCchar* key) {
ulong lng = _tcsclen(key);
ulong hash;
ulong extra;
ulong i;

  if (!key || !*key || !alloc(16)) return false;

  for (i = 0, hash = 0; i < lng; i++)
                        {extra = (hash & 0xff000000) >> 24;   hash = (hash << 3) + extra + key[i];}
  Random rnd(hash);

  for (i = 0; i < n; i++) p[i] = (Byte) (256 * rnd.next());

  return true;
  }



