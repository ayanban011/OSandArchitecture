a=$1
b=$2
c=$3
 
if [ $# -lt 3 ]
then
 echo "$0 n1 n2 n3"
 exit 1
fi
 
if [ $a -gt $b -a $a -gt $c ]
then
 echo "$a is largest integer"
elif [ $b -gt $a -a $b -gt $c ]
then
 echo "$b is largest integer"
elif [ $c -gt $a -a $c -gt $b ];
then
 echo "$c is largest integer"
else
 echo "Sorry cannot guess number"
fi
