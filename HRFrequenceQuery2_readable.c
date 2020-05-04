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
#include <time.h>

static char* readline();
static char* ltrim(char*);
static char* rtrim(char*);
static char** split_string(char*);

static unsigned int HASH_SIZE = 99999;

typedef struct num_freq_t {
  unsigned int num;
  unsigned int freq;
  struct num_freq_t *next;
}
NUM_FREQ;

typedef struct num_hash_t {
  NUM_FREQ **data;
}
NUM_HASH;

typedef struct freq_hash_t {
  NUM_FREQ **data;
}
FREQ_HASH;

static unsigned int hash_func(unsigned int x) {
  x ^= (x >> 20) ^ (x >> 12);
  return x ^ (x >> 7) ^ (x >> 4);
}

static unsigned int put_num_hash(NUM_HASH *pNumHash, unsigned int n) {
  unsigned int hash = hash_func(n);
  unsigned int index = hash % HASH_SIZE;

  NUM_FREQ *entry = pNumHash->data[index];
  if (!entry) {
    //allocate and assign
    entry = malloc(sizeof(NUM_FREQ));
    entry->num = n;
    entry->freq = 1;
    entry->next = NULL;
    pNumHash->data[index] = entry;
  } else {
    NUM_FREQ *prev = 0;
    while (entry) {
      if (n == entry->num) {
        entry->freq++;
        break;
      }
      prev = entry;
      entry = entry->next;
    }

    if (!entry) {
      entry = malloc(sizeof(NUM_FREQ));
      entry->num = n;
      entry->freq = 1;
      entry->next = NULL;
      prev->next = entry;
    }
  }

  return entry->freq;
}

static int del_num_hash(NUM_HASH *pNumHash, unsigned int n) {
  unsigned int hash = hash_func(n);
  unsigned int index = hash % HASH_SIZE;

  NUM_FREQ *entry = pNumHash->data[index];
  if (!entry) {
    return -1;
  } else {
    NUM_FREQ *prev = 0;
    while (entry) {
      if (n == entry->num) {
        entry->freq--;
        break;
      }
      prev = entry;
      entry = entry->next;
    }

    if (!entry) {
      return -1;
    } else {
      if (entry->freq == 0) {
        //free entry, direct its prev->next to entry->next
        if (prev)
          prev->next = entry->next;
        else {
          //"entry" is the first...set its next as head
          pNumHash->data[index] = entry->next;
        }
        free(entry);
      }
    }
  }

  return entry->freq;
}

static void put_freq_hash(FREQ_HASH *pFreqHash, unsigned int n, bool is_add,
  unsigned int new_freq) {
  //if new_freq is o, remove the number n
  //	if n is the only entry then remove the bucket
  unsigned int old_freq = is_add ? (new_freq - 1) : (new_freq + 1);
  unsigned int old_hash = hash_func(old_freq);
  unsigned int old_index = old_hash % HASH_SIZE;

  NUM_FREQ *old_list = NULL;
  NUM_FREQ *new_list = NULL;

  if (is_add && old_freq == 0) {
    //skip...adding n first time
  } else {
    old_list = pFreqHash->data[old_index];
    if (old_list) {
      //process old...remove n from old_index
      NUM_FREQ *prev = 0;
      while (old_list) {
        if (n == old_list->num) {
          break;
        }
        prev = old_list;
        old_list = old_list->next;
      }

      if (prev && old_list) {
        prev->next = old_list->next;
      } else if (old_list == pFreqHash->data[old_index]) {
        pFreqHash->data[old_index] = old_list->next;
      }
    }
  }

  if (!is_add && new_freq == 0) {
    if (pFreqHash->data[old_index] == old_list)
      pFreqHash->data[old_index] = NULL;
    free(old_list);
  } else {
    //process new...
    unsigned int new_hash = hash_func(new_freq);
    unsigned int new_index = new_hash % HASH_SIZE;

    new_list = pFreqHash->data[new_index];
    if (!new_list) {
      //allocate and assign
      new_list = malloc(HASH_SIZE * sizeof(NUM_FREQ));
      new_list->num = n;
      new_list->freq = new_freq;
      new_list->next = NULL;
      pFreqHash->data[new_index] = new_list;
    } else {
      NUM_FREQ *prev = 0;
      NUM_FREQ *same_freq_item = 0;

      //The following while loop optimization made me
      //bring down the running time frm 3+ seconds to
      //0.02 seconds. Essentially I'm inserting the item
      //to the "head" of items with same "frequency" instead
      //of going to the "tail" and appending
      while (new_list) {
        if (new_freq == new_list->freq) {
          //found
          same_freq_item = new_list;
          break;
        }
        prev = new_list;
        new_list = new_list->next;
      }

      new_list = old_list ? old_list : malloc(sizeof(NUM_FREQ));
      new_list->num = n;
      new_list->freq = new_freq;
      new_list->next = same_freq_item;
      if (prev)
        prev->next = new_list;
      else
        pFreqHash->data[new_index] = new_list;
    }
  }
}

