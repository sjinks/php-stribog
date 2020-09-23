#ifndef PHP_STRIBOG_H
#define PHP_STRIBOG_H

#include <main/php.h>
#include <ext/hash/php_hash.h>

#define PHP_STRIBOG_EXTNAME  "stribog"
#define PHP_STRIBOG_EXTVER   "1.0.0"

#ifndef ZEND_MOD_END
#define ZEND_MOD_END { NULL, NULL, NULL, 0 }
#endif

#if defined(__GNUC__) && __GNUC__ >= 4
#	define STRIBOG_VISIBILITY_HIDDEN __attribute__((visibility("hidden")))
#else
#	define STRIBOG_VISIBILITY_HIDDEN
#endif


ZEND_DLEXPORT extern const php_hash_ops stribog256_hash_ops;
ZEND_DLEXPORT extern const php_hash_ops stribog512_hash_ops;

#ifdef COMPILE_DL_STRIBOG
STRIBOG_VISIBILITY_HIDDEN
#endif
extern zend_module_entry stribog_module_entry;

#define phpext_stribog_ptr &stribog_module_entry

#endif /* PHP_STRIBOG_H */
