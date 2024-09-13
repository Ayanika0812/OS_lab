//race.c
#include <pthread.h>
#include<stdio.h>
int counter = 0;
void * func(void * params)
{
counter++;
printf("Counter : %d\n ",counter);
}
void main()
{
pthread_t thread1, thread2;
pthread_create(&thread1, 0, func, 0);
printf(" after thread1 Counter : %d\n ",counter);
pthread_create(&thread2, 0, func, 0);
printf(" after thread2 Counter : %d\n ",counter);
pthread_join(thread1, 0 );
pthread_join(thread2, 0 );
}
