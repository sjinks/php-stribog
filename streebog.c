#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <main/php.h>
#include <Zend/zend.h>
#include <ext/hash/php_hash.h>
#include <ext/standard/info.h>
#include "php_streebog.h"
#include "gost3411-2012.h"

static void streebog256_init(void* context)
{
	size_t offset = (((size_t)context + 15) & ~0x0F) - (size_t)context;
	void *ctx     = (char*)context + offset;
	GOST34112012Init(ctx, 256);
}

static void streebog512_init(void* context)
{
	size_t offset = (((size_t)context + 15) & ~0x0F) - (size_t)context;
	void *ctx     = (char*)context + offset;
	GOST34112012Init(ctx, 512);
}

static void streebog_update(void* context, const unsigned char* buf, unsigned int count)
{
	size_t offset = (((size_t)context + 15) & ~0x0F) - (size_t)context;
	void *ctx     = (char*)context + offset;
	GOST34112012Update(ctx, buf, count);
}

static void streebog_final(unsigned char* digest, void* context)
{
	size_t offset = (((size_t)context + 15) & ~0x0F) - (size_t)context;
	void *ctx     = (char*)context + offset;
	GOST34112012Final(ctx, digest);
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
	(sizeof(GOST34112012Context) + 15)
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
	(sizeof(GOST34112012Context) + 15)
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
	php_info_print_table_start();
	php_info_print_table_row(2, "GOST R 34.11-2012 hash functions", "enabled");
	php_info_print_table_row(2, "Version", PHP_STREEBOG_EXTVER);
	php_info_print_table_end();
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
