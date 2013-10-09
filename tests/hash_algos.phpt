--TEST--
hash_algos()
--SKIPIF--
<?php if (!extension_loaded('streebog')) die('skip'); ?>
--FILE--
<?php
var_dump(in_array('streebog', hash_algos()));
var_dump(in_array('streebog256', hash_algos()));
var_dump(in_array('streebog512', hash_algos()));
?>
--EXPECT--
bool(true)
bool(true)
bool(true)
