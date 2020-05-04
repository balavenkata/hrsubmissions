#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<time.h>

static char* readline(FILE *fpter);
static char** split_string(char*);

static int MAX_EXPENSE_ARR_SIZE = 201;

static int cmp_num(const void *a, const void *b) {
    int a_int = *((int*)a);
    int b_int = *((int*)b);
    return a_int-b_int;
}

static double get_median(int *exp_freq_arr, int d) {
	int expense_freq, sum_freq = 0;
	int last_expense = -1, last_freq = -1;
	for(int expense = 0; expense < MAX_EXPENSE_ARR_SIZE; expense++) {
		expense_freq = exp_freq_arr[expense];
		sum_freq += expense_freq;

		if(expense_freq && sum_freq>=d/2 && last_expense < 0) {
			last_expense = expense;
			last_freq = exp_freq_arr[expense];
			continue;
		}

		if(last_expense > 0 && last_freq > 0 && expense_freq > 0) {
			if((sum_freq-exp_freq_arr[expense]) <= d/2) {
				if(d%2)
					return expense;
				else
					return (last_expense+expense)/2.0;
			} else {
				return last_expense;
			}
		}
	}

	return last_expense;
}

// Complete the activityNotifications function below.
static int activityNotifications(int expenditure_count, int* expenditure, int d) {
    int *last_d = malloc(d * sizeof(int));

    //Max expenditure is 200; so limiting the expenditure num array to 201
    int *expense_freq_arr = malloc(MAX_EXPENSE_ARR_SIZE*sizeof(int));
    memset(expense_freq_arr, 0, MAX_EXPENSE_ARR_SIZE*sizeof(int));

    int count = 0;
    double median;
    for(int i = d; i<expenditure_count ; i++) {
		if(i==d) {
			memcpy(last_d, (expenditure+i-d), d * sizeof(int));
			qsort(last_d, d, sizeof(int), cmp_num);
			for(int m = 0; m < d; m++) {
				expense_freq_arr[last_d[m]]++;
			}
		} else {
			//eject (i-d-1)th element; add (i-1)th element
			expense_freq_arr[expenditure[i-d-1]]--;
			expense_freq_arr[expenditure[i-1]]++;
		}
		median = get_median(expense_freq_arr, d);

		if(expenditure[i] >= (2*median))
			count++;
    }

    free(expense_freq_arr);
    free(last_d);

    return count;
}

#if 0
int main()
{
    //FILE* fptr = fopen(getenv("OUTPUT_PATH"), "w");
	FILE* fptr = fopen("C:\\Users\\bvenkata\\OneDrive\\eclipse-workspace\\HelloWorld\\src\\HRFraudNotifInput.txt", "r");

    char** nd = split_string(readline(fptr));

    char* n_endptr;
    char* n_str = nd[0];
    int n = strtol(n_str, &n_endptr, 10);

    if (n_endptr == n_str || *n_endptr != '\0') { exit(EXIT_FAILURE); }

    char* d_endptr;
    char* d_str = nd[1];
    int d = strtol(d_str, &d_endptr, 10);

    if (d_endptr == d_str || *d_endptr != '\0') { exit(EXIT_FAILURE); }

    char** expenditure_temp = split_string(readline(fptr));

    int* expenditure = malloc(n * sizeof(int));

    for (int i = 0; i < n; i++) {
        char* expenditure_item_endptr;
        char* expenditure_item_str = *(expenditure_temp + i);
        int expenditure_item = strtol(expenditure_item_str, &expenditure_item_endptr, 10);

        if (expenditure_item_endptr == expenditure_item_str || *expenditure_item_endptr != '\0') { exit(EXIT_FAILURE); }

        *(expenditure + i) = expenditure_item;
    }

    int expenditure_count = n;

    clock_t begin = clock();
    int result = activityNotifications(expenditure_count, expenditure, d);
    clock_t end = clock();
	double time_spent = 0.0;
	time_spent += (double) (end-begin) / CLOCKS_PER_SEC;

    printf("%d\n", result);
    printf("time taken to finish %f seconds\n", time_spent);

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
