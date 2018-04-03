#!/bin/bash
count=0
for i in `seq 1 100`
do
    count=`expr $count + 1`
    telnet localhost 8080 2>&1 &
done
echo "send $count"
