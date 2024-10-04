#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    int start;
    int end;
    int *primes;
    int count;
} PrimeData;

// Function to check if a number is prime
bool is_prime(int num) {
    if (num < 2) return false;
    for (int i = 2; i * i <= num; i++) {
        if (num % i == 0) return false;
    }
    return true;
}

// Function to generate prime numbers
void* generate_primes(void* param) {
    PrimeData* data = (PrimeData*)param;
    data->count = 0;

    for (int i = data->start; i <= data->end; i++) {
        if (is_prime(i)) {
            data->primes[data->count++] = i;
        }
    }
    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <start_number> <end_number>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int start = atoi(argv[1]);
    int end = atoi(argv[2]);

    if (start > end) {
        fprintf(stderr, "Start number must be less than or equal to end number.\n");
        return EXIT_FAILURE;
    }

    // Allocate memory for the primes
    int range_size = end - start + 1;
    int* primes = malloc(range_size * sizeof(int));
    if (primes == NULL) {
        perror("Failed to allocate memory");
        return EXIT_FAILURE;
    }

    PrimeData primeData = {start, end, primes, 0};
    pthread_t thread;

    // Create a new thread to generate primes
    if (pthread_create(&thread, NULL, generate_primes, &primeData) != 0) {
        perror("Failed to create thread");
        free(primes);
        return EXIT_FAILURE;
    }

    // Wait for the child thread to finish
    pthread_join(thread, NULL);

    // Output the generated prime numbers
    printf("Prime numbers between %d and %d:\n", start, end);
    for (int i = 0; i < primeData.count; i++) {
        printf("%d ", primeData.primes[i]);
    }
    printf("\n");

    // Free allocated memory
    free(primes);

    return EXIT_SUCCESS;
}


/*

./q3 1 10
Prime numbers between 1 and 10:
2 3 5 7 
~/Desktop/220905128/lab9/exercises$ ./q3 10 30
Prime numbers between 10 and 30:
11 13 17 19 23 29 


*/
