//====================================================================
// TEST COMPILE-TIME BALANCED RESOURCE MANAGEMENT
// This file should COMPILE SUCCESSFULLY as resources are balanced.
// It uses the resource management logic from compile_time_resmgr.c
//====================================================================

#include <stdio.h>
#include <stdlib.h>

// Include the resource management framework
#include "compile_time_resmgr.c"

// This function demonstrates balanced resource usage
void test_balanced_resources_function(void) {
    printf("\n=== Testing Balanced Resources (Should Compile Successfully) ===\n");

    FUNCTION_BALANCE_START(balanced_func_scope);

    BALANCED_SCOPE_BEGIN(scope1)
        printf("Entering scope1...\n");
        ACQUIRE_RESOURCE(memory, mem1, scope1);
        ACQUIRE_RESOURCE(file, f1, scope1);

        if (mem1.handle && f1.handle) {
            printf("Resources mem1 and f1 acquired successfully in scope1.\n");
            fprintf((FILE*)f1.handle, "Hello from balanced scope1!\n");
        }

        RELEASE_RESOURCE(file, f1, scope1);
        RELEASE_RESOURCE(memory, mem1, scope1);
        printf("Exiting scope1, resources released.\n");
    BALANCED_SCOPE_END(scope1);

    BALANCED_SCOPE_BEGIN(scope2)
        printf("\nEntering scope2...\n");
        ACQUIRE_RESOURCE(memory, mem2, scope2);
        if (mem2.handle) {
            printf("Resource mem2 acquired successfully in scope2.\n");
        }
        RELEASE_RESOURCE(memory, mem2, scope2);
        printf("Exiting scope2, mem2 released.\n");
    BALANCED_SCOPE_END(scope2);
    
    // Demonstrate pair-wise verification (implicitly balanced)
    printf("\nDemonstrating ACQUIRE_RELEASE_PAIR...\n");
    ACQUIRE_RELEASE_PAIR(memory, mem_pair, pair_scope_balanced);
    ACQUIRE_RELEASE_PAIR(file, file_pair, pair_scope_balanced);
    printf("ACQUIRE_RELEASE_PAIR demonstrated successfully.\n");

    FUNCTION_BALANCE_END(balanced_func_scope);
    printf("test_balanced_resources_function completed successfully.\n");
}

int main(void) {
    printf("Starting tests for balanced resource management...\n");
    test_balanced_resources_function();
    printf("\nAll balanced tests completed. Compilation should be successful.\n");
    return 0;
} 