#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "txt_num.h"

// Test case structure
struct TestCase {
    long input;
    const char* expected;
};

// Helper function to run a single test case
void run_test(conversion_func convert, const struct TestCase* test, int test_num) {
    char* result = convert(test->input);
    if (strcmp(result, test->expected) != 0) {
        printf("Test %d failed!\n", test_num);
        printf("Input: %ld\n", test->input);
        printf("Expected: %s\n", test->expected);
        printf("Got: %s\n\n", result);
        assert(0);
    } else {
        printf("Test %d passed ✓\n", test_num);
    }
}

int main() {
    // Initialize the conversion function with memoization
    conversion_func convert = initialize_memo();
    
    // Define test cases based on requirements from logic.txt
    struct TestCase tests[] = {
        // Zero dollars and zero cents
        {0, "zero dollars and zero cents"},
        
        // Single digit dollars
        {100, "one dollar and zero cents"},
        {900, "nine dollars and zero cents"},
        
        // Teen numbers
        {1100, "eleven dollars and zero cents"},
        {1900, "nineteen dollars and zero cents"},
        
        // Numbers ending in zero
        {2000, "twenty dollars and zero cents"},
        {70000, "seven hundred dollars and zero cents"},
        
        // Numbers with zeros in middle
        {10200, "one hundred two dollars and zero cents"},
        {100100, "one thousand one dollar and zero cents"},
        
        // Complex numbers with cents
        {123456, "one thousand two hundred thirty-four dollars and fifty-six cents"},
        
        // Single cent
        {1, "zero dollars and one cent"},
        
        // Complex cents only
        {99, "zero dollars and ninety-nine cents"},
        
        // Large numbers
        {100000000, "one million dollars and zero cents"},
        {999999999, "nine million nine hundred ninety-nine thousand nine hundred ninety-nine dollars and ninety-nine cents"},
        
        // Edge cases near MAX_CACHED_VALUE (150000 cents = $1500)
        {149999, "one thousand four hundred ninety-nine dollars and ninety-nine cents"},
        {150000, "one thousand five hundred dollars and zero cents"},
        
        // Numbers with all places
        {111111111, "one million one hundred eleven thousand one hundred eleven dollars and eleven cents"},
    };
    
    int num_tests = sizeof(tests) / sizeof(tests[0]);
    printf("Running %d tests...\n\n", num_tests);
    
    // Run all test cases
    for (int i = 0; i < num_tests; i++) {
        run_test(convert, &tests[i], i + 1);
    }
    
    // Test input validation
    printf("\nTesting input validation...\n");
    assert(validate_input(-1) == 0);
    assert(validate_input(MAX_CAP + 1) == 0);
    assert(validate_input(100) == 1);
    printf("Input validation tests passed ✓\n");
    
    // Clean up
    cleanup_cache();
    printf("\nAll tests completed successfully!\n");
    
    return 0;
}