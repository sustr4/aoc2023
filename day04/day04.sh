#!/bin/bash

SUM = 0

while read line; do

	MUL=0

	echo $line

	WINNERS=`echo $line | sed 's/.*://' | sed 's/|.*//'`

	CARD=`echo $line | sed 's/.*|//'`

	echo Winners: $WINNERS
	echo Cards: $CARD

	for tok in $WINNERS; do
		echo $CARD | grep -o "\b$tok\b"
		if [ $? -eq 0 ]; then
			if [ $MUL -eq 0 ]; then
				MUL=1
			else
				MUL=$(( MUL * 2 ))
			fi
		fi
	done

	SUM=$(( $SUM + $MUL ))
	echo Score $MUL, running sum $SUM

done <<< $(cat input.txt)
