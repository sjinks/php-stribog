--TEST--
hash_algos()
--SKIPIF--
<?php if (!extension_loaded('stribog')) die('skip'); ?>
--FILE--
<?php
var_dump(in_array('stribog', hash_algos()));
var_dump(in_array('stribog256', hash_algos()));
var_dump(in_array('stribog512', hash_algos()));
?>
--EXPECT--
bool(true)
bool(true)
bool(true)
