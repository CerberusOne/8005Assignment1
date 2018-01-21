#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include "primedecompose.h"

//gcc -Wall -o pdec primedec.c primedecompose.c -lgmp

#define MAX_FACTORS	1024
#define MECH_NUM 5
#define CALC_VAL "11111111111111" //14 1's
//#define CALC_VAL "12312312313"

void* work(void*);
 
pthread_mutex_t fileLock = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char **argv) 
{
        FILE *fp;

        pthread_t thread1, thread2, thread3, thread4, thread5;

        fp = fopen("RESULT_FILE_THREADS", "w");
        
        pthread_create(&thread1, NULL, work, (void*) fp);
        pthread_create(&thread2, NULL, work, (void*) fp);
        pthread_create(&thread3, NULL, work, (void*) fp);
        pthread_create(&thread4, NULL, work, (void*) fp);
        pthread_create(&thread5, NULL, work, (void*) fp);

        pthread_join(thread1, NULL);
        pthread_join(thread2, NULL);
        pthread_join(thread3, NULL);
        pthread_join(thread4, NULL);
        pthread_join(thread5, NULL);

        return 0;
}

void* work(void* fp) {
        mpz_t dest[MAX_FACTORS];
        mpz_t n;
        int i, l;
        struct timeval stop, start;
        
        mpz_init_set_str(n, CALC_VAL, 10);
        
        //do caclulation and time it
        gettimeofday(&start, NULL);
        l = decompose(n, dest);
        gettimeofday(&stop, NULL);

        pthread_mutex_lock(&fileLock);
        //print results to file
        //printf("PID: %d\t", getpid());
        fprintf(fp, "PID: %d\t", getpid());
        
        //printf("time: %ld usec\n", (stop.tv_sec * 1000000 + stop.tv_usec) 
          //              - (start.tv_sec * 1000000 + start.tv_usec));
        fprintf(fp, "Time: %ld usec\n",(stop.tv_sec * 1000000 + stop.tv_usec) 
                        - (start.tv_sec * 1000000 + start.tv_usec));

        //printf("Result:\n");
        fprintf(fp, "Result: \n");

        for(i=0; i < l; i++) 
        {
                //gmp_printf("%s%Zd", i?" * ":"", dest[i]);
                gmp_fprintf(fp, "%s%Zd", i?" * ":"", dest[i]);
                mpz_clear(dest[i]);
        }

        //printf("\n\n");
        fprintf(fp, "\n\n");
        pthread_mutex_unlock(&fileLock);

        return NULL;
}

