echo "Enter a number"
read n
a=`expr $n % 2`
if [ $a -eq 0 ]
then
  echo "$n is an even number"
else
  echo "$n is an odd number"
fi
