#!/bin/bash
for file in "$1"/*;
do
   if [ -d "$file" ]
then
	continue;
   else
	filename="${file##*/}"
	pathname="${file%$filename}"
	filename="`echo $filename | tr [:upper:] [:lower:]`"
	mv -- "$file" "$pathname$filename"
   fi
done;
