# CCompileTimeResourceChecker

This project demonstrates a C framework for compile-time resource management verification. It uses preprocessor macros, `__COUNTER__`, and `_Static_assert` to ensure that resources acquired within specific scopes or functions are properly released. If a resource leak (an acquisition without a corresponding release) is detected, the compilation will fail, preventing runtime errors due to resource mismanagement.

## Core Concept

The framework primarily relies on the `__COUNTER__` preprocessor macro to assign unique IDs or to track the number of operations. At the end of a designated scope or function, `_Static_assert` is used to check if the count of resource operations (acquires and releases) is balanced (i.e., an even number, where each acquire has a matching release). If the assertion fails, the compiler issues an error, and compilation stops.

## Files

*   `compile_time_resmgr.c`:
    *   The core resource management framework.
    *   Contains macros like `BALANCED_SCOPE_BEGIN`, `BALANCED_SCOPE_END`, `ACQUIRE_RESOURCE`, `RELEASE_RESOURCE`, `FUNCTION_BALANCE_START`, `FUNCTION_BALANCE_END`, and `ACQUIRE_RELEASE_PAIR`.
    *   Includes `DECLARE_RESOURCE_TYPE` to define new trackable resource types and provides example implementations for `memory` and `file` resources.
    *   This file is designed to be directly included (`#include "compile_time_resmgr.c"`) into other C files that want to use the framework.

*   `test_balanced.c`:
    *   Demonstrates the correct usage of the resource management framework.
    *   All resources are acquired and released properly within their scopes.
    *   This file should compile and run successfully.

*   `test_unbalanced.c`:
    *   Demonstrates a scenario where a resource (`memory`) is acquired but not released.
    *   This file **will fail to compile** due to the static assertion in the framework detecting the resource leak.

## How to Compile and Run

It's recommended to use a C11 compatible compiler (like GCC or Clang).

### 1. Testing Balanced Resource Management

This demonstrates a successful compilation and run where resources are managed correctly.

```bash
# Compile test_balanced.c (it includes compile_time_resmgr.c)
gcc -std=c11 -Wall -Wextra test_balanced.c -o test_balanced

# Run the compiled program
./test_balanced
```

Expected output will show acquisition and release messages, and the program will complete successfully.

### 2. Testing Unbalanced Resource Management (Compile-Time Failure)

This demonstrates the framework catching a resource leak at compile time.

```bash
# Attempt to compile test_unbalanced.c
gcc -std=c11 -Wall -Wextra test_unbalanced.c -o test_unbalanced
```

Compilation will **fail** with an error message similar to:
`error: static assertion failed: "COMPILE-TIME ERROR: Resource leak in scope leak_scope - unbalanced acquire/release!"`
This proves that the framework prevents compilation when resources are not balanced.

## Using the Framework

To use the framework in your own C project:

1.  Place `compile_time_resmgr.c` in your project directory or an include path.
2.  In the C file where you want to manage resources, add `#include "compile_time_resmgr.c"`.
3.  Use the `DECLARE_RESOURCE_TYPE(your_resource_type_name)` macro to define your custom resource types and implement their `_do_acquire` and `_do_release` functions.
4.  Wrap resource operations within `BALANCED_SCOPE_BEGIN(scope_name)` / `BALANCED_SCOPE_END(scope_name)` blocks, or use `FUNCTION_BALANCE_START(func_name)` / `FUNCTION_BALANCE_END(func_name)` for function-level verification.
5.  Use `ACQUIRE_RESOURCE(type, var, scope)` and `RELEASE_RESOURCE(type, var, scope)` for managing resources.

This system provides a powerful, zero-runtime-overhead mechanism for increasing resource safety in C programs by shifting checks to the compile phase. 