# Find the factorial of a given number
#!/bin/bash

echo "Enter the number: "
read n
m=$((n))
# or m=$n
f=1
while (( n > 0 )); do
    f=$((f * n))
    n=$((n - 1))
done

echo "The factorial of $m is $f"

