#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char* readline(FILE *);
static char** split_string(char*);

typedef struct str_count {
	char str[100];
	int count;
}STR_COUNT;

typedef struct hash_t {
	STR_COUNT data[100][1000];
	int count_in_buckets[100];
} HASH;

static int hash_func(char *str) {
	int hash = 0;
	int len = strlen(str);

	for(int i = 0; i<len; i++)
		hash += (int)str[i];

	return hash;
}

HASH g_hash;

static void put(char *str) {
	//
	int hash = hash_func(str);
	int index = hash%100;
	STR_COUNT *row = g_hash.data[index];
	int count_in_bucket = g_hash.count_in_buckets[index];
	int i;
	for (i = 0; i< count_in_bucket; i++) {
		if(strcmp(row[i].str, str) ==0) {
			row[i].count++;
			break;
		}
	}

	if(i==count_in_bucket) {
		//not found...insert.
		STR_COUNT s;
		strcpy(s.str, str);
		s.count = 1;
		row[count_in_bucket] = s;
		g_hash.count_in_buckets[index]++;
	}
}

static int get(char *str) {
	int hash = hash_func(str);
	int index = hash%100;
	STR_COUNT *row = g_hash.data[index];
	int count_in_bucket = g_hash.count_in_buckets[index];

	/*if(count_in_bucket==1) {
		STR_COUNT s = row[0];
		s.count--;
		return 1;
	} else {*/
		for (int i = 0; i< count_in_bucket; i++) {
			if((strcmp(row[i].str, str) ==0) && row[i].count) {
				row[i].count--;
				return 1;
			}
		}
	//}

	return 0;
}

// Complete the checkMagazine function below.
void checkMagazine(int magazine_count, char** magazine, int note_count, char** note) {
	int i;

	memset(&g_hash, 0, sizeof(HASH));

	for(i=0; i < magazine_count; i++) {
		//printf("magazine[%d]: %s\n", i, magazine[i]);
		put(magazine[i]);
	}

	for(i=0; i < note_count; i++) {
		//printf("note[%d]: %s\n", i, note[i]);
		int j = get(note[i]);
		if(j==0) {
			printf("No");
			break;
		}
	}

	if(i==note_count) {
		printf("Yes");
	}
}

#if 0
int main()
{
	FILE *fptr = fopen("C:\\Users\\bvenkata\\OneDrive\\eclipse-workspace\\HelloWorld\\src\\HRHashtableInput.txt", "r");
	if(fptr==0)
		printf("not opening file");

    char** mn = split_string(readline(fptr));

    char* m_endptr;
    char* m_str = mn[0];
    int m = strtol(m_str, &m_endptr, 10);

    if (m_endptr == m_str || *m_endptr != '\0') { exit(EXIT_FAILURE); }

    char* n_endptr;
    char* n_str = mn[1];
    int n = strtol(n_str, &n_endptr, 10);

    if (n_endptr == n_str || *n_endptr != '\0') { exit(EXIT_FAILURE); }

    char** magazine_temp = split_string(readline(fptr));

    char** magazine = malloc(m * sizeof(char*));

    for (int i = 0; i < m; i++) {
        char* magazine_item = *(magazine_temp + i);

        *(magazine + i) = magazine_item;
    }

    int magazine_count = m;

    char** note_temp = split_string(readline(fptr));

    char** note = malloc(n * sizeof(char*));

    for (int i = 0; i < n; i++) {
        char* note_item = *(note_temp + i);

        *(note + i) = note_item;
    }

    int note_count = n;

    checkMagazine(magazine_count, magazine, note_count, note);

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
