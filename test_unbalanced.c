//====================================================================
// TEST ResMgr LEAK DETECTION
// This file should FAIL to compile due to unbalanced resources
// It uses the resource management logic from compile_time_resmgr.c
//====================================================================

#include <stdio.h>
#include <stdlib.h>

// Include the resource management framework
// We are only interested in the macros and resource type definitions,
// not its main() or its own test functions.
#include "compile_time_resmgr.c"

// This function will FAIL to compile due to resource leak
void test_unbalanced_function(void) {
    printf("Testing unbalanced resources...\n");
    
    BALANCED_SCOPE_BEGIN(leak_scope)
        ACQUIRE_RESOURCE(memory, leaked_mem, leak_scope);
        printf("Acquired memory but won't release it!\n");
        // Missing RELEASE_RESOURCE - this should cause compilation failure!
    BALANCED_SCOPE_END(leak_scope);
}

int main(void) {
    printf("This should never print due to compilation failure!\n");
    test_unbalanced_function();
    return 0;
} 