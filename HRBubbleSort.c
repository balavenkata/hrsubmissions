#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <strings.h>

static char** split_string(char*);
static char* readline(FILE *fptr);

static void swap(long *a, long*b) {
	long temp = *a;
	*a = *b;
	*b = temp;
}

static void bubble_sort(int n, long *arr) {
	int last_unsorted = n-1;
	bool is_sorted = false;

	clock_t begin = clock();

	while(!is_sorted) {
		is_sorted = true;
		for(int i = 0; i<(n-1); i++) {
			if(arr[i] > arr[i+1]) {
				swap(&arr[i], &arr[i+1]);
				is_sorted = false;
			}
		}
		last_unsorted--;
	}

	clock_t end = clock();
	double time_spent = 0.0;
	time_spent += (double) (end-begin) / CLOCKS_PER_SEC;

	printf("Sorted array...\n");
	for(int i = 0; i<(n-1); i++) {
		printf("%ld ", arr[i]);
	}
	printf("\nSorted ascending...in %f seconds\n", time_spent);
}

#if 0
int main() {
	FILE* fptr = fopen("C:\\Users\\bvenkata\\OneDrive\\eclipse-workspace\\HelloWorld\\src\\HRBubbleSortInput.txt", "r");

	char** nr = split_string(readline(fptr));
	char* n_endptr;
	char* n_str = nr[0];
	int n = strtol(n_str, &n_endptr, 10);

	if (n_endptr == n_str || *n_endptr != '\0') { exit(EXIT_FAILURE); }

	char** arr_temp = split_string(readline(fptr));
	long* arr = malloc(n * sizeof(long));
	for (int i = 0; i < n; i++) {
		char* arr_item_endptr;
		char* arr_item_str = *(arr_temp + i);
		long arr_item = strtol(arr_item_str, &arr_item_endptr, 10);

		if (arr_item_endptr == arr_item_str || *arr_item_endptr != '\0') { exit(EXIT_FAILURE); }

		*(arr + i) = arr_item;
	}

	bubble_sort(n, arr);

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
