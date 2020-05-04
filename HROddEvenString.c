#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

static char* readline() {
    size_t alloc_length = 1024;
    size_t data_length = 0;
    char* data = malloc(alloc_length);

    while (1) {
        char* cursor = data + data_length;
        char* line = fgets(cursor, alloc_length - data_length, stdin);
        if (!line) {
            break;
        }
        data_length += strlen(cursor);
        if (data_length < alloc_length - 1 || data[data_length - 1] == '\n') {
            break;
        }
        alloc_length <<= 1;
        data = realloc(data, alloc_length);
        if (!line) {
            break;
        }
    }

    if (data[data_length - 1] == '\n') {
        data[data_length - 1] = '\0';

        data = realloc(data, data_length);
    } else {
        data = realloc(data, data_length + 1);

        data[data_length] = '\0';
    }

    return data;
}

void odd_even_strs(int t, char **strs) {
    char *str;
    for (int i = 0; i < t; i++) {
        str = *(strs + i);
        int len = strlen(str);
        char *odd = malloc(((len+1)/2 + 1) * sizeof(char));
        char *even = malloc(((len+1)/2 + 1) * sizeof(char));
        memset(odd, 0, (len+1)/2+1);
        memset(even, 0, (len+1)/2+1);
        for(int j=0; j<len; j++) {
            if(j%2 == 0) {
                //odd indexed
                odd[j/2] = str[j];
            } else {
                even[j/2] = str[j];
            }
        }
        odd[(len%2 == 0) ? (len/2) : (len+1)/2] = '\0';
        even[(len)/2] = '\0';

        printf("%s %s\n", odd, even);
    }
}

#if 0
int main() {

    /* Enter your code here. Read input from STDIN. Print output to STDOUT */
    char* t_str = readline();
    char* t_endptr;
    int t = strtol(t_str, &t_endptr, 10);

    char** strs = malloc(t * sizeof(char*));
    for (int i = 0; i < t; i++) {
        char* str = readline();
        *(strs + i) = str;
    }

    odd_even_strs(t, strs);

    return 0;
}
#endif
