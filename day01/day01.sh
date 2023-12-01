
INPUT="input.txt"
#INPUT="unit1.txt"

DBL=$(cat $INPUT | grep -o '[0-9].*[0-9]' | sed 's/\(.\).*\(.\)/\1\2/' | awk '{sum=sum+$1} END {print sum}')

SING=$(cat $INPUT | grep -v '[0-9].*[0-9]' | sed 's/.*\([0-9]\).*/\1\1/' | awk '{sum=sum+$1} END {print sum}')

echo $DBL + $SING
echo $DBL + $SING | bc
