#!/bin/bash

set -o nounset
# set -x
#set -e

for dir in `ls .`
do
	if [ -d $dir ]
	then
		cd $dir

		if [ -f ex-1.c ]
		then
			mv ex-1.c ../"$dir""-1.c"
		fi
		if [ -f ex-2.c ]
		then
			mv ex-2.c ../"$dir""-2.c"
		fi
		if [ -f ex-3.c ]
		then
			mv ex-3.c ../"$dir""-3.c"
		fi

		cd ..
		rm -r $dir
	fi
done
