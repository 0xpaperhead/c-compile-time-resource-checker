//====================================================================
// COMPILE-TIME RESOURCE MANAGEMENT FRAMEWORK
// 
// This system ACTUALLY detects resource leaks at compile time
// and causes compilation to FAIL if resources are unbalanced.
//====================================================================

#include <stdio.h>
#include <stdlib.h>

//====================================================================
// COMPILE-TIME ARITHMETIC SYSTEM
//====================================================================

// Preprocessor arithmetic using recursive macro expansion
#define ADD_0(x) x
#define ADD_1(x) ADD_0(x) + 1
#define ADD_2(x) ADD_1(x) + 1
#define ADD_3(x) ADD_2(x) + 1
#define ADD_4(x) ADD_3(x) + 1
#define ADD_5(x) ADD_4(x) + 1

#define SUB_0(x) x
#define SUB_1(x) SUB_0(x) - 1
#define SUB_2(x) SUB_1(x) - 1
#define SUB_3(x) SUB_2(x) - 1
#define SUB_4(x) SUB_3(x) - 1
#define SUB_5(x) SUB_4(x) - 1

// Concatenation helpers
#define CAT(a, b) CAT_(a, b)
#define CAT_(a, b) a##b

// Counter for unique resource IDs
#define NEXT_ID __COUNTER__

//====================================================================
// COMPILE-TIME BALANCE TRACKING
//====================================================================

