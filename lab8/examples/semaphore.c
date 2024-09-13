#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
sem_t semaphore;
void *func1( void * param )
{
printf( "Thread 1\n" );
sem_post( &semaphore );
}
void *func2( void * param )
{
sem_wait( &semaphore );
printf( "Thread 2\n" );
}
int main()
{
pthread_t threads[2];
sem_init( &semaphore, 0, 1 );  //&semaphore is ptr to semaphore. 0 is private to a threads[2] process. 1 is value initialized to the semaphore
pthread_create( &threads[0], 0, func1, 0 );
pthread_create( &threads[1], 0, func2, 0 );
pthread_join( threads[0], 0 );
pthread_join( threads[1], 0 );
sem_destroy( &semaphore );
}

/*

./a.out
Thread 1
Thread 2
*/
