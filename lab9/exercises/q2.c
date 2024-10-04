#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *numbers;
    int count;
    int result;
} SumData;

// Function to calculate the sum of non-negative integers
void* calculate_sum(void* param) {
    SumData* data = (SumData*)param;
    data->result = 0;

    for (int i = 0; i < data->count; i++) {
        data->result += data->numbers[i];
    }

    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <number1> <number2> ... <numberN>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int count = argc - 1;
    int* numbers = malloc(count * sizeof(int));
    if (numbers == NULL) {
        perror("Failed to allocate memory");
        return EXIT_FAILURE;
    }

    // Convert command line arguments to integers
    for (int i = 0; i < count; i++) {
        numbers[i] = atoi(argv[i + 1]);
        if (numbers[i] < 0) {
            fprintf(stderr, "Please enter only non-negative integers.\n");
            free(numbers);
            return EXIT_FAILURE;
        }
    }

    SumData sumData = {numbers, count, 0};
    pthread_t thread;

    // Create a new thread to calculate the sum
    if (pthread_create(&thread, NULL, calculate_sum, &sumData) != 0) {
        perror("Failed to create thread");
        free(numbers);
        return EXIT_FAILURE;
    }

    // Wait for the child thread to finish
    pthread_join(thread, NULL);

    // Output the result
    printf("The sum of the provided non-negative integers is: %d\n", sumData.result);

    // Free allocated memory
    free(numbers);
    
    return EXIT_SUCCESS;
}

/*

./q2 1 2 3
The sum of the provided non-negative integers is: 6
~/Desktop/220905128/lab9/exercises$ ./q2 1 2 3 4
The sum of the provided non-negative integers is: 10

*/
