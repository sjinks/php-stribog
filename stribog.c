#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <main/php.h>
#include <Zend/zend.h>
#include <ext/hash/php_hash.h>
#include <ext/standard/info.h>
#include "php_stribog.h"
#include "gost3411-2012.h"

static void stribog256_init(void* context)
{
	size_t offset = (((size_t)context + 15) & ~0x0F) - (size_t)context;
	void *ctx     = (char*)context + offset;
	GOST34112012Init(ctx, 256);
}

static void stribog512_init(void* context)
{
	size_t offset = (((size_t)context + 15) & ~0x0F) - (size_t)context;
	void *ctx     = (char*)context + offset;
	GOST34112012Init(ctx, 512);
}

static void stribog_update(void* context, const unsigned char* buf, unsigned int count)
{
	size_t offset = (((size_t)context + 15) & ~0x0F) - (size_t)context;
	void *ctx     = (char*)context + offset;

	offset = (((size_t)buf + 15) & ~0x0F) - (size_t)buf;
	if (!offset) {
		GOST34112012Update(ctx, buf, count);
	}
	else {
		ALIGN(16) unsigned char tmp[15];
		assert(offset < 16);
		if (count > offset) {
			memcpy(tmp, buf, offset);
			GOST34112012Update(ctx, tmp, offset);
			GOST34112012Update(ctx, buf + offset, count - offset);
		}
		else {
			memcpy(tmp, buf, count);
			GOST34112012Update(ctx, tmp, count);
		}
	}
}

static void stribog_final(unsigned char* digest, void* context)
{
	size_t offset = (((size_t)context + 15) & ~0x0F) - (size_t)context;
	void *ctx     = (char*)context + offset;
	GOST34112012Final(ctx, digest);
}

const php_hash_ops stribog256_hash_ops = {
	stribog256_init,
	stribog_update,
	stribog_final,
#if PHP_VERSION_ID >= 50300
	(php_hash_copy_func_t)php_hash_copy,
#endif
	32,
	64,
	(sizeof(GOST34112012Context) + 15)
};

const php_hash_ops stribog512_hash_ops = {
	stribog512_init,
	stribog_update,
	stribog_final,
#if PHP_VERSION_ID >= 50300
	(php_hash_copy_func_t)php_hash_copy,
#endif
	64,
	64,
	(sizeof(GOST34112012Context) + 15)
};

static PHP_MINIT_FUNCTION(stribog)
{
	php_hash_register_algo("stribog256", &stribog256_hash_ops);
	php_hash_register_algo("stribog512", &stribog512_hash_ops);
	php_hash_register_algo("stribog",    &stribog512_hash_ops);
	return SUCCESS;
}

static PHP_MINFO_FUNCTION(stribog)
{
	php_info_print_table_start();
	php_info_print_table_row(2, "GOST R 34.11-2012 hash functions", "enabled");
	php_info_print_table_row(2, "Version", PHP_STRIBOG_EXTVER);
	php_info_print_table_end();
}

static
#if ZEND_MODULE_API_NO > 20060613
const
#endif
zend_module_dep stribog_deps[] = {
	ZEND_MOD_REQUIRED("hash")
	ZEND_MOD_END
};

zend_module_entry stribog_module_entry = {
	STANDARD_MODULE_HEADER_EX,
	NULL,
	stribog_deps,
	PHP_STRIBOG_EXTNAME,
	NULL,
	PHP_MINIT(stribog),
	NULL,
	NULL,
	NULL,
	PHP_MINFO(stribog),
	PHP_STRIBOG_EXTVER,
	NO_MODULE_GLOBALS,
	NULL,
	STANDARD_MODULE_PROPERTIES_EX
};

#ifdef COMPILE_DL_STRIBOG
ZEND_GET_MODULE(stribog)
#endif