static bool get_freq_hash(FREQ_HASH *pFreqHash, unsigned int freq) {
  unsigned int hash = hash_func(freq);
  unsigned int index = hash % HASH_SIZE;

  NUM_FREQ *entry = pFreqHash->data[index];
  while (entry) {
    if (freq == entry->freq) {
      break;
    }
    entry = entry->next;
  }

  if (entry)
    return true;
  else
    return false;
}

// Complete the freqQuery function below.
static int *freqQuery(int queries_rows, int queries_columns, int **queries,
  int *result_count) {

  FREQ_HASH *freq_hash = malloc(sizeof(FREQ_HASH));
  freq_hash->data = malloc(HASH_SIZE * sizeof(NUM_FREQ *));
  memset(freq_hash->data, 0, HASH_SIZE * sizeof(NUM_FREQ *));

  NUM_HASH *num_hash = malloc(sizeof(NUM_HASH));
  num_hash->data = malloc(HASH_SIZE * sizeof(NUM_FREQ *));
  memset(num_hash->data, 0, HASH_SIZE * sizeof(NUM_FREQ *));

  *result_count = 0;
  int *result = malloc(queries_rows * sizeof(int));

  for (int i = 0; i < queries_rows; i++) {
    int *query = queries[i];
    int oper = query[0];
    int num_or_freq = query[1];
    int freq;

    if (oper == 1) {
      //insert num into data struct
      freq = put_num_hash(num_hash, num_or_freq);
      put_freq_hash(freq_hash, num_or_freq, true, freq);
    } else if (oper == 2) {
      //Delete one occurence of num from your data structure, if present.
      freq = del_num_hash(num_hash, num_or_freq);
      if (freq >= 0)
        put_freq_hash(freq_hash, num_or_freq, false, freq);
    } else if (oper == 3) {
      //Check if any integer is present whose frequency is exactly num.
      //If yes, print 1 else 0.
      if (get_freq_hash(freq_hash, num_or_freq))
        result[( *result_count)] = 1;
      else
        result[( *result_count)] = 0;
      ( *result_count) ++;
    }
  }

  return result;
}

#if 0
int main() {
	//FILE* fptr = fopen(getenv("OUTPUT_PATH"), "w");
	FILE* fptr =
			fopen(
					"C:\\Users\\bvenkata\\OneDrive\\eclipse-workspace\\HelloWorld\\src\\HRFrequencyQueryInput.txt",
					"r");

	char* q_endptr;
	char* q_str = ltrim(rtrim(readline(fptr)));
	int q = strtol(q_str, &q_endptr, 10);

	if (q_endptr == q_str || *q_endptr != '\0') {
		exit(EXIT_FAILURE);
	}

	int** queries = malloc(q * sizeof(int*));

	for (int i = 0; i < q; i++) {
		*(queries + i) = malloc(2 * (sizeof(int)));

		char** queries_item_temp = split_string(rtrim(readline(fptr)));

		for (int j = 0; j < 2; j++) {
			char* queries_item_endptr;
			char* queries_item_str = *(queries_item_temp + j);
			int queries_item = strtol(queries_item_str, &queries_item_endptr,
					10);

			if (queries_item_endptr == queries_item_str
					|| *queries_item_endptr != '\0') {
				exit(EXIT_FAILURE);
			}

			*(*(queries + i) + j) = queries_item;
		}
	}

	int queries_rows = q;
	int queries_columns = 2;

	int ans_count;
	clock_t begin = clock();
	int* ans = freqQuery(queries_rows, queries_columns, queries, &ans_count);

	for (int i = 0; i < ans_count; i++) {
		//fprintf(fptr, "%d", *(ans + i));
		printf("%d", *(ans + i));

		if (i != ans_count - 1) {
			//fprintf(fptr, "\n");
			printf("\n");
		}
	}

	//fprintf(fptr, "\n");
	printf("\n");

	clock_t end = clock();
	double time_spent = 0.0;
	time_spent += (double) (end - begin) / CLOCKS_PER_SEC;
	//printf("time taken to read key value %f seconds..max1 %u max2 %u max3 %u\n", time_spent,max1, max2, max3);
	printf("time taken to read key value %f seconds..\n", time_spent);

	if (fptr)
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
		char* line = fgets(cursor, alloc_length - data_length,
				fptr ? fptr : stdin);

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

