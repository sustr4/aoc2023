#!/bin/bash

if [ "$1" == "" ]; then
	FILE="progres-local.txt"
else
	FILE="$1"
fi

for i in $(seq 0 62); do


	for y in $(seq 0 63 1000); do

		N=$(( $i + $y ))

		cat "$FILE" | awk '{ print $1 }' | sed 's/[^0-9]//g' | grep -w "$N" >/dev/null
		if [ $? -eq 0 ]; then 
			printf "   %3d" $N
		else
			printf "      "
		fi

	done
	printf "\n"
done

echo "$FILE"
