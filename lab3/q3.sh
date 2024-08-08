# Find all the possible quadratic equation roots using case.
#!/bin/bash

echo "Enter the value of a:"
read a
echo "Enter the value of b:"
read b
echo "Enter the value of c:"
read c

# Calculate the discriminant
d=$((b * b - 4 * a * c))

# Determine the nature of the roots using a case statement
if (( d > 0 )); then
    sqrt=$(echo "scale=2; sqrt($d)" | bc -l)
    root1=$(((-$b + $sqrt) / (2 * $a)))
    root2=$(((-$b - $sqrt) / (2 * $a)))
    echo "Roots are real and different:"
    echo "Root 1: $root1"
    echo "Root 2: $root2"
elif (( d == 0 )); then
    root=$(echo "scale=2; -$b / (2 * $a)" | bc -l)
    echo "Roots are real and the same:"
    echo "Root: $root"
else
    realPart=$(echo "scale=2; -$b / (2 * $a)" | bc -l)
    imagPart=$(echo "scale=2; sqrt(-$d) / (2 * $a)" | bc -l)
    echo "Roots are complex:"
    echo "Root 1: $realPart + ${imagPart}i"
    echo "Root 2: $realPart - ${imagPart}i"
fi

