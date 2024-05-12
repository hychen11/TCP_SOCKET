#!/bin/bash
#PORT $2 is like argv[2]!
MACHINE=${1:-localhost}
PORT=$2
NUM_PLAYERS=$3
sleep 1 
for ((i=1; i<=$NUM_PLAYERS; i++))
do
    ./player $MACHINE $PORT &
done
wait