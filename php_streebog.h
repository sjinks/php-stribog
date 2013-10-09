#ifndef PHP_STREEBOG_H
#define PHP_STREEBOG_H

#include <main/php.h>
#include <ext/hash/php_hash.h>

#define PHP_STREEBOG_EXTNAME  "streebog"
#define PHP_STREEBOG_EXTVER   "0.1"

#ifndef ZEND_MOD_END
#define ZEND_MOD_END { NULL, NULL, NULL, 0 }
#endif

#if defined(__GNUC__) && __GNUC__ >= 4
#	define STREEBOG_VISIBILITY_HIDDEN __attribute__((visibility("hidden")))
#else
#	define STREEBOG_VISIBILITY_HIDDEN
#endif


ZEND_DLEXPORT extern const php_hash_ops streebog256_hash_ops;
ZEND_DLEXPORT extern const php_hash_ops streebog512_hash_ops;

#ifdef COMPILE_DL_STREEBOG
STREEBOG_VISIBILITY_HIDDEN
#endif
extern zend_module_entry streebog_module_entry;

#define phpext_streebog_ptr &streebog_module_entry

#endif /* PHP_STREEBOG_H */
