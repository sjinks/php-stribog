--TEST--
Segfault on small input
--SKIPIF--
<?php if (!extension_loaded('stribog')) die('skip'); ?>
--FILE--
<?php
echo hash('stribog','IVANOVVVVV'), PHP_EOL;
echo hash('stribog','IVANOV'), PHP_EOL;
?>
--EXPECT--
4ccb958ee7969cfa2dc23b5b61abda3e5adbdc426ef1182cda992df7e5446ad2fa15d3f7b80d2862fd31254b94b078e7948baddc7e9cd239d6480109b321daeb
0c898fab3da2cb386e7a443a677683be3b177ffe32141cc8aad3e2e287fdef8c1c672932a3b2298d7ed7ff94c863ba5fe297fec62dbb18702b262276267cc854
