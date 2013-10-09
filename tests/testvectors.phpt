--TEST--
GOST R 34.11-2012: official test vectors
--SKIPIF--
<?php if (!extension_loaded('streebog')) die('skip'); ?>
--FILE--
<?php
$vec = '012345678901234567890123456789012345678901234567890123456789012';
echo hash('streebog256', $vec), PHP_EOL;
echo hash('streebog512', $vec), PHP_EOL;
echo hash('streebog', $vec), PHP_EOL;

$vec = base64_decode('0eUg4uXy8OgsINHy8Ojh7uboIOLt8/boLCDi5f7y+iDxIOzu8P8g8fLw5evg7Ogg7eAg9fDg4fD7/yDv6/rq+yDI4+7w5eL7');
echo hash('streebog256', $vec), PHP_EOL;
echo hash('streebog512', $vec), PHP_EOL;
echo hash('streebog', $vec), PHP_EOL;
?>
--EXPECT--
9d151eefd8590b89daa6ba6cb74af9275dd051026bb149a452fd84e5e57b5500
1b54d01a4af5b9d5cc3d86d68d285462b19abc2475222f35c085122be4ba1ffa00ad30f8767b3a82384c6574f024c311e2a481332b08ef7f41797891c1646f48
1b54d01a4af5b9d5cc3d86d68d285462b19abc2475222f35c085122be4ba1ffa00ad30f8767b3a82384c6574f024c311e2a481332b08ef7f41797891c1646f48
9dd2fe4e90409e5da87f53976d7405b0c0cac628fc669a741d50063c557e8f50
1e88e62226bfca6f9994f1f2d51569e0daf8475a3b0fe61a5300eee46d961376035fe83549ada2b8620fcd7c496ce5b33f0cb9dddc2b6460143b03dabac9fb28
1e88e62226bfca6f9994f1f2d51569e0daf8475a3b0fe61a5300eee46d961376035fe83549ada2b8620fcd7c496ce5b33f0cb9dddc2b6460143b03dabac9fb28
