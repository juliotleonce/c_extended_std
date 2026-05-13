# xstdlib

`xstdlib` is a comprehensive extended standard library for the C programming language. It provides modern data structures, simplified memory management, and robust utilities designed to enhance productivity, safety, and code readability in C projects.

## Table of Contents

- [Key Features](#key-features)
- [Installation](#installation)
- [Module Overview](#module-overview)
    - [XArray](#xarray)
    - [XString](#xstring)
    - [XHashMap](#xhashmap)
    - [XHashSet](#xhashset)
    - [XMemCtl](#xmemctl)
    - [XResult](#xresult)
    - [XLinkedList](#xlinkedlist)
    - [XIOUtils](#xioutils)
- [Usage Example](#usage-example)
- [Running Tests](#running-tests)
- [License](#license)

## Key Features

- **Modern Data Structures**: Dynamic arrays, hash maps, hash sets, and linked lists with easy-to-use APIs.
- **Memory Safety**: An integrated memory controller with a scoping system (`XMEM_SCOPE`) to automate memory cleanup and prevent leaks.
- **Error Handling**: Rust-inspired `XResult` type for explicit and safe error propagation.
- **String Manipulation**: Enhanced dynamic string handling beyond the standard `string.h`.
- **Lightweight**: Easy to integrate into existing CMake projects.

## Installation

The library uses **CMake** as its build system.

### Prerequisites
- CMake (version 3.10 or higher)
- A C compiler (GCC, Clang, or MSVC)

### Compilation

```bash
mkdir build
cd build
cmake ..
make
```

### Usage in Your Project
You can easily include `xstdlib` as a subdirectory in your CMake project:

```cmake
add_subdirectory(path/to/xstdlib)
target_link_libraries(your_target_name xstdlib)
```

## Type-Safe Aliasing

The library provides a system for type-safe aliasing of generic containers using macros. This helps with code readability and allows for clearer type definitions.

### Defining an Alias
Use `DEFINE_XXX_OF(T)` to create a type alias for a container holding type `T`.

> **Note**: These macros use `typedef` internally. It is recommended to use them once per file for a given type `T`, or ideally place them in a shared header file to avoid redundancy and potential "redefinition" errors.

```c
DEFINE_XARRAY_OF(int)      // Defines XArray_of_int
DEFINE_XHASHMAP_OF(float)  // Defines XHashMap_of_float
DEFINE_XRESULT_OF(int)     // Defines XResult_of_int
```

### Using an Alias
Use `XXX_(T)` (or `XResult(T)`) to refer to the defined alias.

```c
XArray_(int) my_array = XARRAY_NEW(int);
XResult(int) res = OK(int, 42);
```

## Module Overview

### XArray
Dynamic array implementation (vectors).
- **Use Case**: Storing a list of elements whose size can vary during execution.
- **Features**: Push, pop, index access, slicing, copying.
- **Example**:
```c
DEFINE_XARRAY_OF(int)
XArray_(int) arr = XARRAY_NEW(int);
int val = 42;
xarray_push(arr, &val);
int *retrieved = (int *)xarray_at(arr, 0);
printf("%d\n", *retrieved); // Outputs: 42
xarray_free(arr);
```

### XString
Dynamic string manipulation.
- **Use Case**: Replace manual and risky `char*` management.
- **Features**: Concatenation, replacement, split, join, case conversion, and search.
- **Example**:
```c
XString *s = xstring_new("Hello");
xstring_concat_c_str(s, " World");
xstring_to_upper(s);
printf("%s\n", s->c_str); // Outputs: HELLO WORLD
xstring_free(s);
```

### XHashMap
Hash map with string keys and generic values.
- **Use Case**: Efficient key-value pair storage and retrieval (dictionary).
- **Features**: Insertion, retrieval, removal, key/value enumeration. Uses Robin Hood hashing for performance.
- **Example**:
```c
DEFINE_XHASHMAP_OF(int)
XHashMap_(int) map = XHASHMAP_NEW(int);
int score = 100;
xhashmap_put(map, "player1", &score);
int *val = (int *)xhashmap_get(map, "player1");
if (val) printf("Score: %d\n", *val);
xhashmap_free(map);
```

### XHashSet
Set of unique elements.
- **Use Case**: Membership checking and duplicate elimination.
- **Features**: Add, remove, existence check, and set operations (union, intersection, difference).
- **Example**:
```c
DEFINE_XHASHSET_OF(int)
XHashSet_(int) set = xhashset_new(sizeof(int), xhashset_default_equal_fn, xhashset_default_hash_fn);
int val = 10;
xhashset_add(set, &val);
if (xhashset_has(set, &val)) {
    printf("10 is in the set\n");
}
xhashset_free(set);
```

### XMemCtl
Memory controller with a checkpoint and scoping system.
- **Use Case**: Automating memory release in specific blocks to prevent leaks.
- **Features**: `XMEM_SCOPE` for automatic cleanup at block exit, `xmem_alloc`, and `xmem_rollback`.
- **Example**:
```c
XMEM_SCOPE {
    void *data = xmem_alloc(1024);
    // 'data' is automatically freed when the scope is exited.
}
```

### XResult
Explicit error management inspired by Rust's `Result` type.
- **Use Case**: Returning errors without relying on global variables or ambiguous return codes.
- **Features**: `OK`, `ERR`, `IS_OK`, `TRY`, `UNWRAP`, and `PROPAGATE` macros.
- **Example**:
```c
DEFINE_XRESULT(int);
XResult(int) divide(int a, int b) {
    if (b == 0) return ERR(int, 1, "Division by zero");
    return OK(int, a / b);
}

XResult(int) res = divide(10, 2);
if (IS_OK(res)) {
    printf("Result: %d\n", UNWRAP(res));
}
```

### XLinkedList
Doubly linked list implementation.
- **Use Case**: Efficient insertion and deletion of elements at any position.
- **Features**: Iterators, push, and index-based access.
- **Example**:
```c
DEFINE_XLINKED_LIST_OF(int)
XLinkedList_(int) list = xlinked_list_new(sizeof(int));
int val = 5;
xlinked_list_push(list, &val);
XLinkedListIterator it = xlinked_list_iterator(list);
printf("Value: %d\n", *(int *)it.current->value);
xlinked_list_free(list);
```

### XIOUtils
Simplified input/output utilities.
- **Use Case**: Easy user input reading from the console.
- **Features**: `xio_read_console_line`.
- **Example**:
```c
XString *input = xio_read_console_line("Enter your name: ");
printf("Hello, %s\n", input->c_str);
xstring_free(input);
```

## Usage Example

A complete example demonstrating the integration of multiple modules:

```c
#include "xstdlib.h"
#include <stdio.h>

DEFINE_XARRAY_OF(int)

int main() {
    XMEM_SCOPE {
        // Input and String modules
        XString *name = xio_read_console_line("Enter your name: ");
        XString *msg = xstring_new("Greetings, ");
        xstring_concat(msg, name);
        printf("%s!\n", msg->c_str);

        // Dynamic Array module
        XArray_(int) numbers = XARRAY_NEW(int);
        for(int i = 0; i < 5; i++) {
            xarray_push(numbers, &i);
        }
        printf("Array contains %u elements.\n", numbers->length);
        
    } // All allocations (XString, XArray, etc.) tracked by XMemCtl are freed here.
    
    return 0;
}
```

## Running Tests

To build and run the unit tests:

```bash
mkdir build && cd build
cmake ..
make
./test_xstdlib
```

## License

This project is licensed under the MIT License - see the LICENSE file for details (or your preferred license).
