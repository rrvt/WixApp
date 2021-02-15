// Crypt.cpp:

#include "stdafx.h"
#include "Crypt.h"




// Ensure that the default cryptographic client is set up.

BOOL Crypt::initialize() {

  // Attempt to acquire a handle to the default key container or create default key container.

  if (!CryptAcquireContext(&hProv, NULL, MS_DEF_PROV, PROV_RSA_FULL, 0) &&
      !CryptAcquireContext(&hProv, NULL, MS_DEF_PROV, PROV_RSA_FULL, CRYPT_NEWKEYSET)) return FALSE;

  // Attempt to get handle to signature key.

  if (!CryptGetUserKey(hProv, AT_SIGNATURE, &hKey)) {
                                       // Error during CryptGetUserKey!
    if (GetLastError() != NTE_NO_KEY) {CryptReleaseContext(hProv, 0); return FALSE;}

    // Create signature key pair.

    if (!CryptGenKey(hProv, AT_SIGNATURE, 0, &hKey)) {release(); return FALSE;}

    release();
    }

  // Attempt to get handle to exchange key.

  if (!CryptGetUserKey(hProv, AT_KEYEXCHANGE, &hKey)) {
                                      // Error during CryptGetUserKey!
    if (GetLastError()!= NTE_NO_KEY) {release(); return FALSE;}

    // Create key exchange key pair.                    // Error during CryptGenKey!
    if (!CryptGenKey(hProv, AT_KEYEXCHANGE, 0, &hKey)) {release(); return FALSE;}
    }

  release(); return TRUE;
  }


void Crypt::release() {
  if (hKey)  {CryptDestroyKey(hKey);         hKey  = 0;}
  if (hHash) {CryptDestroyHash(hHash);       hHash = 0;}
  if (hProv) {CryptReleaseContext(hProv, 0); hProv = 0;}
  }


bool Crypt::acquireContext()
          {return hProv ? true : CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, 0) != 0;}


bool Crypt::createHash() {return CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash) != 0;}



bool Crypt::deriveKey(String& key) {
DWORD n = (DWORD) key.length();

  if (!CryptHashData(hHash, (BYTE*) key.str(), n, 0)) return false;

  return CryptDeriveKey(hProv, CALG_RC4, hHash, CRYPT_EXPORTABLE, &hKey) != 0;
  }


