// Crypt.cpp:

#include "pch.h"
#include "Crypt.h"

#ifdef CryptLogic
BOOL Crypt::encrypt(String& password, String& key, Byte* encryption, DWORD& noBytes) {
BOOL       bResult  = TRUE;
DWORD      dwLength;

  if (!initialized) return FALSE;

  // Get handle to user default provider.
  if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, 0)) return false;

  // Create hash object.

  if (!CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash)) {release(); return false;}

  dwLength = key.length();

  if (!CryptHashData(hHash, (BYTE*) key.c_str(), dwLength, 0)) {release();  return false;}

  // Create block cipher session key based on hash of the password.

  if (!CryptDeriveKey(hProv, CALG_RC4, hHash, CRYPT_EXPORTABLE, &hKey)) {release(); return false;}

  dwLength = password.length(); if (dwLength < noBytes) noBytes = dwLength;

  memcpy_s(encryption, noBytes, password.c_str(), dwLength);                 // Encrypt data

  if (!CryptEncrypt(hKey, 0, TRUE, 0, encryption, &noBytes, noBytes)) {release(); return false;}

  release(); return true;
  }
#endif

bool Crypt::enc(String& password, String& key, ByteBlock& encryption) {
DWORD n;
NewArray(Byte);

  if (!initialized) return FALSE;

  // Get handle to user default provider.
  if (!acquireContext()) return false;

  if (!createHash()) return false;

  if (!deriveKey(key)) return false;

  n = (DWORD) (password.length() + 1);

  if (!encryption.noBytes)
                {encryption.bytes = AllocArray(n); encryption.noBytes = n; encryption.allocated = true;}

  if (n > encryption.noBytes) return false;

  encryption.noBytes = n;

  memcpy_s(encryption.bytes, encryption.noBytes, password, n);                 // Encrypt data

  return CryptEncrypt(hKey, 0, TRUE, 0, encryption.bytes, &encryption.noBytes, n) != 0;
  }

