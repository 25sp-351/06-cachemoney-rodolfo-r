#include <stdio.h>
#include "txt_num.h"

typedef char* (*conversion_func)(long);

int main() {
    conversion_func convert = initialize_memo();
    long cents;
    
    printf("enter money amount here: ");
    
    while (scanf("%ld", &cents) != EOF) {
        if (validate_input(cents)) {
            char* result = convert(cents);
            printf("%ld = %s\n", cents, result);
        }
    }

    save_cache_to_file();
    cleanup_cache();
    return 0;
}