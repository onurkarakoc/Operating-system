#!/bin/bash
#Here, we are checking the argument is entered or not.

if [ -z $1 ]
then
	searchDir=$PWD #To find current directory
else 
	searchDir=$1
fi

#İf the user enter a command line argument;we are going to
#user's directory. If the user don't enter a command line argument
#We are going to current directory.
#Then we are deleting the all zero length files.
#With wc -l , we can know how many files will be deleted.
	
find $searchDir -size 0|wc -l 
find $searchDir -size 0 -delete # now delete 0 length file
echo zero-length files are removed from the directory: $searchDir


