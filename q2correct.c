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

int main() {
    int number;
    int *numbers = malloc(100 * sizeof(int)); // Allocate memory for up to 100 integers
    if (numbers == NULL) {
        perror("Failed to allocate memory");
        return EXIT_FAILURE;
    }

    int count = 0;

    printf("Enter non-negative integers (enter a negative number to stop):\n");
    
    while (1) {
        printf("Number: ");
        scanf("%d", &number);
        
        if (number < 0) {
            break; // Exit if the user enters a negative number
        }

        numbers[count++] = number; // Store the entered number
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

./q2 
Enter non-negative integers (enter a negative number to stop):
Number: 4
Number: 6 
Number: 8
Number: 1
Number: -1
The sum of the provided non-negative integers is: 19
*/

