// Crypt.h: Schnittstelle für die Klasse Crypt.
//
//////////////////////////////////////////////////////////////////////


#pragma once
#include <WinCrypt.h>

struct ByteBlock {
ulong noBytes;
Byte* bytes;
bool  allocated;

  ByteBlock()                 : noBytes(0), bytes(0), allocated(false) {}
  ByteBlock(ulong n, Byte* d) : noBytes(n), bytes(d), allocated(false) {}
 ~ByteBlock() {if (allocated) {NewAlloc(Byte); FreeNode(bytes);} noBytes = 0; bytes = 0;}
  };


class Crypt {

BOOL       initialized;
HCRYPTPROV hProv;
HCRYPTHASH hHash;
HCRYPTKEY  hKey;

public:
           Crypt() : hProv(0), hHash(0), hKey(0) {initialized = initialize();}
  virtual ~Crypt() {release(); initialized = false;}

  bool enc(String& password, String& key, ByteBlock& encryption);
  bool dec(ByteBlock& encryption, String& key, ByteBlock& password);

private:

  BOOL initialize();
  void release();

  bool acquireContext();
  bool createHash();
  bool deriveKey(String& key);
  };

