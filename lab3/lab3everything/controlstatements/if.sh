echo "enter a number: "
read number
if [ $number -lt 0 ] 
then 
   echo "negative" 
elif [ $number -eq 0 ] 
then
   echo "zero" 
else
   echo "positive"
fi
#echo "completed if"
#enter a number: 
#5
#positive
#enter a number: 
#-7
#negative
#enter a number: 
#0
#zero

