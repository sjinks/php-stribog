PHP_ARG_ENABLE(streebog, whether to enable GOST R 34.11-2012 Hash Function, [ --enable-streebog  Enable GOST R 34.11-2012 Hash Function])

if test "$PHP_STREEBOG" = "yes"; then
	PHP_C_BIGENDIAN
	AC_CHECK_HEADERS([cpuid.h])

	AC_DEFINE([HAVE_STREEBOG], [1], [Whether GOST R 34.11-2012 Hash Function is supported])
	PHP_NEW_EXTENSION([streebog], [streebog.c tables.c gost3411-2012.c gost3411-2012-ref.c gost3411-2012-mmx.c gost3411-2012-sse2.c], $ext_shared)
	PHP_ADD_EXTENSION_DEP([streebog], [hash])
fi
