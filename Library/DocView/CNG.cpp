// A little class to save and restore names and passwords (or anytext) using Cryptography API : Next Generation (CNG)


#include "pch.h"
#include "CNG.h"
#include "NotePad.h"
#include "Random.h"


CNG::~CNG() {
  if (hAES) BCryptCloseAlgorithmProvider(hAES, 0);    hAES = 0;
  if (hKey) BCryptDestroyKey(hKey);                   hKey = 0;

  plainBlk.expunge();   ivBlk.clear();   keyBlk.clear();   cipherBlk.clear();
  }


// Accepts a string and key, returns a zero or ptr to an encrypted block

CNGblock* CNG::operator() (TCchar* tc, TCchar* key) {
String s;
ulong  n;
ulong  data;

  if (!setProvider())    return 0;

  if (!generateKey(key)) return 0;

  if (!getInitalValue()) return 0;

  plainBlk = tc;

  // Get the output buffer size.

  if (BCryptEncrypt(hKey, plainBlk.p, plainBlk.n, 0, ivBlk.p, ivBlk.n, 0, 0, &n,
                                                                   BCRYPT_BLOCK_PADDING)) return 0;
  if (!cipherBlk.alloc(n)) return 0;

  if (BCryptEncrypt(hKey, plainBlk.p, plainBlk.n, 0, ivBlk.p, ivBlk.n,
                                  cipherBlk.p, cipherBlk.n, &data, BCRYPT_BLOCK_PADDING)) return 0;

  if (hAES) BCryptCloseAlgorithmProvider(hAES, 0);    hAES = 0;
  if (hKey) BCryptDestroyKey(hKey);                   hKey = 0;

  plainBlk.clear();   ivBlk.clear();   keyBlk.clear();

  return &cipherBlk;
  }


// Accepts an encrypted block and key, returns zero or ptr to a string

TCchar* CNG::operator() (CNGblock& blk, TCchar* key) {
ulong  n;

  if (!setProvider())    return 0;

  if (!generateKey(key)) return 0;

  if (!getInitalValue()) return 0;

    // Get the output buffer size.

  if (BCryptDecrypt(hKey, blk.p, blk.n, 0, ivBlk.p, ivBlk.n, 0, 0, &n, BCRYPT_BLOCK_PADDING))
                                                                                          return 0;
  if (!plainBlk.alloc(n)) return 0;

  if (BCryptDecrypt(hKey, blk.p, blk.n, 0, ivBlk.p, ivBlk.n, plainBlk.p, plainBlk.n, &n,
                                                                   BCRYPT_BLOCK_PADDING)) return 0;
  if (hAES) BCryptCloseAlgorithmProvider(hAES, 0);   hAES = 0;
  if (hKey) BCryptDestroyKey(hKey);                  hKey = 0;

  ivBlk.clear();   keyBlk.clear();   cipherBlk.clear();

  return (TCchar*) plainBlk.p;
  }


bool CNG::setProvider() {
  if (BCryptOpenAlgorithmProvider(&hAES, BCRYPT_AES_ALGORITHM, 0, 0)) return false;

  return !BCryptSetProperty(hAES, BCRYPT_CHAINING_MODE,
                                  (Byte*) BCRYPT_CHAIN_MODE_CBC, sizeof(BCRYPT_CHAIN_MODE_CBC), 0);
  }


bool CNG::getInitalValue() {
ulong n;
ulong data;
Byte* p;
ulong i;
Random rnd(54321);

  // Calculate the block length for the IV.
  if (BCryptGetProperty(hAES, BCRYPT_BLOCK_LENGTH, (Byte*) &n, sizeof(n), &data, 0)) return false;

  if (!ivBlk.alloc(n)) return false;

  for (p = ivBlk.p, i = 0; i < n; i++) *p++ = (Byte) (256 * rnd.next());

  return true;
  }


bool CNG::generateKey(TCchar* key) {
CNGblock blk;
ulong    n;
ulong    data;

  blk.setKey(key);

  // Get the size of the buffer to hold the KeyObject.
  if (BCryptGetProperty(hAES, BCRYPT_OBJECT_LENGTH, (Byte*) &n, sizeof(n), &data, 0)) return false;

  if (!keyBlk.alloc(n)) return false;

  return !BCryptGenerateSymmetricKey(hAES, &hKey, keyBlk.p, keyBlk.n, blk.p, blk.n, 0);
  }


