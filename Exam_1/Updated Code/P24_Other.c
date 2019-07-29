/**************************************
* Code provided in ECEN5623:
* - sumdigits/sumdigits.c
* 
* Compile code using make file or the gcc command below:
* $ sudo gcc P24_Other.c -o b.out -lpthread -Wall
* and run by typing $./b.out
***************************************/


#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <syslog.h>
#include <sys/time.h>

#define COUNT  100

typedef struct
{
    int threadIdx;
} threadParams_t;


// POSIX thread declarations and scheduling attributes
pthread_t threads[3];
threadParams_t threadParams[3];

// Thread specific globals
int gsum[3]={0, 100, 200};

void *sumThread(void *threadp)
{
    int i, idx;
    threadParams_t *threadParams = (threadParams_t *)threadp;

    idx = threadParams->threadIdx;

    for(i=1; i<COUNT; i++)
    {
        gsum[idx] = gsum[idx] + (i + (idx*COUNT));
        syslog(LOG_CRIT,"Increment %d for thread idx=%d, gsum=%d\n", i, idx, gsum[idx]);
		//printf("Increment %d for thread idx=%d, gsum=%d\n", i, idx, gsum[idx]);
    }
}

int main (int argc, char *argv[])
{
   int rc, testsum=0; int i=0;

   for(i=0; i<3; i++)
   {
      threadParams[i].threadIdx=i;
      pthread_create(&threads[i], (void *)0, sumThread, (void *)&(threadParams[i]));
   }

   for(i=0; i<3; i++)
     pthread_join(threads[i], NULL);

   syslog(LOG_CRIT, "TEST COMPLETE: gsum[0]=%d, gsum[1]=%d, gsum[2]=%d, gsumall=%d\n", 
          gsum[0], gsum[1], gsum[2], (gsum[0]+gsum[1]+gsum[2]));
  // printf("TEST COMPLETE: gsum[0]=%d, gsum[1]=%d, gsum[2]=%d, gsumall=%d\n", 
     //     gsum[0], gsum[1], gsum[2], (gsum[0]+gsum[1]+gsum[2]));

   // Verfiy with single thread version and (n*(n+1))/2
   for(i=1; i<(3*COUNT); i++)
       testsum = testsum + i;

   syslog(LOG_CRIT,"TEST COMPLETE: testsum=%d, [n[n+1]]/2=%d\n", testsum, ((3*COUNT-1)*(3*COUNT))/2);
   //printf("TEST COMPLETE: testsum=%d, [n[n+1]]/2=%d\n", testsum, ((3*COUNT-1)*(3*COUNT))/2);
}