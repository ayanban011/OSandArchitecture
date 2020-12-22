#!/bin/bash
#generate pascal triangle
echo -n "Enter the number of Row "
read NR

typeset -A arr
#declare -a arr

for i in `seq 0 $NR`;do
 arr[$i,0]=1 #start is 1
 arr[$i,$i]=1  #end is 1
 p=$((i-1))
 #for j in `seq 1 $p`;do 
  for ((j=1;j<$i;j++));do
   a=${arr[$((i-1)),$((j-1))]}
   b=${arr[$((i-1)),$j]}
   arr[$i,$j]=$((a+b))
 done
#echo ${arr[$i]}
done
#print result
for ((i=0;i<=$NR;i++));do
for((j=0;j<=$i;j++))
do
  echo -n ${arr[$i,$j]} " "
done
 printf "\n"
done
