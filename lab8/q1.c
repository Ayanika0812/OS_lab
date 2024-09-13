#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>  

#define BUFFER_SIZE 5
#define MAX_DIFF 10

int buf[BUFFER_SIZE], f = 0, r = 0;
sem_t mutex, full, empty, limit_sem;
int produced_count = 0, consumed_count = 0;

void *produce(void *arg) {
    int i;
    for (i = 0; i < 20; i++) {  
        sem_wait(&empty);        
        sem_wait(&mutex);        // Lock 

        
        while ((produced_count - consumed_count) >= MAX_DIFF) {
            sem_post(&mutex);    
            sem_wait(&limit_sem); 
            sem_wait(&mutex);    
        }

        // Produce the item
        printf("Produced item: %d\n", i);
        buf[r] = i;
        r = (r + 1) % BUFFER_SIZE;  
        produced_count++;

        sem_post(&mutex);        
        sem_post(&full);       
        
        sleep(1);  
    }
    return NULL;
}

void *consume(void *arg) {
    int item, i;
    for (i = 0; i < 10; i++) { 
        sem_wait(&full);         
        sem_wait(&mutex);        

        // Consume the item
        item = buf[f];
        f = (f + 1) % BUFFER_SIZE;  // Update front index
        consumed_count++;
        printf("Consumed item: %d\n", item);

        // Allow the producer to produce more if the difference is within the limit
        if ((produced_count - consumed_count) < MAX_DIFF) {
            sem_post(&limit_sem);  // Signal the producer to produce more
        }

        sem_post(&mutex);        // Unlock access to shared data
        sem_post(&empty);        // Signal that there is an empty slot in the buffer

        sleep(1); 
    }
    return NULL;
}

int main() {
    pthread_t tid1, tid2;

    sem_init(&mutex, 0, 1);
    sem_init(&full, 0, 0);  
    sem_init(&empty, 0, BUFFER_SIZE);  
    sem_init(&limit_sem, 0, 0);  

    pthread_create(&tid1, NULL, produce, NULL);
    pthread_create(&tid2, NULL, consume, NULL);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    sem_destroy(&mutex);
    sem_destroy(&full);
    sem_destroy(&empty);
    sem_destroy(&limit_sem);

    return 0;
}

/*
./a.out
Produced item: 0
Consumed item: 0
Produced item: 1
Consumed item: 1
Produced item: 2
Consumed item: 2
Produced item: 3
Consumed item: 3
Produced item: 4
Consumed item: 4
Produced item: 5
Consumed item: 5
Produced item: 6
Consumed item: 6
Produced item: 7
Consumed item: 7
Produced item: 8
Consumed item: 8
Produced item: 9
Consumed item: 9
Produced item: 10
Produced item: 11
Produced item: 12
Produced item: 13
Produced item: 14
*/

