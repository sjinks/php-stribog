--TEST--
hash_init()
--SKIPIF--
<?php if (!extension_loaded('stribog')) die('skip'); ?>
--FILE--
<?php
foreach (array('stribog256', 'stribog512', 'stribog') as $h) {
	$ctx = hash_init($h);
	hash_update($ctx, '012345678901234567890123456789012345678901234567890123456789012');
	echo hash_final($ctx), PHP_EOL;
}
?>
--EXPECT--
9d151eefd8590b89daa6ba6cb74af9275dd051026bb149a452fd84e5e57b5500
1b54d01a4af5b9d5cc3d86d68d285462b19abc2475222f35c085122be4ba1ffa00ad30f8767b3a82384c6574f024c311e2a481332b08ef7f41797891c1646f48
1b54d01a4af5b9d5cc3d86d68d285462b19abc2475222f35c085122be4ba1ffa00ad30f8767b3a82384c6574f024c311e2a481332b08ef7f41797891c1646f48
