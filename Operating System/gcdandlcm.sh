printf "Enter first  nuumber: "
read n1
printf "Enter second  nuumber: "
read n2
m=$n1
n=$n2
r=$n2
while [ $r -ne 0  ];do
  r=$(( n1%n2 ))
  if [ $r -eq 0 ];then
        break
  else
     ((n1=$n2))
     ((n2=$r))
  fi
done
printf  "GCD of %d and %d is %d \n" $m $n $n2
printf  "LCM Of %d and %d is %d \n" $m $n $((($m*$n)/$n2))
