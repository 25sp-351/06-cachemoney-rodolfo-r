#include "txt_num.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* memo_cache[MAX_CACHED_VALUE] = {NULL};

static const char* ones[] = {"", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};
static const char* teens[] = {"ten", "eleven", "twelve", "thirteen", "fourteen", "fifteen", "sixteen", "seventeen", "eighteen", "nineteen"};
static const char* tens[] = {"", "", "twenty", "thirty", "forty", "fifty", "sixty", "seventy", "eighty", "ninety"};
static const char* places[] = {"", "thousand", "million", "billion"};

int validate_input(long cents){
    if(cents < 0 || cents > MAX_CAP){
        printf("try again, make sure to input numbers from 0 to %ld\n", MAX_CAP);
        return 0;
    } 
    return 1;
}

typedef char* (*conversion_func)(long);

char* memoized_conversion(long cents) {
    // For values under $1500
    if (cents < MAX_CACHED_VALUE) {
        if (memo_cache[cents] != NULL) {
            return memo_cache[cents];
        }
        char* result = convert_to_words(cents);
        memo_cache[cents] = strdup(result);  // Make a copy
        return memo_cache[cents];
    }
    // For larger values, don't memoize
    return convert_to_words(cents);
}

conversion_func initialize_memo(void) {
    // Initialize cache array to NULL
    for (int i = 0; i < MAX_CACHED_VALUE; i++) {
        memo_cache[i] = NULL;
    }
    load_cache_from_file();
    return memoized_conversion;
}

char *convert_to_words(long cents) {
    static char output_buffer[MAX_BUFFER_SIZE] = "";
    char temp_buffer[MAX_BUFFER_SIZE] = "";
    output_buffer[0] = '\0';
    temp_buffer[0] = '\0';
    
    long dollars = cents / 100;
    int change = cents % 100;

    if (dollars == 0) {
        strcat(temp_buffer, "zero dollars");
    } else {
        int first_group = 1;  // Flag for first group
        int group_count = 0;
        char groups[4][100];  // Store groups
        int num_groups = 0;

        // Break into groups and store
        while (dollars > 0) {
            int chunk = dollars % 1000;
            if (chunk > 0) {
                char temp[100];
                // Only add space for place value if it exists
                if (places[group_count][0] != '\0') {
                    sprintf(temp, "%s %s", convert_three_digits(chunk), places[group_count]);
                } else {
                    sprintf(temp, "%s", convert_three_digits(chunk));
                }
                strcpy(groups[num_groups++], temp);
            }
            dollars /= 1000;
            group_count++;
        }

        // Add groups in reverse order
        for (int i = num_groups - 1; i >= 0; i--) {
            if (!first_group) {
                strcat(temp_buffer, " ");
            }
            strcat(temp_buffer, groups[i]);
            first_group = 0;
        }
        dollars = cents / 100;  // Restore dollars value
        strcat(temp_buffer, (dollars % 100 == 1) ? " dollar" : " dollars");
    }

    // Handle cents
    if (change == 1) {
        strcat(temp_buffer, " and one cent");
    } else if (change > 0) {
        strcat(temp_buffer, " and ");
        strcat(temp_buffer, convert_two_digits(change));
        strcat(temp_buffer, " cents");
    } else {
        strcat(temp_buffer, " and zero cents");
    }

    strncpy(output_buffer, temp_buffer, MAX_BUFFER_SIZE - 1);
    output_buffer[MAX_BUFFER_SIZE - 1] = '\0';
    return output_buffer;
}

char* convert_two_digits(int number) {
    static char buffer[100];
    buffer[0] = '\0';

    if (number < 10) {
        strcpy(buffer, ones[number]);
    } else if (number < 20) {
        strcpy(buffer, teens[number - 10]);
    } else {
        if (number % 10 == 0) {
            strcpy(buffer, tens[number / 10]);
        } else {
            sprintf(buffer, "%s-%s", tens[number / 10], ones[number % 10]);
        }
    }
    return buffer;
}

char* convert_three_digits(int number) {
    static char buffer[100];
    buffer[0] = '\0';

    if (number >= 100) {
        sprintf(buffer, "%s hundred", ones[number / 100]);
        number %= 100;
        if (number > 0) {
            strcat(buffer, " ");
            strcat(buffer, convert_two_digits(number));
        }
    } else {
        strcpy(buffer, convert_two_digits(number));
    }
    return buffer;
}

void load_cache_from_file(void) {
    FILE *file = fopen(CACHE_FILENAME, "r");
    if (file == NULL) return;
    
    char line[MAX_BUFFER_SIZE];
    long cents;
    char text[MAX_BUFFER_SIZE];
    
    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "%ld|%[^\n]", &cents, text) == 2) {
            if (cents < MAX_CACHED_VALUE) {
                memo_cache[cents] = strdup(text);
            }
        }
    }
    fclose(file);
}

void save_cache_to_file(void) {
    FILE *file = fopen(CACHE_FILENAME, "w");
    if (file == NULL) return;
    
    for (long i = 0; i < MAX_CACHED_VALUE; i++) {
        if (memo_cache[i] != NULL) {
            fprintf(file, "%ld|%s\n", i, memo_cache[i]);
        }
    }
    fclose(file);
}

void cleanup_cache(void) {
    for (int i = 0; i < MAX_CACHED_VALUE; i++) {
        if (memo_cache[i] != NULL) {
            free(memo_cache[i]);
            memo_cache[i] = NULL;
        }
    }
}