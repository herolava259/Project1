#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define MAX_BUFF   100

void *consume(void *arg);
void *produce(void *arg);
int buf[MAX_BUFF], ffront, rear;
/* Here lock is used to create a critical section
 *  full and empty used to signaling events to other threads
 */

int counter = 0;
pthread_mutex_t lock;
pthread_cond_t nonempty  = PTHREAD_COND_INITIALIZER;
pthread_cond_t full  = PTHREAD_COND_INITIALIZER;
int main()
{
 pthread_t tid1,tid2;
 /* lock is a binary semaphore used to make a critical same as mutex
 * To make sure only one thread accessing at a time*/
 pthread_mutex_init(&lock,NULL);
 /* full is used to check queue is empty or not */
 //pthread_cond_init(&full,NULL);
 /* empty is used to check queue is full or not */
 //pthread_cond_init(&nonempty,NULL);
 pthread_create(&tid1, NULL, produce, NULL);
 pthread_create(&tid2, NULL, consume, NULL);
 pthread_join(tid1, NULL);
 pthread_join(tid2, NULL);
}
void *produce(void *arg)
{
 int i;
 for(i=0;i<10;i++)
 {
 /* Now this is same as is_queue_full()
 * before storing value in queue */
 pthread_mutex_lock(&lock);
 if(counter == MAX_BUFF){

    pthread_cond_wait(&full, &lock);
 }
 printf("produced item is %d\n", i);
 buf[(++rear)%MAX_BUFF] = i;
 counter++;
 srand(time(NULL));
 int i = rand()%100;

 if(counter ==100){
    pthread_cond_signal(&nonempty);
 }
 pthread_mutex_unlock(&lock);
 sleep(2);
 /* This is to inform consumer that data is available in queue */

 }
 pthread_cond_signal(&nonempty);
 return NULL;
}
void *consume(void *arg)
{
 int item, i;
 for(i = 0; i < 10; i++)
 {
 /* wait untill data is available in queue same as
 * is_queue_empty() */
 pthread_mutex_lock(&lock);
 if(counter == 0){

    pthread_cond_wait(&nonempty, &lock);
 }
 item=buf[(++ffront) % MAX_BUFF];
 printf("consumed item is %d\n", item);
 counter--;

 if(counter == 0){

    pthread_cond_signal(&full);
 }
 pthread_mutex_unlock(&lock);
 sleep(1);
 /* Notifynig consumer that data consumed and psition is available */

 }
 pthread_cond_signal(&full);
 return NULL;
}
