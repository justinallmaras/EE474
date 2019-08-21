#!/bin/bash

dir="LAB4"

if [ -d $dir ]
then
	echo "ERROR:  Script cannot when there is a directory named $dir"
	exit 1
fi


mkdir $dir
for file in $@
do
	if [ -e $file ]
	then
		cp $file $dir
	else
		echo '$file does not exist\n'
	fi
done
scp -r $dir root@192.168.7.2:/root/

rm -rf $dir

echo 'Finished'