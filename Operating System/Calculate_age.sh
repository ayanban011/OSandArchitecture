d=`echo $n | cut -c1-2`
m=`echo $n | cut -c3-4`
y=`echo $n | cut -c5-8`
yy=`date "+%Y"`
mm=`date "+%m"`
dd=`date "+%d"`
if [ $y -le $yy ]
then
yyy=`expr $yy - $y`
mmm=`expr $mm - $m`
#ddd=`expr $dd - $d`
if [ $m -gt $mm ]
then
yyy=`expr $yyy - 1`
mmm=`expr $mmm + 12`
fi
if [ $d -gt $dd ]
then
ddd=`expr $d - $dd`
ddd=`expr 31 - $ddd`
else
ddd=`expr $dd - $d`
fi
fi
echo "Your age : $yyy years $mmm months $ddd days"
