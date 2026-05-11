# xstdlib

`xstdlib` is an extended standard library for the C language, providing modern data structures, simplified memory management, and utilities to improve productivity and safety of C code.

## Table of Contents

- [Description](#description)
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

## Description

The `xstdlib` project aims to fill the gaps in the C standard library (`stdlib.h`) by providing robust and easy-to-use implementations for common data structures (dynamic arrays, dictionaries, sets) as well as modern mechanisms for error and memory management.

## Installation

The library uses **CMake** as its build system.

### Prerequisites
- CMake (version 3.10 or higher)
- A C compiler (GCC, Clang, MSVC)

### Compilation

```bash
mkdir build
cd build
cmake ..
make
```

### Usage in a Project
You can include `xstdlib` in your CMake project:

```cmake
add_subdirectory(path/to/xstdlib)
target_link_libraries(your_project xstdlib)
```

## Module Overview

### XArray
Dynamic array implementation (vectors).
- **Use Case**: Storing a list of elements whose size can vary during execution.
- **Features**: Push, pop, index access, slicing, copy.
- **Example**:
```c
XArray *arr = XARRAY_NEW(int);
int val = 42;
xarray_push(arr, &val);
int *retrieved = (int *)xarray_at(arr, 0);
printf("%d\n", *retrieved); // 42
```

### XString
Dynamic string manipulation.
- **Use Case**: Replace the tedious and risky use of `char*` and `string.h`.
- **Features**: Concatenation, replacement, split, join, case conversion, search.
- **Example**:
```c
XString *s = xstring_new("Hello");
xstring_concat_c_str(s, " World");
xstring_to_upper(s);
printf("%s\n", s->c_str); // HELLO WORLD
```

### XHashMap
Hash map with string keys.
- **Use Case**: Storing key-value pairs for fast access (dictionary).
- **Features**: Insertion (put), retrieval (get), removal, key and value enumeration. Uses Robin Hood hashing for collisions.
- **Example**:
```c
XHashMap *map = XHASHMAP_NEW(int);
int score = 100;
xhashmap_put(map, "player1", &score);
int *val = (int *)xhashmap_get(map, "player1");
printf("Score: %d\n", *val);
```

### XHashSet
Set of unique elements.
- **Use Case**: Checking membership in a group, eliminating duplicates.
- **Features**: Add, remove, check existence, set operations (union, intersection, difference).
- **Example**:
```c
XHashSet *set = xhashset_new(sizeof(int), xhashset_default_equal_fn, xhashset_default_hash_fn);
int val = 10;
xhashset_add(set, &val);
if (xhashset_has(set, &val)) {
    printf("10 is in the set\n");
}
```

### XMemCtl
Memory manager with checkpoint system.
- **Use Case**: Automating memory release in specific scopes, preventing memory leaks.
- **Features**: `XMEM_SCOPE` for automatic release at block end, `xmem_alloc`, `xmem_rollback`.
- **Example**:
```c
XMEM_SCOPE {
    void *data = xmem_alloc(1024);
    // data is automatically freed when exiting the block
}
```

### XResult
Error management inspired by Rust's `Result` type.
- **Use Case**: Returning errors explicitly without using global or complex return codes.
- **Features**: `OK`, `ERR`, `IS_OK`, `TRY`, `UNWRAP`, `PROPAGATE` macros.
- **Example**:
```c
DEFINE_XRESULT(int);
XResult(int) divide(int a, int b) {
    if (b == 0) return ERR(int, 1);
    return OK(int, a / b);
}

// Usage
XResult(int) res = divide(10, 2);
if (IS_OK(res)) {
    printf("Result: %d\n", UNWRAP(res));
}

// Pointer result value
DEFINE_XPTR(char);
DEFINE_XRESULT(XPtr(char));
XResult(XPtr(char)) read_file(const char *path) {
    char *data = xmem_alloc(1024);
    FILE *fp = fopen(path, "r");
    if (!fp) return ERR(XPtr(char), -22);
    fread(data, 1024, 1, fp);
    fclose(fp);
    return OK(XPtr(char), data);
}

XResult(XPtr(char)) res = read_file("file.txt");
if (IS_OK(res)) {
    printf("File content: %s\n", UNWRAP(res).c_str);
}
```

### XLinkedList
Doubly linked list.
- **Use Case**: Fast insertion and deletion of elements anywhere in the list.
- **Features**: Iterators, push, index access.
- **Example**:
```c
XLinkedList *list = xlinked_list_new(sizeof(int));
int val = 5;
xlinked_list_push(list, &val);
XLinkedListIterator it = xlinked_list_iterator(list);
printf("First: %d\n", *(int *)it.current->value);
```

### XIOUtils
Input/output utilities.
- **Use Case**: Simplified reading from the console.
- **Features**: `xio_read_console_line`.
- **Example**:
```c
XString *input = xio_read_console_line("Enter something: ");
printf("You entered: %s\n", input->c_str);
```

## Usage Example

Here is a simple example combining several modules:

```c
#include "xstdlib.h"
#include <stdio.h>

int main() {
    XMEM_SCOPE {
        // Using XString and XIOUtils
        XString *name = xio_read_console_line("Enter your name: ");
        XString *msg = xstring_new("Hello ");
        xstring_concat(msg, name);
        
        printf("%s!\n", msg->c_str);

        // Using XArray
        XArray *numbers = XARRAY_NEW(int);
        for(int i = 0; i < 5; i++) {
            xarray_push(numbers, &i);
        }
        
        printf("Array size: %u\n", numbers->length);
    } // All allocations made with xmem_alloc or via modules are freed here.
    
    return 0;
}
```

## Other Information

### Tests
To run unit tests:
```bash
./cmake-build-debug/test_xstdlib
```

### License
This project is distributed under the MIT license (or specify according to your needs).
