/*
 *  Source:     8005A1_proc.c
 *
 *  Program:    COMP8005 Assignment 1
 *  
 *  Functions:  main(int argc, char** argv)
 *              int randomGen()
 *              
 *  Date:       Jan 22, 2018
 *
 *  Designer:   Aing Ragunathan
 *
 *  Programmer: Aing Ragunathan
 *
 *  Notes:      This program tests the performance of processes.
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>
#include "primedecompose.h"

#define RESULT_FILE "RESULT_FILE_PROC"
#define MAX_FACTORS	1024
#define MECH_NUM 5
#define BUFSIZE 1024
#define STATIC_VAL "11111111111111"
#define CALC_VAL 1000000000 

int randomGen();

mpz_t dest[MAX_FACTORS]; 
 
/*
 *  Function:   main
 *  Date:       Jan 11, 2018
 *  Designer:   Aing Ragunathan
 *  Programmer: Aing Ragunathan
 *  Interface:  main(int argc, char** argv)
 *  Returns:    int
 *  Notes:      Tests the performance of multi-processes
 *
 */
int main(int argc, char **argv) 
{
        FILE *rfp;
        int childpid;
        struct timeval pstart, pstop;

        rfp = fopen(RESULT_FILE, "w");
        fclose(rfp);

        gettimeofday(&pstart, NULL);

        for(int i = 0; i < MECH_NUM; i++) {
                if((childpid = fork()) <= 0) {
                        mpz_t n;
                        int i, l, r;
                        struct timeval stop, start;
                        FILE *fp;
                        char buf[BUFSIZE];

                        //srand((unsigned) time(&t));
                        srand((unsigned) time(NULL) ^ (getpid()<<16));
                        r = randomGen();
                        snprintf(buf, sizeof(buf), "%d", r);
                        printf("num: %s\n", buf);

                        mpz_init_set_str(n, STATIC_VAL, 10);
                        //mpz_init_set_str(n, buf, 10);


                        //do caclulation and time it
                        gettimeofday(&start, NULL);
                        fp = fopen(RESULT_FILE, "a");
                        fprintf(fp, "PID: %d\t", getpid());
                        l = decompose(n, dest);
                        gettimeofday(&stop, NULL);

                        
                        fprintf(fp, "Time: %ld usec\n",(stop.tv_sec * 1000000 + stop.tv_usec) 
                                        - (start.tv_sec * 1000000 + start.tv_usec));
                        fprintf(fp, "Result: \n");

                        for(i=0; i < l; i++) 
                        {
                                gmp_fprintf(fp, "%s%Zd", i?" * ":"", dest[i]);
                                mpz_clear(dest[i]);
                        }

                        fprintf(fp, "\n\n");
                        
                        fclose(fp);
                        exit(1);
                 }
        }

        while(wait(&childpid) != -1);

        gettimeofday(&pstop, NULL);
        rfp = fopen(RESULT_FILE, "a");

        fprintf(rfp, "Total Time: %ld usec\n",(pstop.tv_sec * 1000000 + pstop.tv_usec) 
                                        - (pstart.tv_sec * 1000000 + pstart.tv_usec));
 
        return 0;
}


/*
 *  Function:   randomGen
 *  Date:       Jan 12, 2018
 *  Designer:   Aing Ragunathan
 *  Programmer: Aing Ragunathan 
 *  Interface:  int randomGen()
 *  Returns:    random integer
 * 
 */
int randomGen() {
        int num;
        num = rand() % CALC_VAL + CALC_VAL;
        num = rand();
        return num;
}

