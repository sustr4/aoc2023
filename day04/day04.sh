#!/bin/bash

SUM=0
LNUM=0

declare -a REPS # Declare array and pre-fill with starting count
TOTALLINES=`cat input.txt | wc -l`
for i in `seq $TOTALLINES`; do
	REPS[$i]=1
done

while read line; do

	LNUM=$(( $LNUM + 1))
	MUL=0
	MATCH=0

	WINNERS=`echo $line | sed 's/.*://' | sed 's/|.*//'`
	CARD=`echo $line | sed 's/.*|//'`

	for tok in $WINNERS; do # Count matches
		echo $CARD | grep -o "\b$tok\b" >/dev/null
		if [ $? -eq 0 ]; then
			MATCH=$(($MATCH + 1));
			if [ $MUL -eq 0 ]; then
				MUL=1
			else
				MUL=$(( MUL * 2 ))
			fi
		fi
	done

	if [ $MATCH -gt 0 ]; then # There were matches, let's spread the count
		for i in `seq $MATCH`; do # Each of the following cards will appear more by the count of the current one.
			COPYNO=$(( $LNUM + $i ))
			REPS[$COPYNO]=$(( ${REPS[$COPYNO]} + ${REPS[$LNUM]} ))
		done
	fi

	REPSUM=$(( $REPSUM + ${REPS[$LNUM]} ))
	SUM=$(( $SUM + $MUL ))
	echo Task 1: Score $MUL, running sum $SUM
	echo Task 2: $MATCH matches, card repeats ${REPS[$LNUM]} times, running sum $REPSUM

done <<< $(cat input.txt)
#done <<< $(cat unit1.txt)
