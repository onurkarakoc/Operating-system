#!/bin/bash

#This is for the command line argument
inputfile=$1
#We are reading the file.
while IFS= read -r line
do
	for((i=0;i<"$line";i++));do
		echo -ne "*"
	done
	echo
done < "$inputfile"


