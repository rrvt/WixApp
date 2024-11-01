// A little class to save and restore names and passwords (or anytext) using
// Cryptography API : Next Generation (CNG)


#pragma once
#include "CNGblock.h"
#include <bcrypt.h>        // Be sure to add: Bcrypt.lib to list of extera libraryies


class CNG {

BCRYPT_ALG_HANDLE hAES;
BCRYPT_KEY_HANDLE hKey;
CNGblock          plainBlk;       // Incoming or outgoing plain text
CNGblock          ivBlk;          // Initial value (set to initial value by app)
CNGblock          keyBlk;         // Provider (i.e. AES) subobject, in this case the processed key
CNGblock          cipherBlk;      // encrypted result

public:

  CNG() : hAES(0), hKey(0) { }
 ~CNG();

  CNGblock* operator() (TCchar* tc,    TCchar* key);    // Accepts a string and key, returns a
                                                        // zero or ptr to an encrypted block
  TCchar*   operator() (CNGblock& blk, TCchar* key);    // Accepts an encrypted block and key,
                                                        // returns zero or ptr to a string
private:

  bool setProvider();
  bool getInitalValue();
  bool generateKey(TCchar* key);
  };

