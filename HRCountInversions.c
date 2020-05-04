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

static char* readline(FILE *fpter);
static char** split_string(char*);

// Complete the countInversions function below.
long countInversions(int arr_count, int* arr) {
	long inversions = 0;
	for(int i = 0; i<arr_count; i++) {

	}
	return inversions;
}

#if 0
int main()
{
    //FILE* fptr = fopen(getenv("OUTPUT_PATH"), "w");
	FILE* fptr = fopen("C:\\Users\\bvenkata\\OneDrive\\eclipse-workspace\\HelloWorld\\src\\HRCountInversionsInput.txt", "r");

	char* t_endptr;
	char* t_str = readline(fptr);
	int t = strtol(t_str, &t_endptr, 10);

	if (t_endptr == t_str || *t_endptr != '\0') { exit(EXIT_FAILURE); }

	for (int t_itr = 0; t_itr < t; t_itr++) {
		char* n_endptr;
		char* n_str = readline(fptr);
		int n = strtol(n_str, &n_endptr, 10);

		if (n_endptr == n_str || *n_endptr != '\0') { exit(EXIT_FAILURE); }

		char** arr_temp = split_string(readline(fptr));

		int* arr = malloc(n * sizeof(int));

		for (int i = 0; i < n; i++) {
			char* arr_item_endptr;
			char* arr_item_str = *(arr_temp + i);
			int arr_item = strtol(arr_item_str, &arr_item_endptr, 10);

			if (arr_item_endptr == arr_item_str || *arr_item_endptr != '\0') { exit(EXIT_FAILURE); }

			*(arr + i) = arr_item;
		}

		int arr_count = n;

		long result = countInversions(arr_count, arr);

		printf("%ld\n", result);
	}

	fclose(fptr);

	return 0;
}
#endif

char* readline(FILE *fptr) {
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
