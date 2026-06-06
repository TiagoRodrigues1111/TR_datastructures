# TR_datastructures

[![Build and Test](https://github.com/TiagoRodrigues1111/TR_datastructures/actions/workflows/build_test.yml/badge.svg)](https://github.com/TiagoRodrigues1111/TR_datastructures/actions/workflows/build_test.yml)
[![Documentation](https://github.com/TiagoRodrigues1111/TR_datastructures/actions/workflows/docs.yml/badge.svg)](https://tiagorodrigues1111.github.io/TR_datastructures/)
[![License: Unlicense](https://img.shields.io/badge/license-Unlicense-blue.svg)](https://unlicense.org/)

A generic, portable C data structures library with support for multiple underlying implementations. Designed to work across C89, C99, C11 and C23 standards.

---

## Features

- **Portable** — supports C89 through C23 with automatic standard detection
- **Generic** — all data structures work with any data type via `void *` and `memcpy`
- **Multiple implementations** — choose between array based and linked list backends at runtime
- **Safe API** — every function returns a `tr_result_t` error code, enforced by `TR_NODISCARD`
- **Cross platform** — tested on Ubuntu and Windows with GCC and Clang
- **Well documented** — full API documentation available at [GitHub Pages](https://tiagorodrigues1111.github.io/TR_datastructures/)

---

## Data Structures

| Structure | Array Dynamic | Array Fixed | Linked List | Status      |
|-----------|--------------|-------------|-------------|-------------|
| Stack     | ✅           | ✅          | 🔄          | In progress |
| Queue     | ⬜           | ⬜          | ⬜          | Planned     |

---

## Requirements

- **CMake** 3.21 or later
- **C compiler** — GCC, Clang, or MSVC
- **Ninja** (recommended) or Unix Makefiles

---

## Building

### Clone the repository

```bash
git clone https://github.com/TiagoRodrigues1111/TR_datastructures.git
cd TR_datastructures
```

### Configure and build

**Using CMake presets (recommended):**

```bash
# debug build
cmake --preset debug
cmake --build --preset debug

# release build
cmake --preset release
cmake --build --preset release
```

**Using CMake directly:**

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

### Build options

| Option                | Default | Description                    |
|-----------------------|---------|--------------------------------|
| `TR_BUILD_TESTS`      | OFF     | Build the test suite           |
| `TR_BUILD_EXAMPLES`   | OFF     | Build the usage examples       |
| `TR_BUILD_BENCHMARKS` | OFF     | Build the benchmarks           |
| `TR_BUILD_FUZZ`       | OFF     | Build the fuzz targets         |

Example with options:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -DTR_BUILD_TESTS=ON -DTR_BUILD_EXAMPLES=ON
cmake --build build
```

### Running tests

```bash
ctest --test-dir build --output-on-failure
```

---

## Usage

### Including the library

Include the umbrella header to get access to the full API:

```c
#include "tr_datastructures.h"
```

Or include individual headers as needed:

```c
#include "tr_datastructures/tr_stack.h"
```

### Stack example

```c
#include "tr_datastructures/tr_stack.h"

int main(void)
{
    struct stack *p_stack = NULL;
    tr_result_t   res     = TR_OK;
    int           val     = 42;
    int           top     = 0;

    /* create a dynamic array stack for integers */
    res = tr_stack_create(sizeof(int), 10, TR_STACK_ARRAY_DYNAMIC, &p_stack);
    if (TR_OK != res)
    {
        return 1;
    }

    /* push a value */
    res = tr_stack_push(p_stack, &val);
    if (TR_OK != res)
    {
        tr_stack_destroy(&p_stack);
        return 1;
    }

    /* peek at the top */
    res = tr_stack_top(p_stack, &top);
    if (TR_OK == res)
    {
        printf("Top value: %d\n", top);
    }

    /* pop the value */
    (void)tr_stack_pop(p_stack);

    /* destroy the stack */
    (void)tr_stack_destroy(&p_stack);

    return 0;
}
```

### Stack implementation types

```c
/* dynamic array - grows automatically when full */
tr_stack_create(sizeof(int), 10, TR_STACK_ARRAY_DYNAMIC, &p_stack);

/* fixed array - returns TR_ERR_FULL when capacity is reached */
tr_stack_create(sizeof(int), 10, TR_STACK_ARRAY_FIXED, &p_stack);

/* linked list - dynamic node allocation (coming soon) */
tr_stack_create(sizeof(int), 10, TR_STACK_LL, &p_stack);
```

### Error handling

Every API function returns a `tr_result_t`:

```c
typedef enum tr_result
{
    TR_OK               = 0,
    TR_ERR_NULL         = 1,
    TR_ERR_ALLOC        = 2,
    TR_ERR_EMPTY        = 3,
    TR_ERR_FULL         = 4,
    TR_ERR_INVALID      = 5,
    TR_ERR_NOT_FOUND    = 6,
    TR_ERR_DUPLICATE    = 7,
    TR_ERR_OUT_OF_RANGE = 8
} tr_result_t;
```

The `TR_NODISCARD` attribute causes a compiler warning if a return value is ignored, encouraging proper error handling.

---

## Version checking

```c
#include "tr_datastructures/tr_version.h"

/* check version at compile time */
#if TR_VERSION_AT_LEAST(1, 0, 0)
    /* use features from 1.0.0 onwards */
#endif

/* print version at runtime */
printf("TR_datastructures version: %s\n", TR_VERSION_STRING);
```

---

## Documentation

Full API documentation is available at:

**[https://tiagorodrigues1111.github.io/TR_datastructures/](https://tiagorodrigues1111.github.io/TR_datastructures/)**

To generate documentation locally:

```bash
# Windows
scripts\generate_docs.bat

# Linux / macOS
./scripts/generate_docs.sh
```

Documentation is generated into `docs/output/html/index.html`.

---

## Project structure

```
TR_datastructures/
├── include/
│   ├── tr_datastructures.h          # umbrella header
│   └── tr_datastructures/
│       ├── tr_export.h              # symbol visibility macros
│       ├── tr_result.h              # error code enum
│       ├── tr_types.h               # portable type definitions
│       ├── tr_version.h             # version information (generated)
│       └── tr_stack.h               # stack public API
├── src/
│   ├── internal/
│   │   └── include/
│   │       └── tr_internal.h        # internal utility macros
│   ├── stack/
│   │   ├── stack.c                  # array based implementation
│   │   └── stack_ll.c               # linked list implementation (planned)
│   └── tr_datastructures.c          # library placeholder
├── tests/
│   └── stack/
│       └── test_stack.c             # stack unit tests
├── examples/
│   └── stack/
│       └── example_stack.c          # stack usage examples
├── benchmarks/                      # benchmarks (planned)
├── fuzz/                            # fuzz targets (planned)
├── docs/
│   └── Doxyfile                     # Doxygen configuration
├── scripts/
│   ├── generate_docs.sh             # documentation generation (Linux)
│   └── generate_docs.bat            # documentation generation (Windows)
├── cmake/
│   ├── tr_datastructures-config.cmake.in
│   └── tr_version.h.in
├── .github/
│   └── workflows/
│       ├── build_test.yml           # CI build and test
│       └── docs.yml                 # documentation deployment
├── CMakeLists.txt
├── CMakePresets.json
└── vcpkg.json
```



---

## Contributing

Contributions are welcome. Please read [CONTRIBUTING.md](CONTRIBUTING.md) before submitting a pull request.

---

## License

This project is released into the public domain under the [Unlicense](LICENSE).