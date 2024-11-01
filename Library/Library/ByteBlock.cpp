


#include "pch.h"
#include "ByteBlock.h"
#include "Archive.h"
#include "NotePad.h"
#include "Random.h"


void ByteBlock::clear() {
NewArray(Byte);

  if (p) {FreeArray(p);    p = 0;   n = 0;}
  }


void ByteBlock::expunge() {
ulong  i;
Random rnd;

  if (!n || !p) return;

  for (i = 0; i < n; i++) p[i] = (Byte) (256 * rnd.next());

  clear();
  }



bool ByteBlock::alloc(ulong nBytes) {
NewArray(Byte);

  if (!nBytes || nBytes == n) return true;
  if (p) clear();
  p = AllocArray(nBytes);   if (p) n = nBytes;

  memset(p, 0, n);   return p != 0;
  }


ByteBlock& ByteBlock::operator= (ByteBlock& b) {

  if (n != b.n) clear();

  if (!p && !alloc(b.n)) return *this;

  memcpy_s(p, n, b.p, b.n); return *this;
  }


bool ByteBlock::load( Archive& ar) {
ulong count;
int   x;

  clear();   if (!ar.read(count) || !alloc(count)) return false;

  x = n;   return ar.readBlk(p, x);
  }


bool ByteBlock::store(Archive& ar)
                             {if (!n || !p || !ar.write(n)) return false;   return ar.write(p, n);}


void ByteBlock::display(TCchar* title) {
String s;
ulong  i;

  notePad << title << _T(" - Count = ") << n << nCrlf << nCrlf;

  for (i = 0; i < n; i++)
              {s.format(_T("0x%02x, "), p[i]);   notePad << s;   if (isMod10(i)) notePad << nCrlf;}

  if (!isMod10(i)) notePad << nCrlf;
  }


