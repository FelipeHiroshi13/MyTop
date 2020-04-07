#!/bin/bash

PATHPID='/proc/'

PATH_TRAB=$PWD

cd $PATHPID

ELEMENTS=ls

echo $ELEMENTS 

for i in `$ELEMENTS` 
do
	## To verify if is a PID and if the file status exists
	if [[ ${i} =~ ^([0-9]+)$ ]] && [ -e $path$i'/status' ];
	then
		$PATH_TRAB/./mytop $PATHPID$i'/status'
	fi
done


