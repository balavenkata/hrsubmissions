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

static void merge_sort_fn(int *arr, int *temp, int leftStart, int rightEnd);
static void merge_halves(int *arr, int *temp, int leftStart, int rightEnd);

int inversions = 0;

// Complete the countInversions function below.
void merge_sort(int arr_count, int* arr) {
	int *temp = malloc(arr_count*sizeof(int));
	merge_sort_fn(arr, temp, 0, arr_count-1);
}

void merge_sort_fn(int *arr, int *temp, int leftStart, int rightEnd) {
	if(leftStart >= rightEnd)
		return;

	int mid = (leftStart+rightEnd)/2;
	merge_sort_fn(arr, temp, leftStart, mid);
	merge_sort_fn(arr, temp, mid+1, rightEnd);
	merge_halves(arr, temp, leftStart, rightEnd);
}

void merge_halves(int *arr, int *temp, int leftStart, int rightEnd) {
	int leftEnd = (leftStart+rightEnd)/2;
	int rightStart = leftEnd+1;

	int left = leftStart;
	int right = rightStart;
	int index = leftStart;

	printf("leftStart...%d leftEnd...%d rightStart...%d rightEnd...%d \n", leftStart, leftEnd, rightStart, rightEnd);

	while(left <= leftEnd && right <= rightEnd) {
		if(arr[left] <= arr[right]) {
			if(index < left) {
				inversions += (left-index);
				printf("Moving left number...%d\n", arr[left]);
			}
			temp[index++] = arr[left++];
		} else {
			if(index < right) {
				inversions += (right-index);
				printf("Moving right number...%d\n", arr[right]);
			}
			temp[index++] = arr[right++];
		}
	}

	//copy the rest from left & right
	for(int i = left; i <= leftEnd; i++) {
		temp[index++] = arr[i];
	}
	for(int i = right; i <= rightEnd; i++) {
		temp[index++] = arr[i];
	}

	//and finally copy from temp to arr
	for(int i = leftStart; i <= rightEnd; i++) {
		arr[i] = temp[i];
	}

	printf("end....\n");
	for(int j=0; j<5; j++) {
		printf("%d ", arr[j]);
	}
	printf("---\n");
}

//#if 0
int main()
{
    //FILE* fptr = fopen(getenv("OUTPUT_PATH"), "w");
	FILE* fptr = fopen("C:\\Users\\bvenkata\\OneDrive\\eclipse-workspace\\HelloWorld\\src\\HRMergeSortInput.txt", "r");

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
		printf("Unsorted & Sorted:\n");
		for (int i = 0; i < n; i++)
			printf("%d ", arr[i]);
		printf("\n");

		merge_sort(arr_count, arr);

		for (int i = 0; i < n; i++)
			printf("%d ", arr[i]);
		printf("inversions....%d\n", inversions);
		inversions = 0;
	}

	fclose(fptr);

	return 0;
}
//#endif

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
