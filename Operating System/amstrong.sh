echo "Enter a number: "
read c
function armstrong {
x=$1
sum=0
r=0
n=0
l=${#x}
while [ $x -gt 0 ]
do
r=`expr $x % 10`

n=$(bc <<< "$r^$l")

sum=`expr $sum + $n`

x=`expr $x / 10`
done

if [ $sum -eq $c ]
then
echo "It is an Armstrong Number."
else
echo "It is not an Armstrong Number."
fi

}

result=`armstrong $c`
echo $result
