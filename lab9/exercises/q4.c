#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *array;
    int size;
    int sum;
    int is_even; // 1 for even, 0 for odd
} SumData;

// Function to calculate the sum of even or odd numbers
void* calculate_sum(void* param) {
    SumData* data = (SumData*)param;
    data->sum = 0;

    for (int i = 0; i < data->size; i++) {
        if (data->is_even && data->array[i] % 2 == 0) {
            data->sum += data->array[i];
        } else if (!data->is_even && data->array[i] % 2 != 0) {
            data->sum += data->array[i];
        }
    }

    return NULL;
}

int main() {
    int size;

    // Input size of the array
    printf("Enter the number of elements: ");
    scanf("%d", &size);

    int* array = malloc(size * sizeof(int));
    if (array == NULL) {
        perror("Failed to allocate memory");
        return EXIT_FAILURE;
    }

    // Input array elements
    printf("Enter the elements of the array:\n");
    for (int i = 0; i < size; i++) {
        scanf("%d", &array[i]);
    }

    SumData evenData = {array, size, 0, 1}; // Even sum data
    SumData oddData = {array, size, 0, 0};  // Odd sum data

    pthread_t even_thread, odd_thread;

    // Create threads for even and odd sums
    if (pthread_create(&even_thread, NULL, calculate_sum, &evenData) != 0) {
        perror("Failed to create even thread");
        free(array);
        return EXIT_FAILURE;
    }
    if (pthread_create(&odd_thread, NULL, calculate_sum, &oddData) != 0) {
        perror("Failed to create odd thread");
        free(array);
        return EXIT_FAILURE;
    }

    // Wait for both threads to finish
    pthread_join(even_thread, NULL);
    pthread_join(odd_thread, NULL);

    // Output the results
    printf("Sum of even numbers: %d\n", evenData.sum);
    printf("Sum of odd numbers: %d\n", oddData.sum);

    // Free allocated memory
    free(array);
    
    return EXIT_SUCCESS;
}

/*

 ./q4
Enter the number of elements: 4
Enter the elements of the array:
2 4 6 
4 8 9
Sum of even numbers: 16
Sum of odd numbers: 0
~/Desktop/220905128/lab9/exercises$ ./q4
Enter the number of elements: 5
Enter the elements of the array:
1 8 2 9 5
Sum of even numbers: 10
Sum of odd numbers: 15

*/
