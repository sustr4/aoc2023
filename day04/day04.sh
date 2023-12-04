#!/bin/bash

SUM=0

LNUM=0

declare -a REPS

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


	for tok in $WINNERS; do
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
		for i in `seq $MATCH`; do
			COPYNO=$(( $LNUM + $i ))
			echo $COPYNO
			echo "${REPS[$COPYNO]} * ${REPS[$LNUM]}"
			REPS[$COPYNO]=$( echo "${REPS[$COPYNO]} + ${REPS[$LNUM]}" | bc )
		done
	fi

	echo Line ${LNUM}: $MATCH matches, repeats ${REPS[$LNUM]} times

	REPSUM=$(( $REPSUM + ${REPS[$LNUM]} ))
	SUM=$(( $SUM + $MUL ))
#	echo Score $MUL, running sum $SUM
	echo running sum $REPSUM

done <<< $(cat input.txt)
#done <<< $(cat unit1.txt)
