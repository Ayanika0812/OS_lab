# Find whether the given number is even or odd.
#!/bin/bash

echo "Enter the number: "
read n

#if (( n % 2 == 0 )); then
if [ $(expr $n % 2) -eq 0 ]; then
    echo "$n is an Even number"
else
    echo "$n is an Odd number"
fi

