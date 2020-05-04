#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* readline();

#if 0
int main()
{
    char* n_endptr;
    //char* n_str = readline();
    //int n = strtol(n_str, &n_endptr, 10);
    int n = 683; //10 1010 1011

    //if (n_endptr == n_str || *n_endptr != '\0') { exit(EXIT_FAILURE); }

    int bin[24];
    int index = 0;
    bool this_1;

    while(n !=0) {
    	this_1 = (n%2) ? 1 : 0;
    	bin[index++] = this_1;
    	n = n>>1;
    }

    int count = 0;
    int max = 0;
    for(int i=0; i< index; i++) {
    	if(!bin[i])
    		count = 0;
    	else
    		count++;

    	if(count > max)
    		max = count;
    }

    //printf("n: %d m %d j %d", n, m , j);
    printf("count of 1's %d", max);

    return 0;
}
#endif
