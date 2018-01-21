#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include "primedecompose.h"

#define RESULT_FILE "RESULT_FILE_PROC"
#define MAX_FACTORS	1024
#define MECH_NUM 5
#define CALC_VAL "11111111111111"

mpz_t dest[MAX_FACTORS]; 
 
int main(int argc, char **argv) 
{
        FILE *rfp;
        int childpid;

        rfp = fopen(RESULT_FILE, "w");
        fclose(rfp);
        
        for(int i = 0; i < MECH_NUM; i++) {
                if((childpid = fork()) <= 0) {
                        mpz_t n;
                        int i, l;
                        struct timeval stop, start;
                        FILE *fp;

                        mpz_init_set_str(n, CALC_VAL, 10);
                        
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

        return EXIT_SUCCESS;
}

