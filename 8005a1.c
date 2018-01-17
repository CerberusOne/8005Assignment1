//Assignment 1
//compile with 
//  gcc -Wall -o 8005a1 8005a1.c primedecompose.c -l gmp
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <gmp.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "primedecompose.h"

#define FILENAME "a1_value"
#define RESULT_FILENAME "result_value"
#define BUFSIZE 1024
#define MAX_FACTOR 1024
#define CALC_VALUE "1"

mpz_t dest[MAX_FACTOR]; 

int main (int argc, char** argv) {
        int mechNum = 1;
        int childpid;
        int pidList[mechNum];

        //loop mechNum of times 
        for(int i = 0; i < mechNum; i++) {
                //create a mech
                if((childpid = fork()) <= 0) {
                        int fd;
                        int bytesRead; 
                        //bytesWrote, 
                        int resultNum;
                        struct timeval stop, start;
                        char calcVal[BUFSIZE];
                        mpz_t n;
                        char buf[BUFSIZE];
                        //result[BUFSIZE];
/*
                        //open file for calculation values
                        if((fd = open(FILENAME, O_RDWR | O_CREAT, 0)) < 0) {
                                perror("opening file");
                                exit(1);
                        }

                        //read value from file
                        if((bytesRead = read(fd, buf, BUFSIZE)) < 0) {
                                perror("writing to file");
                                exit(1);
                        }
                        
                        sprintf(calcVal, "%s", buf);        //only get number
                        printf("Value to calculate %s\n", calcVal);
                        calcVal[strlen(calcVal)-1] = 0;     //remove new line char
                        close(fd);
*/
                        //initialize mpz struct with value as second arg
//                        mpz_init_set_str(n, buf, 10);
                        mpz_init_set_str(n, CALC_VALUE, 10);
                        
                        //get startTime
                        gettimeofday(&start, NULL);
                        //run calculation
                        resultNum = decompose(n, dest);
                        //get endTime
                        gettimeofday(&stop, NULL);
                        //get the results
                        printf("resultNum = %d\n", resultNum);
                        for(int j = 0; j < resultNum; j++) {
                                //gmp_snprintf(result, BUFSIZE, "%Zd\n", dest);
                                gmp_printf("%s%Zd", i?" * ":"", dest[i]);
                                mpz_clear(dest[i]);
                        }
                        printf("\n");


                        //printf("results: %s\n", result);
  
                        printf("time: %lu microseconds\n", stop.tv_usec - start.tv_usec);
/*                        
                        //open file for calculation values
                        if((fd = open(RESULT_FILENAME, O_RDWR | O_CREAT, 0)) < 0) {
                                perror("opening file");
                                exit(1);
                        }

                        


                        //read value from file
                        if((bytesWrote = write(fd, buf, BUFSIZE)) < 0) {
                                perror("writing to file");
                                exit(1);
                        }
*/
                        //mutex lock(total timem)
                        
                        //append result to result file 
                        //mutex unlock(total timer)

                        //print mech number and timea
                        exit(1);
                } else {
                        pidList[i] = childpid;
                }
        }

        //wait for all mechs to finish 
         
        for(int i = 0; i < (sizeof(pidList)/sizeof(pidList[0])); i++) {
                if(wait(&pidList[i]) < 0) {
                        printf("%d already closed\n", pidList[i]);
                } else {
                        printf("%d closed\n", pidList[i]);
                }
        }
        printf("all done\n");
        //close all mechs
        //print out results 
        //exit

        return 0;
}
