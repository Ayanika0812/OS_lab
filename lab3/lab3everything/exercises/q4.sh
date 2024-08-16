echo "enter number"
read n


x=1
for ((y=n; y>0; y--))
do
x=$((x*y))
done
echo "$x"
