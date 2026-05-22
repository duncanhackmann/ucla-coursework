//
//  main.c
//  finalStudy
//
//  Created by Duncan Hackmann on 12/9/23.
//

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <omp.h>
#define ENTRIES 50
#define TOTAL 1000


int accum[ENTRIES];
omp_lock_t array_locks[ENTRIES];
int sources[TOTAL];

int main( int argc, const char* argv[] )
{
    int i,x;
    
#pragma omp parallel
    {
        
//set each index of sources to value of its index
#pragma omp for
        for (i=0;i<TOTAL;i++){
            sources[i]=i;
        }
        
//calls function init_lock and passes &array_locks[i] for each  value of i
//sets each value of accum[] to 0
#pragma omp for
        for(i=0;i<ENTRIES; i++){
            omp_init_lock(&array_locks[i]);
            accum[i] = 0;
        }
        
//each loop creates private x and sets it to sources[i] % 50
//uses x which is different each iteration of loop and calls function set_lock and passes &array_locks[x] for each  value of x
//if sources[i] = 0, increment accum[x] by 1 and calls function unset_lock and passes &array_locks[x]
#pragma omp for private(x)
        for(i=0;i<TOTAL;i++){
            x = sources[i]%ENTRIES;
            omp_set_lock(&array_locks[x]);
            if (sources[i]==0)
            {
                accum[x]++;
                omp_unset_lock(&array_locks[x]);
            }
        }
        
//calls function destroy_lock and passes &array_locks[i]
#pragma omp for
        for(i=0;i<ENTRIES; i++)
            omp_destroy_lock(&array_locks[i]);
        printf("%d\n",accum[0]);
    }
}

