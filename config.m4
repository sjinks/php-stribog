PHP_ARG_ENABLE(stribog, whether to enable GOST R 34.11-2012 Hash Function, [ --enable-stribog  Enable GOST R 34.11-2012 Hash Function])

if test "$PHP_STRIBOG" = "yes"; then
	PHP_C_BIGENDIAN
	AC_CHECK_HEADERS([cpuid.h])

	AC_DEFINE([HAVE_STRIBOG], [1], [Whether GOST R 34.11-2012 Hash Function is supported])
	PHP_NEW_EXTENSION([stribog], [stribog.c tables.c gost3411-2012.c gost3411-2012-ref.c gost3411-2012-mmx.c gost3411-2012-sse2.c gost3411-2012-sse41.c], $ext_shared,, [-D_GNU_SOURCE])
	PHP_ADD_EXTENSION_DEP([stribog], [hash])
fi
