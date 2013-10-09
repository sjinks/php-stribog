# php-streebog ([![Build Status](https://travis-ci.org/sjinks/php-streebog.png?branch=master)](https://travis-ci.org/sjinks/php-streebog))

GOST R 34.11-2012 (Streebog) Hash Function implementation for PHP.

## Introduction

This extension provides implementation of GOST R 34.11-2012 256 and 512 bit hash functions to be used with PHP [hash](http://php.net/hash) extension.

The extension does not introduce any new functions; instead, it registers three hashes:
  * `streebog-256` (GOST R 34.11-2012 256 bit hash function);
  * `streebog-512` (GOST R 34.11-2012 512 bit hash function);
  * `streebog` (alias of `streebog-512`).

These hashes can be used with any function from PHP `hash` extension.

The implementation of the hash function (`gost3411-2012.c`) is based on the implementation by Alexey Degtyarev <alexey@renatasystems.org>, see `LICENSE.gost`

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
echo hash('streebog256', $data), PHP_EOL;
echo hash('streebog512', $data), PHP_EOL;
echo hash('streebog', $data), PHP_EOL;
```

**HMAC calculation:**

```php
$data = 'Very secret data';
$key  = 'password';
echo hash_hmac('streebog', $data, $key);
```
