## Print the first ‘n’ odd numbers.


echo "Enter value of n:"
read n
i=1
x=1

while (( i <= n )); do
    echo $x
    #x=$((x + 2))  # Increment x by 2 to get the next odd number
    #i=$((i + 1))  # Increment i by 1
    x=$(expr $x \+ 2)
    i=$(expr $i \+ 1)
done

