<?php
ini_set('memory_limit', '2G');
$size  = 200;
$data  = str_repeat('x', $size * 1048576);
$algos = hash_algos();
foreach ($algos as $algo) {
	$b = microtime(true);
	$h = hash($algo, $data);
	$d = microtime(true) - $b;
	echo $algo, ":\t", sprintf("%5f s, %5f MB/s", $d, $size / $d), "\n";
}

echo "\n";
?>
