#ifndef _ABSOCKET_H
#define _ABSOCKET_H

#include <stdbool.h>
#include <stddef.h>
#include <unistd.h>

#ifdef USE_OPENSSL
#include <openssl/ossl_typ.h>
#endif

#include "urlparse.h"

/*
 * Abstract socket utility, handles adding SSL if necessary.
 */

struct absocket {
	int basefd;
	bool use_ssl;
#ifdef USE_OPENSSL
	SSL *ssl;
	SSL_CTX *ctx;
	X509 *cert;
#endif
};
typedef struct absocket absocket_t;

void abs_init();
absocket_t *absocket_new(const struct uri *uri, bool use_ssl);
void absocket_free(absocket_t *socket);
ssize_t ab_recv(absocket_t *socket, void *buffer, size_t len);
ssize_t ab_send(absocket_t *socket, void *buffer, size_t len);
bool ab_enable_ssl(absocket_t *socket);

#endif
