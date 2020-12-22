#!/bin/bash

if [ -r /tmp/myfile.txt ]
then
     echo "File has read permission"
else
     echo "You don't have read permission"
fi
if [ -w /tmp/myfile.txt ]
then
     echo "File has write permission"
else
     echo "You don't have write permission"
fi

if [ -x /tmp/myfile.txt ]
then
     echo "File has execute permission"
else
     echo "You don't have execute permission"
fi
