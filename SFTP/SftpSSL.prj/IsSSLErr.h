// Determine SSL Error


#pragma once
#include <openssl/ssl.h>


SSLRslt sslRslt(SSL* ssl, int e);

