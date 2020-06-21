#!/bin/bash

echo "Enter a sequence of number followed by 'end' : "
#This is our storage.
declare  arr
#This is for the index of the array.
index=0

while [ NUM != "end" ]; do
	read NUM  # We are reading the numbers line by line.
	arr[index]=$NUM #Now numbers are in our storage.
	if [ $NUM == "end" ]; then
		break
	fi
	index=$((index+1))
done

#Let's assume that the first number is the max number.

MAX=$((arr[0]))

#Finding maximum algorithm.
for ((i=0;i<index;i++));do
	if ((arr[i]>MAX));then
		MAX=$((arr[i]))
	fi
done
#Print max
echo The maximum number is: $((MAX))
