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

static char* readline(FILE *);
static char** split_string(char*);

typedef struct str_count {
	char *name;
	int phone_num;
}STR_COUNT;

typedef struct hash_t {
	STR_COUNT data[1000][1000];
	int count_in_buckets[1000];
} HASH;

static int hash_func(char *str) {
	int hash = 997;
	int len = strlen(str);

	for(int i = 0; i<len; i++)
		hash += ((int)str[i] << i) + 997; //add the largest 3 digit prime
		//hash += (hash<<i) + (int)str[i];

	//hash *= rand

	return hash;
}

unsigned long djb2_hash_func(unsigned char *str)
{
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

HASH g_hash;

static void put(char *str, int phone_num) {
	//
	int hash = hash_func(str);
	int index = hash%1000;
	STR_COUNT *row = g_hash.data[index];
	int count_in_bucket = g_hash.count_in_buckets[index];
	int i;
	for (i = 0; i< count_in_bucket; i++) {
		if(strcmp(row[i].name, str) ==0) {
			row[i].phone_num = phone_num;
			break;
		}
	}

	if(i >= 1000) {
		printf("no space");
	}

	if(i==count_in_bucket && i<1000) {
		//not found...insert.
		STR_COUNT s;
		s.name = malloc(strlen(str)*sizeof(char));
		strcpy(s.name, str);
		s.phone_num = phone_num;
		row[count_in_bucket] = s;
		g_hash.count_in_buckets[index]++;
	}
}

static int get(char *str) {
	int hash = hash_func(str);
	int index = hash%1000;
	STR_COUNT *row = g_hash.data[index];
	int count_in_bucket = g_hash.count_in_buckets[index];

	for (int i = 0; i< count_in_bucket; i++) {
		if((strcmp(row[i].name, str) ==0)) {
			return row[i].phone_num;
		}
	}

	return -1;
}

// Complete the phoneBook function below.
void phoneBook(int n, char **keys, int*values, int qry_count, char** queries) {
	int i;

	memset(&g_hash, 0, sizeof(HASH));

	for(i=0; i < n; i++) {
		put(keys[i], values[i]);
	}

	for(i=0; i < qry_count; i++) {
		int j = get(queries[i]);
		if(j<=0) {
			printf("Not found\n");
		} else {
			printf("%s=%d\n",queries[i], j);
		}
	}
}

#if 0
int main()
{
	//FILE *fptr = fopen("C:\\Users\\bvenkata\\OneDrive\\eclipse-workspace\\HelloWorld\\src\\HRDictPhoneBookInput.large.txt", "r");
	FILE *fptr = fopen("C:\\Users\\bvenkata\\OneDrive\\eclipse-workspace\\HelloWorld\\src\\HRDictPhoneBookInput.2.txt", "r");
	if(fptr==0)
		printf("not opening file");

	//fptr = 0;
	memset(&g_hash, 0, sizeof(HASH));
    char* n_endptr;
    char* n_str = readline(fptr);
    int n = strtol(n_str, &n_endptr, 10);

    if (n_endptr == n_str || *n_endptr != '\0') { exit(EXIT_FAILURE); }

    char** keys = malloc(n * sizeof(char*));
    int* values = malloc(n * sizeof(int));

    clock_t begin = clock();
    for (int i = 0; i < n; i++) {
    	char** key_value_temp = split_string(readline(fptr));
        char* key = key_value_temp[0];
        char* value_str = key_value_temp[1];
        char* value_endptr;
        int value = strtol(value_str, &value_endptr, 10);

        *(keys + i) = key;
        *(values + i) = value;
    }
    clock_t end = clock();
	double time_spent = 0.0;
	time_spent += (double) (end-begin) / CLOCKS_PER_SEC;
	printf("time taken to read key value %f seconds\n", time_spent);
	begin = end;

    int max_qry = pow(10,5);//10^5;
    char** queries = malloc(max_qry * sizeof(char*));
    int idx = 0;
    while(true) {
    	char *line = readline(fptr);
    	//printf("line is %s\n", line);
    	if(!line)
    		break;
    	*(queries+idx) = line;
    	idx++;
    }

    end = clock();
    time_spent = 0.0;
   	time_spent += (double) (end-begin) / CLOCKS_PER_SEC;
   	printf("time taken to read queries %f seconds\n", time_spent);

    phoneBook(n, keys, values, idx, queries);

    end = clock();
	time_spent = 0.0;
	time_spent += (double) (end-begin) / CLOCKS_PER_SEC;
	printf("time taken to cal %f seconds\n", time_spent);

    return 0;
}
#endif

static char* readline(FILE *fptr) {
    size_t alloc_length = 1024;
    size_t data_length = 0;
    char* data = malloc(alloc_length);

    char *line;
    while (true) {
        char* cursor = data + data_length;
        line = fgets(cursor, alloc_length - data_length, fptr ? fptr : stdin);

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

    if(!line)
    	return NULL;

    if (data[data_length - 1] == '\n') {
        data[data_length - 1] = '\0';

        data = realloc(data, data_length);
    } else {
        data = realloc(data, data_length + 1);

        data[data_length] = '\0';
    }

    return data;
}

static char** split_string(char* str) {
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
