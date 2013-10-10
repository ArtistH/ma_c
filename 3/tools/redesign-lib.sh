#!/usr/bin/bash
set -e

# -i change the files
for file in `find -type f -name *.c`
do
	sed -i 's/^#include "/#include "basic\//g' $file
done
