// Crypt.cpp:

#include "stdafx.h"
#include "Crypt.h"

bool Crypt::dec(ByteBlock& encryption, String& key, ByteBlock& password) {

  if (!initialized) return FALSE;

  // Get handle to user default provider.
  if (!acquireContext()) return false;

  // Create hash object.
  if (!createHash()) return false;

  if (!deriveKey(key)) return false;

  // copy encrypted password to temporary TCHAR

  if (password.noBytes < encryption.noBytes) return false;

  password.noBytes = encryption.noBytes;

  memcpy_s(password.bytes, password.noBytes, encryption.bytes, encryption.noBytes);

  return CryptDecrypt(hKey, 0, TRUE, 0, password.bytes, &password.noBytes) != 0;
  }

