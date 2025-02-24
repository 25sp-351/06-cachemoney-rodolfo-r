#ifndef TXT_NUM_H
#define TXT_NUM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Define constants
#define MAX_BUFFER_SIZE 1024    // For string operations
#define MAX_CACHED_VALUE 150000 // $1500 in cents
#define CACHE_FILENAME "money_cache.txt"
#define MAX_CAP 100000000000

// Function pointer type definition
typedef char* (*conversion_func)(long);

// // Error codes
// #define SUCCESS 0
// #define ERROR_INVALID_INPUT 1
// #define ERROR_MEMORY_ALLOCATION 2
// #define ERROR_FILE_OPERATION 3

// Main functions
int validate_input(long cents);
char *convert_to_words(long cents);

//memoized functions
conversion_func initialize_memo(void);  // Returns function pointer to memoized version
char* memoized_conversion(long cents);  // The memoized wrapper function

// Helper functions
char* convert_three_digits(int number);
char* convert_two_digits(int number);

// Memory management
int initialize_cache(void);
void cleanup_cache(void);
int add_to_cache(long cents, const char* text);  // Added
char* get_from_cache(long cents);                // Added

// String handling
int append_to_buffer(char* buffer, const char* word);
void cleanup_string(char* str);                  // Added

// Cache persistence
void load_cache_from_file(void);
void save_cache_to_file(void);

#endif