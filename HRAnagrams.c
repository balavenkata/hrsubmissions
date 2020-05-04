#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static char* readline(FILE *fptr);
static unsigned long HASH_SIZE = 9999991;//a 4 digit prime closest to 5K
static int primes[26] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97,101};//,103,107,109,113,127,131};

typedef struct str_len_count_t {
	int length;
	int count;
	struct str_len_count_t *next;
}STR_LEN_CNT;

typedef struct hash_t {
	STR_LEN_CNT **data;
} HASH;

static int cmp_char(void *c1, void *c2) {
    char *char1 = (char*)c1;
    char *char2 = (char*)c2;
    return (*char1 - *char2);
}

static unsigned long hash_func(char *s) {
    int len = strlen(s);
    unsigned long hash = 1;

    qsort(s, len, sizeof(char), cmp_char);

    for(int i = 0; i <len; i++) {
        int k = s[i];
    	hash *= primes[k-'a'];
    }

    return hash;
}

static void put(HASH *h, char *s, int s_len) {
    unsigned long hash = hash_func(s);
    unsigned long index = hash % HASH_SIZE;

    STR_LEN_CNT *items_in_bucket = h->data[index];

    if(items_in_bucket==0) {
    	STR_LEN_CNT *s = malloc(sizeof(STR_LEN_CNT));
    	s->count = 1;
    	s->length = s_len;
    	s->next = NULL;

    	items_in_bucket = s;
    	h->data[index] = items_in_bucket;
    } else {
    	bool found = false;
    	STR_LEN_CNT *last;
    	while(items_in_bucket && !found) {
    		if(items_in_bucket->length == s_len) {
    			found = true;
    			items_in_bucket->count++;
    		}
    		last = items_in_bucket;
    		items_in_bucket = items_in_bucket->next;
    	}
    	if(!found) {
    		STR_LEN_CNT *s = malloc(sizeof(STR_LEN_CNT));
			s->count = 1;
			s->length = s_len;
			s->next = NULL;
    		last->next = s;
    	}
    }
}




// Complete the sherlockAndAnagrams function below.
static int sherlockAndAnagrams(char* s) {
    int len = strlen(s);

    HASH *pHash = malloc(sizeof(HASH));
    pHash->data = malloc(HASH_SIZE*sizeof(STR_LEN_CNT*));
    memset(pHash->data, 0, HASH_SIZE*sizeof(STR_LEN_CNT*));

    for(int i = 0; i<len; i++) {
        //find all char's of size (i+1)
        for(int j=0; j<(len-i); j++) {
            char *str1 = malloc((i+2)*sizeof(char));
            memset(str1, 0, (i+2)*sizeof(char));
            memcpy(str1, s+j, (i+1));
            str1[i+1] = '\0';

            put(pHash, str1, (i+1));

            free(str1);
        }
    }

    //count
	int count = 0;
	for(unsigned long j = 0; j< HASH_SIZE; j++) {
		STR_LEN_CNT *items_in_bucket = pHash->data[j];
		while(items_in_bucket) {
			if(items_in_bucket->count >1) {
				count += (items_in_bucket->count * (items_in_bucket->count-1)/2);
			}
			items_in_bucket = items_in_bucket->next;
		}
	}

	free(pHash->data);
	free(pHash);

    return count;
}

#if 0
int main()
{
	//HRAnagramsInput.large.txt

    FILE* fptr = fopen("C:\\Users\\bvenkata\\OneDrive\\eclipse-workspace\\HelloWorld\\src\\HRAnagramsInput.large.txt", "r");

    char* q_endptr;
    char* q_str = readline(fptr);
    int q = strtol(q_str, &q_endptr, 10);

    if (q_endptr == q_str || *q_endptr != '\0') { exit(EXIT_FAILURE); }

    for (int q_itr = 0; q_itr < q; q_itr++) {
        char* s = readline(fptr);

        clock_t begin = clock();
        int result = sherlockAndAnagrams(s);
        printf("%d\n", result);
        clock_t end = clock();
		double time_spent = 0.0;
		time_spent += (double) (end-begin) / CLOCKS_PER_SEC;
		//printf("time taken to read key value %f seconds\n", time_spent);
    }

    fclose(fptr);

    return 0;
}
#endif
static char* readline(FILE *fptr) {
    size_t alloc_length = 1024;
    size_t data_length = 0;
    char* data = malloc(alloc_length);

    while (true) {
        char* cursor = data + data_length;
        char* line = fgets(cursor, alloc_length - data_length, fptr ? fptr : stdin);

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
