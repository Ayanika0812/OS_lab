#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_NUMBERS 100

typedef struct {
    int *numbers;
    int count;
    int result;
} SumData;

void* calculate_sum(void* param) {
    SumData* data = (SumData*)param;
    data->result = 0;

    for (int i = 0; i < data->count; i++) {
        data->result += data->numbers[i];
    }

    return NULL;
}

int main() {
    int numbers[MAX_NUMBERS];
    int count = 0;

    printf("Enter non-negative integers (negative number to stop):\n");

    while (count < MAX_NUMBERS) {
        int input;
        scanf("%d", &input);
        
        if (input < 0) {
            break; 
        }
        
        numbers[count++] = input;
    }

    if (count == 0) {
        printf("No non-negative integers were entered.\n");
        return EXIT_FAILURE;
    }

    SumData sumData = {numbers, count, 0};
    pthread_t thread;

    if (pthread_create(&thread, NULL, calculate_sum, &sumData) != 0) {
        perror("Failed to create thread");
        return EXIT_FAILURE;
    }

    pthread_join(thread, NULL);

    printf("The sum of the provided non-negative integers is: %d\n", sumData.result);

    return EXIT_SUCCESS;
}
/*

./q31
Enter non-negative integers (negative number to stop):
3
4
-1
The sum of the provided non-negative integers is: 7
*/
