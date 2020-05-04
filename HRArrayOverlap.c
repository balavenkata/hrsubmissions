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

char* readline();
char** split_string(char*);

/*
typedef struct row {
    int a;
    int b;
    unsigned long k;
} ROW;

static int cmp_row(const void *a, const void *b) {
    ROW *a_row = (ROW*)a;
    ROW *b_row = (ROW*)b;
    if(a_row->a == b_row->a)
    	if(a_row->b == b_row->b)
    		return  (a_row->k - b_row->k);
    	else
    		return  (a_row->b - b_row->b);
    else
    	return  (a_row->a - b_row->a);
}

static int cmp_row_k(const void *a, const void *b) {
    ROW *a_row = (ROW*)a;
    ROW *b_row = (ROW*)b;
    return  (b_row->k - a_row->k);
}
*/

/*
// Complete the arrayManipulation function below.
static long arrayManipulation(int n, int queries_rows, int queries_columns, int** queries) {
    unsigned long max = 0;
    int o_min, o_max;

    ROW *rows = malloc(queries_rows * sizeof(ROW));
    memset(rows, 0, queries_rows*sizeof(ROW));

    for(int i=0; i<queries_rows; i++) {
        int *row = queries[i];
        ROW my_row;
        my_row.a = row[0];
        my_row.b = row[1];
        my_row.k = row[2];
        rows[i] = my_row;

       // printf("a = %d, b = %d, k = %d\n", my_row.a, my_row.b, my_row.k);
    }

    qsort(rows, queries_rows, sizeof(ROW), cmp_row);
    //bool in_overlap = false;
    //long over	lap = 0;
    int max2 = 0;

    for(int i=0; i<queries_rows; i++) {
    	printf("reading row....a:%d b: %d k:%d\n", rows[i].a, rows[i].b, rows[i].k);

        if(i >0) {
            if(rows[i-1].b >= rows[i].a) {
                //overlap
                //in_overlap = true;

                if(o_max >= rows[i].a) {
                    //continuing overlap
                    o_min = rows[i].a;
                    if(rows[i].b < o_max)
                    	o_max = rows[i].b > rows[i-1].b ? rows[i-1].b : rows[i].b;

                    max += rows[i].k;
                } else {
                    //new overlap OR expansion
                    max2 = rows[i-1].k + rows[i].k;
                    if(max2 > max) {
                        max = max2;
                        o_min = rows[i].a;
                        o_max = rows[i].b > rows[i-1].b ? rows[i].b : rows[i-1].b;
                    }
                }
            } else {
                //no overlap
                //in_overlap = false;
                if (max < rows[i].k) {
                    max = rows[i].k;
                    o_min = rows[i].a;
                    o_max = rows[i].b;
                }
            }
        } else {
            max = rows[i].k;
            o_min = rows[i].a;
            o_max = rows[i].b;
        }

        printf("max after %dth row....%lu...o_min %d  o_max %d\n", i,max, o_min, o_max);
    }



    return max;
}*/

/*
static long arrayManipulation(int n, int queries_rows, int queries_columns, int** queries) {
	ROW *rows = malloc(queries_rows * sizeof(ROW));
	memset(rows, 0, queries_rows*sizeof(ROW));

	for(int i=0; i<queries_rows; i++) {
		int *row = queries[i];
		ROW my_row;
		my_row.a = row[0];
		my_row.b = row[1];
		my_row.k = row[2];
		rows[i] = my_row;
	}

	qsort(rows, queries_rows, sizeof(ROW), cmp_row);

	ROW *overlap_rows = malloc(n * sizeof(ROW));
	memset(overlap_rows, 0, n*sizeof(ROW));
	int overlap_row_count = 0;

	///logic
	for(int i=0; i<queries_rows; i++) {
		printf("reading row....a: %d b: %d k: %d\n", rows[i].a, rows[i].b, rows[i].k);
		if(!overlap_row_count) {
			ROW overlap_row;
			overlap_row.a = rows[i].a; overlap_row.b = rows[i].b; overlap_row.k = rows[i].k;
			overlap_rows[overlap_row_count] = overlap_row;
			overlap_row_count++;
		} else {
			int j;
			for(j = overlap_row_count; j > 0; j--) {
				if(overlap_rows[j-1].b >= rows[i].a && overlap_rows[j-1].b <= rows[i].b) {
					//there is overlap
					int temp;
					temp = overlap_rows[j-1].b;
					overlap_rows[j-1].b = (rows[i].a)-1;

					ROW overlap_row;
					overlap_row.a = rows[i].a; overlap_row.b = temp; overlap_row.k = overlap_rows[j-1].k + rows[i].k;
					overlap_rows[overlap_row_count] = overlap_row;
					overlap_row_count++;

					overlap_row.a = temp+1; overlap_row.b = rows[i].b; overlap_row.k = rows[i].k;
					overlap_rows[overlap_row_count] = overlap_row;
					overlap_row_count++;
					break;

				} else if(overlap_rows[j-1].b > rows[i].a) {
					continue;
				}
				else {
					//no overlap
					ROW overlap_row;
					overlap_row.a = rows[i].a; overlap_row.b = rows[i].b; overlap_row.k = rows[i].k;
					overlap_rows[overlap_row_count] = overlap_row;
					overlap_row_count++;
					break;
				}
			}
			if(j==0 && overlap_row_count > 0) {
				if(overlap_rows[0].b > rows[i].a) {
					int temp1, temp2;
					temp1 = overlap_rows[0].b;
					temp2 = rows[i].b;

					overlap_rows[0].b = (rows[i].a)-1;

					ROW overlap_row;
					overlap_row.a = rows[i].a; overlap_row.b = (temp1<temp2 ? temp1 : temp2); overlap_row.k = overlap_rows[0].k + rows[i].k;
					overlap_rows[overlap_row_count] = overlap_row;
					overlap_row_count++;

					overlap_row.a = (temp1<temp2 ? temp1 : temp2)+1; overlap_row.b = (temp1>temp2 ? temp1 : temp2); overlap_row.k = (temp1>temp2) ? overlap_rows[0].k : rows[i].k;
					overlap_rows[overlap_row_count] = overlap_row;
					overlap_row_count++;
				}
			}
		}
		for(int k=0; k<n; k++) {
			printf("reading overlap_rows....a: %d b: %d k: %d\n", overlap_rows[k].a,
									overlap_rows[k].b, overlap_rows[k].k);
		}
	}

	qsort(overlap_rows, n, sizeof(ROW), cmp_row_k);

	return overlap_rows[0].k;
}*/

