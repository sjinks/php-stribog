# php-stribog [![Build Status](https://travis-ci.org/sjinks/php-stribog.png?branch=master)](https://travis-ci.org/sjinks/php-stribog)

GOST R 34.11-2012 (stribog) Hash Function implementation for PHP.

## Introduction

This extension provides implementation of GOST R 34.11-2012 256 and 512 bit hash functions to be used with PHP [hash](http://php.net/hash) extension.

The extension does not introduce any new functions; instead, it registers three hashes:
  * `stribog-256` (GOST R 34.11-2012 256 bit hash function);
  * `stribog-512` (GOST R 34.11-2012 512 bit hash function);
  * `stribog` (alias of `stribog-512`).

These hashes can be used with any function from PHP `hash` extension.

The implementation of the hash function (`gost3411-2012-*.c`) is based on the implementation by [Alexey Degtyarev](https://www.streebog.net/en/), see `LICENSE.gost`.
The changes made to Alexey's code:
  * the MMX implementation was almost completely rewritten in order not to use SSE instructions;
  * optimization of 64-bit version of `extract()` — gained extra 0.7 MB/s;
  * the generic implementation was optimized by removing unnecessary `memcpy` calls;
  * inline functions instead of macros;
  * the implementation (generic/MMX/SSE) is chosen at runtime depending on CPU capabilities (GCC 4.6 or newer is required for that).

## Installing/Configuring

```bash
phpize
./configure
make
sudo make install
```

There are no configuration directives defined in `php.ini`.

## Usage Example

**Hash calculation:**

```php
$data = '012345678901234567890123456789012345678901234567890123456789012';
echo hash('stribog256', $data), PHP_EOL;
echo hash('stribog512', $data), PHP_EOL;
echo hash('stribog', $data), PHP_EOL;
```

**HMAC calculation:**

```php
$data = 'Very secret data';
$key  = 'password';
echo hash_hmac('stribog', $data, $key);
```