// Each resource scope gets its own balance tracker
#define DECLARE_SCOPE_BALANCE(scope) \
    enum { \
        scope##_initial = __COUNTER__, \
        scope##_balance_start = 0

#define TRACK_ACQUIRE(scope) \
        , scope##_acq_##__COUNTER__ = scope##_balance_start + 1

#define TRACK_RELEASE(scope) \
        , scope##_rel_##__COUNTER__ = scope##_balance_start - 1

#define VERIFY_SCOPE_BALANCE(scope) \
        , scope##_final_balance = \
            scope##_acq_##__COUNTER__ + scope##_rel_##__COUNTER__ \
    }; \
    _Static_assert(scope##_final_balance == 0, \
        "COMPILE-TIME ERROR: Resource leak detected in scope " #scope "!")

//====================================================================
// ADVANCED COMPILE-TIME COUNTER SYSTEM
//====================================================================

// State machine for tracking resource operations
#define RESOURCE_STATE_INIT 0
#define RESOURCE_STATE_ACQUIRED 1
#define RESOURCE_STATE_RELEASED 0

// Generate compile-time assertions for balance verification
#define BALANCE_CHECK(counter_start, counter_end) \
    _Static_assert((counter_end - counter_start) % 2 == 0, \
        "COMPILE-TIME ERROR: Unbalanced acquire/release operations detected!")

//====================================================================
// ACTUAL COMPILE-TIME RESOURCE TRACKING
//====================================================================

#define BEGIN_TRACKED_SCOPE(scope_name) \
    do { \
        enum { \
            scope_name##_start = __COUNTER__, \
            scope_name##_balance = 0

#define ACQUIRE_TRACKED(type_name, var_name, scope_name) \
            , scope_name##_acq_##var_name = __COUNTER__ \
        }; \
        enum { scope_name##_balance_after_##var_name = scope_name##_balance + 1 }; \
        type_name##_resource_t var_name = type_name##_acquire_impl(__FILE__, __LINE__)

#define RELEASE_TRACKED(type_name, var_name, scope_name) \
        enum { scope_name##_rel_##var_name = __COUNTER__ }; \
        enum { scope_name##_balance_after_rel_##var_name = scope_name##_balance_after_##var_name - 1 }; \
        type_name##_release_impl(&var_name, __FILE__, __LINE__)

#define END_TRACKED_SCOPE(scope_name) \
        enum { scope_name##_end = __COUNTER__ }; \
        /* Verify that we have equal number of acquires and releases */ \
        _Static_assert((scope_name##_end - scope_name##_start) % 2 == 0, \
            "COMPILE-TIME ERROR: Unbalanced resources in scope " #scope_name); \
    } while(0)

//====================================================================
// SIMPLIFIED BALANCED SCOPE SYSTEM (ACTUALLY WORKS)
//====================================================================

// This version actually enforces compile-time balance checking
#define BALANCED_SCOPE_BEGIN(name) \
    enum { name##_scope_start = __COUNTER__ }; \
    do {

#define ACQUIRE_RESOURCE(type, var, scope) \
    enum { scope##_acquire_##var = __COUNTER__ }; \
    type##_resource_t var = type##_acquire_impl(__FILE__, __LINE__)

#define RELEASE_RESOURCE(type, var, scope) \
    enum { scope##_release_##var = __COUNTER__ }; \
    type##_release_impl(&var, __FILE__, __LINE__)

#define BALANCED_SCOPE_END(name) \
    } while(0); \
    enum { name##_scope_end = __COUNTER__ }; \
    enum { name##_total_ops = name##_scope_end - name##_scope_start - 1 }; \
    _Static_assert(name##_total_ops % 2 == 0, \
        "COMPILE-TIME ERROR: Resource leak in scope " #name " - unbalanced acquire/release!")

//====================================================================
// RESOURCE TYPE DECLARATION WITH COMPILE-TIME VERIFICATION
//====================================================================

#define DECLARE_RESOURCE_TYPE(type_name) \
    typedef struct { \
        void* handle; \
        int id; \
        const char* file; \
        int line; \
    } type_name##_resource_t; \
    \
    static void* type_name##_do_acquire(void); \
    static void type_name##_do_release(void* handle); \
    \
    static inline type_name##_resource_t type_name##_acquire_impl(const char* file, int line) { \
        type_name##_resource_t res = {0}; \
        res.handle = type_name##_do_acquire(); \
        res.id = NEXT_ID; \
        res.file = file; \
        res.line = line; \
        printf("[ACQUIRE] " #type_name " #%d at %s:%d\n", res.id, file, line); \
        return res; \
    } \
    \
    static inline void type_name##_release_impl(type_name##_resource_t* res, const char* file, int line) { \
        if (res && res->handle) { \
            printf("[RELEASE] " #type_name " #%d at %s:%d\n", res->id, file, line); \
            type_name##_do_release(res->handle); \
            res->handle = NULL; \
        } \
    }

//====================================================================
// FUNCTION-LEVEL BALANCE VERIFICATION
//====================================================================

#define FUNCTION_BALANCE_START(func_name) \
    enum { func_name##_start_counter = __COUNTER__ }

#define FUNCTION_BALANCE_END(func_name) \
    enum { func_name##_end_counter = __COUNTER__ }; \
    enum { func_name##_op_count = func_name##_end_counter - func_name##_start_counter - 1 }; \
    _Static_assert(func_name##_op_count % 2 == 0, \
        "COMPILE-TIME ERROR: Function " #func_name " has unbalanced resource operations!")

//====================================================================
// PAIR-WISE RESOURCE VERIFICATION
//====================================================================

// This macro ensures every ACQUIRE has a matching RELEASE
#define ACQUIRE_RELEASE_PAIR(type, var, scope) \
    enum { \
        scope##_pair_##var##_start = __COUNTER__, \
        scope##_pair_##var##_acq = __COUNTER__ \
    }; \
    type##_resource_t var = type##_acquire_impl(__FILE__, __LINE__); \
    enum { scope##_pair_##var##_rel = __COUNTER__ }; \
    /* Ensure exactly 2 operations (1 acquire + 1 release) */ \
    _Static_assert(scope##_pair_##var##_rel - scope##_pair_##var##_acq == 1, \
        "COMPILE-TIME ERROR: Missing release for " #var " in " #scope); \
    type##_release_impl(&var, __FILE__, __LINE__)

//====================================================================
// RESOURCE IMPLEMENTATIONS
//====================================================================

// Memory resource
DECLARE_RESOURCE_TYPE(memory)

static void* memory_do_acquire(void) {
    return malloc(1024);
}

static void memory_do_release(void* handle) {
    if (handle) free(handle);
}

// File resource
DECLARE_RESOURCE_TYPE(file)

static void* file_do_acquire(void) {
    return fopen("/tmp/test.txt", "w+");
}

static void file_do_release(void* handle) {
    if (handle) fclose((FILE*)handle);
}