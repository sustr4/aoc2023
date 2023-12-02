#!/bin/bash

INPUT="input.txt"

#12 red cubes, 13 green cubes, and 14 blue cubes

SUM=0
cat $INPUT | while read line; do


	NUM=`echo $line | awk -F"[ :]" '{print $2}'`

	echo $NUM $line

	RED=`echo $line | grep -o '[0-9]*\sred' | awk '{print $1}'`
	GREEN=`echo $line | grep -o '[0-9]*\sgreen' | awk '{print $1}'`
	BLUE=`echo $line | grep -o '[0-9]*\sblue' | awk '{print $1}'`

	echo $RED
	echo $GREEN
	echo $BLUE

	LIN=$NUM

	for i in $RED; do
		if [ $i -gt 12 ]; then LIN=0; fi
	done
	for i in $GREEN; do
		if [ $i -gt 13 ]; then LIN=0; fi
	done
	for i in $BLUE; do
		if [ $i -gt 14 ]; then LIN=0; fi
	done
	echo $LIN

	SUM=$(( $SUM + $LIN))
	echo $SUM
done
