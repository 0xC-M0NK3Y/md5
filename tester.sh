#!/bin/bash

# script to test if my implementation works
make

MAX_STRING_LEN=5000
TEST_NB=1000
ERR_NB=0

echo ""
echo "[md5 TESTER] start"
echo Effectuing $TEST_NB tests with max string length $MAX_STRING_LEN

for ((i = 0; i < TEST_NB; i++));
do
	LEN=$(($RANDOM % $MAX_STRING_LEN))
	rand_string=$(head /dev/urandom | tr -dc 'a-zA-Z0-9' | head -c $LEN)
	diff <(echo -n  $rand_string | md5sum | awk '{print $1}') <(./bin/md5 "$rand_string")
	if [[ $? -ne 0 ]]
	then
		echo "Not same hash with:"
		echo $rand_string | cat -e
		echo ""
		((ERR_NB++))
	fi
done

echo $ERR_NB erros on $TEST_NB tests
echo "[md5 TESTER] end"
echo ""
