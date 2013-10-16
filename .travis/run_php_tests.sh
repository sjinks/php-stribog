#! /bin/bash

DIR=$(readlink -enq "$(dirname $0)/../")

shopt -s nullglob
export NO_INTERACTION=1
export REPORT_EXIT_STATUS=1
make -C "$DIR" test
for i in $DIR/tests/*.log; do
	echo "====== $i ======";
	cat "$i";
done
[ -n "$(echo $DIR/tests/*.log)" ] && exit 1
exit 0
