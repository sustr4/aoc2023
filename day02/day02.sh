#!/bin/bash

INPUT="input.txt"
#INPUT="unit1.txt"

#12 red cubes, 13 green cubes, and 14 blue cubes

SUM=0

cat $INPUT | while read line; do

	MR=0
	MG=0
	MB=0

	RED=`echo $line | grep -o '[0-9]*\sred' | awk '{print $1}'`
	GREEN=`echo $line | grep -o '[0-9]*\sgreen' | awk '{print $1}'`
	BLUE=`echo $line | grep -o '[0-9]*\sblue' | awk '{print $1}'`

	echo Reds: $RED 
	echo Greens: $GREEN
	echo Blues: $BLUE

	for i in $RED; do if [ $i -gt $MR ]; then MR=$i; fi done
	for i in $GREEN; do if [ $i -gt $MG ]; then MG=$i; fi done
	for i in $BLUE; do if [ $i -gt $MB ]; then MB=$i; fi done
	echo Maxes: $MR $MG $MB
	
	POW=$(( $MR * $MG * $MB ))
	SUM=$(( $SUM + $POW ))
	echo Power ${POW}, running sum: $SUM
done
