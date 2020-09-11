#include<pthread.h>
#include<stdio.h>
#include<string.h>
#include<random>
#define MAX 50
#include<unistd.h>
#include<stdlib.h>
#include<time.h>
#define BUFLEN 50
#define MAX_COUNT 15
#define NUMTHREAD 4

void* writer(void *data);
void* reader(void *data);

char buf_data[5][40]={"Hello, ", "Hi, ", "Good morning, ", "Good Afternoon, ", "Good Evening, "};

char buffer[BUFLEN] = "No data";
char source[BUFLEN];
int readcount = 0;
int n = 0;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t read_mutex;
pthread_mutex_t wrt_mutex;


int thread_id[NUMTHREAD] = {1,2,3,4};



int main(){

    int i;

    pthread_t thread[NUMTHREAD];

    pthread_mutex_init(&read_mutex, NULL);
    pthread_mutex_init(&wrt_mutex, NULL);


    //pthread_create(&thread[2], NULL, (void*)watch, &thread_id[2]);

    pthread_create(&thread[0], NULL, &writer,(void *) &(thread_id[0]));
    pthread_create(&thread[1], NULL, &writer,(void *) &(thread_id[1]));
    pthread_create(&thread[2], NULL, &reader,(void *) &(thread_id[2]));
    pthread_create(&thread[3], NULL, &reader,(void *) &(thread_id[3]));
    for(i = 0; i < NUMTHREAD; i++){

        pthread_join(thread[i], NULL);

    }

    return 0;
}



void* writer(void *data){

     int *id = (int *)data;
     int j=0;
     char str[30] = "I'm writer";
     while(j <10){

           pthread_mutex_lock(&wrt_mutex);

           printf("Writer %d is processing...\n", *id);

           pthread_cond_wait(&cond, &wrt_mutex);

           srand(time(NULL));
           int i = rand()%5;
           strcpy(buffer, buf_data[i]);
           strcat(buffer,str);
           n = *id;



          j++;


          /*if(j < MAX - 2){

             if(rand()%100 < 30){

                sleep(rand()%3);
             }*/
          pthread_mutex_unlock(&wrt_mutex);

          sleep(1);

          }

     }





void* reader(void* data){


  int* id = (int *)data;
  int i =0;
  while(i < 10){
     pthread_mutex_lock(&read_mutex);
     readcount++;
     if(readcount==1){
        pthread_mutex_lock(&wrt_mutex);
        pthread_cond_signal(&cond);

        pthread_cond_wait(&cond, &wrt_mutex);
     }
     pthread_mutex_unlock(&read_mutex);

     printf("Reader %d is reading: %s %d\n", *id, buffer, n);
     //fflush(stdout);
     i++;
     srand(time(NULL));
     sleep(rand()%5);
     pthread_mutex_lock(&read_mutex);
     readcount--;
     if(readcount==0){

        pthread_mutex_unlock(&wrt_mutex);
     }
     pthread_mutex_unlock(&read_mutex);


     /*if(i < (MAX - 2))
     {

        if (rand()%100 >= 30){

           sleep(rand()%3);
        }

     }*/

    }




}
