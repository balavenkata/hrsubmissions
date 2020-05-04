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

static char* ltrim(char*);
static char* rtrim(char*);
static char** split_string(char*);

static char* readline(FILE *fptr);
static unsigned long HASH_SIZE = 9999991;//a 4 digit prime closest to 5K

typedef struct num_len_count_t {
	unsigned long number;
	unsigned long long count;
	struct str_len_count_t *next;
}NUM_LEN_CNT;

typedef struct hash_t {
	NUM_LEN_CNT **data;
} HASH;

static unsigned long hash_func(unsigned long x) {
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    return x;
}

static void put(HASH *h, unsigned long n, unsigned long long count) {
    unsigned long hash = hash_func(n);
    unsigned long index = hash % HASH_SIZE;

    NUM_LEN_CNT *items_in_bucket = h->data[index];

    if(items_in_bucket==0) {
    	NUM_LEN_CNT *s = malloc(sizeof(NUM_LEN_CNT));
    	s->number = n;
    	s->count = count;
    	s->next = NULL;

    	items_in_bucket = s;
    	h->data[index] = items_in_bucket;
    } else {
    	bool found = false;
    	NUM_LEN_CNT *last;
    	while(items_in_bucket && !found) {
    		if(items_in_bucket->number == n) {
    			found = true;
    			items_in_bucket->count = count;
    		}
    		last = items_in_bucket;
    		items_in_bucket = items_in_bucket->next;
    	}
    	if(!found) {
    		NUM_LEN_CNT *s = malloc(sizeof(NUM_LEN_CNT));
    		s->number = n;
    		s->count = count;
    		s->next = NULL;
    		last->next = s;
    	}
    }
}

static unsigned long long get(HASH *h, unsigned long n) {
	unsigned long hash = hash_func(n);
	unsigned long index = hash % HASH_SIZE;

	NUM_LEN_CNT *items_in_bucket = h->data[index];
	while(items_in_bucket) {
		if(items_in_bucket->number == n) {
			return items_in_bucket->count;
		}
		items_in_bucket = items_in_bucket->next;
	}

	return 0;
}


// Complete the countTriplets function below.
unsigned long long countTriplets(int arr_count, long* arr, long r) {
    HASH *j_hash = malloc(sizeof(HASH));
    j_hash->data = malloc(HASH_SIZE*sizeof(NUM_LEN_CNT*));
    memset(j_hash->data, 0, HASH_SIZE*sizeof(NUM_LEN_CNT*));

    HASH *k_hash = malloc(sizeof(HASH));
    k_hash->data = malloc(HASH_SIZE*sizeof(NUM_LEN_CNT*));
	memset(k_hash->data, 0, HASH_SIZE*sizeof(NUM_LEN_CNT*));

    unsigned long long count = 0;
    unsigned long long k_hash_n_count, j_hash_n_count, k_hash_nr_count, j_hash_nr_count;

	for(int i = 0; i<arr_count; i++) {
		long n = arr[i];
		k_hash_n_count = get(k_hash, n);
		if(k_hash_n_count > 0)
			count += k_hash_n_count;

		j_hash_n_count = get(j_hash, n);
		if(j_hash_n_count > 0) {
			k_hash_nr_count = get(k_hash, n*r);
			put(k_hash, n*r, (k_hash_nr_count+j_hash_n_count));
		}

		j_hash_nr_count = get(j_hash, n*r);
		put(j_hash, n*r, (j_hash_nr_count+1));
	}

	free(j_hash->data);
	free(j_hash);
	free(k_hash->data);
	free(k_hash);

    return count;
}

#if 0
int main()
{
	//HRCountTripletsInput.txt
    FILE* fptr = fopen("C:\\Users\\bvenkata\\OneDrive\\eclipse-workspace\\HelloWorld\\src\\HRCountTripletsInput.txt", "r");

    char** nr = split_string(rtrim(readline(fptr)));

    char* n_endptr;
    char* n_str = nr[0];
    int n = strtol(n_str, &n_endptr, 10);

    if (n_endptr == n_str || *n_endptr != '\0') { exit(EXIT_FAILURE); }

    char* r_endptr;
    char* r_str = nr[1];
    long r = strtol(r_str, &r_endptr, 10);

    if (r_endptr == r_str || *r_endptr != '\0') { exit(EXIT_FAILURE); }

    char** arr_temp = split_string(rtrim(readline(fptr)));

    long* arr = malloc(n * sizeof(long));

    for (int i = 0; i < n; i++) {
        char* arr_item_endptr;
        char* arr_item_str = *(arr_temp + i);
        long arr_item = strtol(arr_item_str, &arr_item_endptr, 10);

        if (arr_item_endptr == arr_item_str || *arr_item_endptr != '\0') { exit(EXIT_FAILURE); }

        *(arr + i) = arr_item;
    }

    int arr_count = n;

    clock_t begin = clock();
    long long ans = countTriplets(arr_count, arr, r);
    printf("%lld\n", ans);
    clock_t end = clock();
    double time_spent = 0.0;
    time_spent += (double) (end-begin) / CLOCKS_PER_SEC;
    printf("time taken to read key value %f seconds\n", time_spent);

    //fprintf(fptr, "%ld\n", ans);

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

        if (!line) {
            break;
        }

        data_length += strlen(cursor);

        if (data_length < alloc_length - 1 || data[data_length - 1] == '\n') {
            break;
        }

        alloc_length <<= 1;

        data = realloc(data, alloc_length);

        if (!data) {
            data = '\0';

            break;
        }
    }

    if (data[data_length - 1] == '\n') {
        data[data_length - 1] = '\0';

        data = realloc(data, data_length);

        if (!data) {
            data = '\0';
        }
    } else {
        data = realloc(data, data_length + 1);

        if (!data) {
            data = '\0';
        } else {
            data[data_length] = '\0';
        }
    }

    return data;
}

char* ltrim(char* str) {
    if (!str) {
        return '\0';
    }

    if (!*str) {
        return str;
    }

    while (*str != '\0' && isspace(*str)) {
        str++;
    }

    return str;
}

char* rtrim(char* str) {
    if (!str) {
        return '\0';
    }

    if (!*str) {
        return str;
    }

    char* end = str + strlen(str) - 1;

    while (end >= str && isspace(*end)) {
        end--;
    }

    *(end + 1) = '\0';

    return str;
}

char** split_string(char* str) {
    char** splits = NULL;
    char* token = strtok(str, " ");

    int spaces = 0;

    while (token) {
        splits = realloc(splits, sizeof(char*) * ++spaces);

        if (!splits) {
            return splits;
        }

        splits[spaces - 1] = token;

        token = strtok(NULL, " ");
    }

    return splits;
}
