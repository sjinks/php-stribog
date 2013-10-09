#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <main/php.h>
#include <Zend/zend.h>
#include <ext/hash/php_hash.h>
#include "php_streebog.h"
#include "gost3411-2012.h"

static void streebog256_init(void* context)
{
	GOST34112012Init(context, 256);
}

static void streebog512_init(void* context)
{
	GOST34112012Init(context, 512);
}

static void streebog_update(void* context, const unsigned char* buf, unsigned int count)
{
	GOST34112012Update(context, buf, count);
}

static void streebog_final(unsigned char* digest, void* context)
{
	GOST34112012Final(context, digest);
}

const php_hash_ops streebog256_hash_ops = {
	streebog256_init,
	streebog_update,
	streebog_final,
#if PHP_VERSION_ID >= 50300
	(php_hash_copy_func_t)php_hash_copy,
#endif
	32,
	64,
	sizeof(GOST34112012Context)
};

const php_hash_ops streebog512_hash_ops = {
	streebog512_init,
	streebog_update,
	streebog_final,
#if PHP_VERSION_ID >= 50300
	(php_hash_copy_func_t)php_hash_copy,
#endif
	64,
	64,
	sizeof(GOST34112012Context)
};

static PHP_MINIT_FUNCTION(streebog)
{
	php_hash_register_algo("streebog256", &streebog256_hash_ops);
	php_hash_register_algo("streebog512", &streebog512_hash_ops);
	php_hash_register_algo("streebog",    &streebog512_hash_ops);
	return SUCCESS;
}

static PHP_MINFO_FUNCTION(streebog)
{
}

static
#if ZEND_MODULE_API_NO > 20060613
const
#endif
zend_module_dep streebog_deps[] = {
	ZEND_MOD_REQUIRED("hash")
	ZEND_MOD_END
};

zend_module_entry streebog_module_entry = {
	STANDARD_MODULE_HEADER_EX,
	NULL,
	streebog_deps,
	PHP_STREEBOG_EXTNAME,
	NULL,
	PHP_MINIT(streebog),
	NULL,
	NULL,
	NULL,
	PHP_MINFO(streebog),
	PHP_STREEBOG_EXTVER,
	NO_MODULE_GLOBALS,
	NULL,
	STANDARD_MODULE_PROPERTIES_EX
};

#ifdef COMPILE_DL_STREEBOG
ZEND_GET_MODULE(streebog)
#endif
