#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_FIB 100

// Shared data structure
int *fibonacci;
int num_fib;

// Function to generate Fibonacci numbers
void* generate_fibonacci(void* param) {
    for (int i = 0; i < num_fib; i++) {
        if (i == 0) {
            fibonacci[i] = 0; // First Fibonacci number
        } else if (i == 1) {
            fibonacci[i] = 1; // Second Fibonacci number
        } else {
            fibonacci[i] = fibonacci[i - 1] + fibonacci[i - 2]; //F(n) = F(n-1) + F(n-2).
        }
    }
    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <number_of_fibonacci_numbers>\n", argv[0]);
        return EXIT_FAILURE;
    }

    num_fib = atoi(argv[1]); //input to integer
    if (num_fib <= 0 || num_fib > MAX_FIB) {
        fprintf(stderr, "Please enter a positive number up to %d.\n", MAX_FIB);
        return EXIT_FAILURE;
    }

    // Allocate memory for the Fibonacci numbers
    fibonacci = malloc(num_fib * sizeof(int));
    if (fibonacci == NULL) {
        perror("Failed to allocate memory");
        return EXIT_FAILURE;
    }

    pthread_t thread;

    // Create a new thread to generate Fibonacci numbers
    if (pthread_create(&thread, NULL, generate_fibonacci, NULL) != 0) {
        perror("Failed to create thread");
        free(fibonacci);
        return EXIT_FAILURE;
    }

    // Wait for the child thread to finish
    pthread_join(thread, NULL);

    // Print the generated Fibonacci sequence
    printf("Fibonacci series (first %d numbers):\n", num_fib);
    for (int i = 0; i < num_fib; i++) {
        printf("%d ", fibonacci[i]);
    }
    printf("\n");

    // Free allocated memory
    free(fibonacci);
    
    return EXIT_SUCCESS;
}

/*

./q1 10
Fibonacci series (first 10 numbers):
0 1 1 2 3 5 8 13 21 34 
~/Desktop/220905128/lab9/exercises$ ./q1 5
Fibonacci series (first 5 numbers):
0 1 1 2 3 

*/