static long arrayManipulation(int n, int queries_rows, int queries_columns, int** queries) {
	long max = 0,x = 0;
	long *arr;
	int a, b, k;
	arr = malloc((n+1) * sizeof(long));
	memset(arr, 0, (n+1)*sizeof(long));

	for(int i=0; i<queries_rows; i++) {
		a = queries[i][0];
		b = queries[i][1];
		k = queries[i][2];
		arr[a-1] += k;
		arr[b] += -k;

		printf("a=%d, b=%d, k%d arr[a-1] %d arr[b] %d\n", a, b, k, arr[a-1], arr[b]);
	}

	for(int j = 0; j <= n; j++) {
		printf("%d ", arr[j]);
		x += arr[j];
		if(max < x)
			max = x;
	}
	printf("\n");

	return max;
}

//FILE* fptr;

/*
int main()
{
    //FILE* fptr = fopen(getenv("OUTPUT_PATH"), "w");
	FILE *fptr = fopen("C:\\Users\\bvenkata\\OneDrive\\eclipse-workspace\\HelloWorld\\src\\HRArrayOverlapInput.txt", "r");
	if(fptr==0)
		printf("not opening file");

    char** nm = split_string(readline(fptr));

    char* n_endptr;
    char* n_str = nm[0];
    int n = strtol(n_str, &n_endptr, 10);

    if (n_endptr == n_str || *n_endptr != '\0') { exit(EXIT_FAILURE); }

    char* m_endptr;
    char* m_str = nm[1];
    int m = strtol(m_str, &m_endptr, 10);

    if (m_endptr == m_str || *m_endptr != '\0') { exit(EXIT_FAILURE); }

    int** queries = malloc(m * sizeof(int*));

    for (int i = 0; i < m; i++) {
        *(queries + i) = malloc(3 * (sizeof(int)));

        char** queries_item_temp = split_string(readline(fptr));

        for (int j = 0; j < 3; j++) {
            char* queries_item_endptr;
            char* queries_item_str = *(queries_item_temp + j);
            int queries_item = strtol(queries_item_str, &queries_item_endptr, 10);

            if (queries_item_endptr == queries_item_str || *queries_item_endptr != '\0') { exit(EXIT_FAILURE); }

            *(*(queries + i) + j) = queries_item;
        }
    }

    int queries_rows = m;
    int queries_columns = 3;

    clock_t begin = clock();
    long result = arrayManipulation(n, queries_rows, queries_columns, queries);
    clock_t end = clock();
    double time_spent = 0.0;
	time_spent += (double) (end-begin) / CLOCKS_PER_SEC;
	printf("time taken %f seconds\n", time_spent);

    printf("%ld\n", result);

    fclose(fptr);

    return 0;
}

char* readline(FILE *fptr) {
    size_t alloc_length = 1024;
    size_t data_length = 0;
    char* data = malloc(alloc_length);

    while (true) {
        char* cursor = data + data_length;
        char* line = fgets(cursor, alloc_length - data_length, fptr);

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
*/
