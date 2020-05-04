#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char* readline();

typedef struct hash_t {
    int data[26];
} HASH;

void put(HASH *h, char c) {
    int c_int = (int)c;
    int idx = c_int %26;
    h->data[idx]++;
}

int get(HASH *h, char c) {
    int c_int = (int)c;
    int idx = c_int %26;
    if(h->data[idx]==0)
        return 0;
    else
        return h->data[idx]--;
}

// Complete the twoStrings function below.

// Please either make the string static or allocate on the heap. For example,
// static char str[] = "hello world";
// return str;
//
// OR
//
// char* str = "hello world";
// return str;
//
char* twoStrings(char* s1, char* s2) {
    HASH my_hash;
    memset(&my_hash, 0, sizeof(HASH));

    for(int i = 0; i<strlen(s1); i++) {
        put(&my_hash, s1[i]);
    }
    char *str = malloc(strlen(s1)*sizeof(char));
    memset(str, 0, strlen(s1)*sizeof(char));

    int idx = 0;
    for(int i = 0; i<strlen(s2); i++) {
        if(get(&my_hash, s2[i]) != 0) {
            str[idx++] = s2[i];
        }
    }
    return strlen(str) ? "YES" : "NO";
}

#if 0
int main()
{
    /*FILE* fptr = fopen(getenv("OUTPUT_PATH"), "w");

    char* q_endptr;
    char* q_str = readline();
    int q = strtol(q_str, &q_endptr, 10);

    if (q_endptr == q_str || *q_endptr != '\0') { exit(EXIT_FAILURE); }

    for (int q_itr = 0; q_itr < q; q_itr++) {
        char* s1 = readline();

        char* s2 = readline();

        char* result = twoStrings(s1, s2);

        fprintf(fptr, "%s\n", result);
    }

    fclose(fptr);*/

	/* SAMPLE INPUT
	 4
		wouldyoulikefries
		abcabcabcabcabcabc
		hackerrankcommunity
		cdecdecdecde
		jackandjill
		wentupthehill
		writetoyourparents
		fghmqzldbc

		Expected Output
			NO
			YES
			YES
			NO
	 */

    char* result = twoStrings("writetoyourparents", "fghmqzldbc");//s1, s2);
    printf("%s\n", result);

    return 0;
}
#endif

char* readline() {
    size_t alloc_length = 1024;
    size_t data_length = 0;
    char* data = malloc(alloc_length);

    while (true) {
        char* cursor = data + data_length;
        char* line = fgets(cursor, alloc_length - data_length, stdin);

        if (!line) { break; }

        data_length += strlen(cursor);

        if (data_length < alloc_length - 1 || data[data_length - 1] == '\n') { break; }

        size_t new_length = alloc_length << 1;
        data = realloc(data, new_length);

        if (!data) { break; }

        alloc_length = new_length;
    }

    if (data[data_length - 1] == '\n') {
        data[data_length - 1] = '\0';
    }

    data = realloc(data, data_length);

    return data;
}
