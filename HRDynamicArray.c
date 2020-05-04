#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char* readline(FILE *fptr);
static char* ltrim(char*);
static char* rtrim(char*);
static char** split_string(char*);

static int parse_int(char*);

/*
 * Complete the 'dynamicArray' function below.
 *
 * The function is expected to return an INTEGER_ARRAY.
 * The function accepts following parameters:
 *  1. INTEGER n
 *  2. 2D_INTEGER_ARRAY queries
 */

/*
 * To return the integer array from the function, you should:
 *     - Store the size of the array to be returned in the result_count variable
 *     - Allocate the array statically or dynamically
 *
 * For example,
 * int* return_integer_array_using_static_allocation(int* result_count) {
 *     *result_count = 5;
 *
 *     static int a[5] = {1, 2, 3, 4, 5};
 *
 *     return a;
 * }
 *
 * int* return_integer_array_using_dynamic_allocation(int* result_count) {
 *     *result_count = 5;
 *
 *     int *a = malloc(5 * sizeof(int));
 *
 *     for (int i = 0; i < 5; i++) {
 *         *(a + i) = i + 1;
 *     }
 *
 *     return a;
 * }
 *
 */

typedef struct sequence_t {
	int count;
	int size;
	int *data;
} SEQ;

int* dynamicArray(int n, int queries_rows, int queries_columns, int** queries, int* result_count) {
	SEQ **sequences = malloc(n*sizeof(SEQ*));
	memset(sequences, 0, n*(sizeof(SEQ*)));

	int *result = malloc(queries_rows*sizeof(int));
	int last_answer = 0;

	int query, x, y;
	int *row;
	SEQ *sequence;

	for(int i=0; i<queries_rows; i++) {
		row = queries[i];
		query = row[0];
		x = row[1];
		y = row[2];

		if(query==1) {
			sequence = sequences[(x ^ last_answer)%n];
			if(!sequence) {
				sequence = malloc(sizeof(SEQ));
				sequence->count = 0;
				sequence->size = 10;
				sequence->data = malloc(sequence->size*sizeof(int));
				//memset(sequence.data, 0, sequence.size*sizeof(int));
				sequences[(x ^ last_answer)%n] = sequence;
			} else if(sequence->count==sequence->size) {
				//reached limit
				sequence->size *= 2;
				sequence->data = realloc(sequence->data, sequence->size*sizeof(int));
			}
			sequence->data[sequence->count] = y;
			sequence->count++;
		} else if(query==2) {
			sequence = sequences[(x ^ last_answer)%n];
			last_answer = sequence->data[y%sequence->count];
			result[*result_count] = last_answer;
			(*result_count)++;
		}
	}

	//result = realloc(result, *result_count);
	return result;
}

#if 0
int main()
{
    //FILE* fptr = fopen(getenv("OUTPUT_PATH"), "w");
	FILE* fptr = fopen("C:\\Users\\bvenkata\\OneDrive\\eclipse-workspace\\HelloWorld\\src\\HRDynamicArrayInput.txt", "r");

    char** first_multiple_input = split_string(rtrim(readline(fptr)));

    int n = parse_int(*(first_multiple_input + 0));

    int q = parse_int(*(first_multiple_input + 1));

    int** queries = malloc(q * sizeof(int*));

    for (int i = 0; i < q; i++) {
        *(queries + i) = malloc(3 * (sizeof(int)));

        char** queries_item_temp = split_string(rtrim(readline(fptr)));

        for (int j = 0; j < 3; j++) {
            int queries_item = parse_int(*(queries_item_temp + j));

            *(*(queries + i) + j) = queries_item;
        }
    }

    int result_count;
    int* result = dynamicArray(n, q, 3, queries, &result_count);

    for (int i = 0; i < result_count; i++) {
        printf("%d", *(result + i));

        if (i != result_count - 1) {
            printf("\n");
        }
    }

    fprintf(fptr, "\n");

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

int parse_int(char* str) {
    char* endptr;
    int value = strtol(str, &endptr, 10);

    if (endptr == str || *endptr != '\0') {
        exit(EXIT_FAILURE);
    }

    return value;
}
