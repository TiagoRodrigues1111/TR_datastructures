# Contributing to TR_datastructures

Thank you for your interest in contributing. This document explains how to get started.

---

## Code of Conduct

This project follows the [Contributor Covenant Code of Conduct](CODE_OF_CONDUCT.md).
Please read it before contributing.

---

## Getting Started

### Prerequisites

- **CMake** 3.21 or later
- **GCC**, **Clang**, or **MSVC**
- **Ninja** (recommended)
- **Doxygen** 1.9 or later (for documentation)
- **Git**

### Building the project

```bash
git clone https://github.com/TiagoRodrigues1111/TR_datastructures.git
cd TR_datastructures

# configure
cmake --preset debug

# build
cmake --build --preset debug

# run tests
ctest --test-dir build/debug --output-on-failure
```

---

## Development Guidelines

### Code style

All C code must be formatted with **clang-format** using the provided `.clang-format` file:

```bash
clang-format -i include/tr_datastructures/*.h src/**/*.c
```

Static analysis is configured via `.clang-tidy`. Fix all warnings before submitting.

### C standard

The library targets **C89 compatibility** with automatic detection of C99/C11/C23 features via `__STDC_VERSION__`. New code must:

- Not use C99 features unconditionally without a `__STDC_VERSION__` guard
- Use `tr_types.h` types (`uint8_t`, `bool`, `size_t`) rather than raw types
- Follow the portable patterns established in `tr_types.h`

### Naming conventions

| Item | Convention | Example |
|------|-----------|---------|
| Functions | `tr_<structure>_<action>` | `tr_stack_push` |
| Structs | `struct <name>` (no typedef) | `struct stack` |
| Enums | `typedef enum <name> <name>_t` | `tr_result_t` |
| Macros | `TR_UPPER_CASE` | `TR_NODISCARD` |
| Local variables | `lower_case` | `p_stack` |
| Pointer variables | `p_` prefix | `p_stack` |
| Global variables | `g_` prefix | `g_instance` |

### File structure

Every `.c` and `.h` file must follow the established section structure:

```c
/* 0 copyright/licensing */
/* 1 includes */
/* 2 defines */
/* 3 external declarations */
/* 4 typedefs */
/* 5 global variable declarations */
/* 6 function prototypes */
```

### Error handling

- Every public function must return `tr_result_t`
- Mark all public functions with `TR_NODISCARD TR_API`
- Validate all pointer arguments at function entry — return `TR_ERR_NULL` on NULL
- Use `fprintf(stderr, ...)` only for unexpected internal failures (e.g. `malloc` returning NULL)
- Never `fprintf` on argument validation failures — the return code is sufficient

### Memory management

- Every allocation must have a corresponding free path on every error branch
- `free_stack_destroy` must set the pointer to NULL after freeing
- Never rely on the caller to free partial allocations

---

## Adding a New Data Structure

Follow this checklist when adding a new data structure:

1. **Header** — create `include/tr_datastructures/tr_<name>.h`
   - Forward declare `struct <name>;`
   - Define implementation type enum `tr_<name>_type_t` if multiple backends
   - Document all functions with Doxygen `/** */` comments
   - Mark all functions `TR_NODISCARD TR_API`

2. **Implementation** — create `src/<name>/`
   - `<name>.c` for array based implementation
   - `<name>_ll.c` for linked list implementation
   - Define `struct <name>` and `struct <name>_ops` in the `.c` file
   - Use function pointer dispatch table for multiple implementations

3. **Tests** — create `tests/<name>/test_<name>.c`
   - Cover all NULL argument cases
   - Cover all invalid argument cases
   - Cover empty structure cases
   - Cover normal operation
   - Cover implementation specific behaviour (fixed vs dynamic)
   - Register with `add_test` in `tests/CMakeLists.txt`

4. **Examples** — create `examples/<name>/example_<name>.c`
   - Show basic usage
   - Show at least one real use case

5. **CMake** — update:
   - `src/CMakeLists.txt` — add source files to `target_sources`
   - `tests/CMakeLists.txt` — add test executable and `add_test`
   - `examples/CMakeLists.txt` — add example executable

6. **Umbrella header** — uncomment the include in `include/tr_datastructures.h`

7. **Documentation** — update:
   - `README.md` — add to the data structures table
   - `CHANGELOG.md` — add entry under `[Unreleased]`

---

## Submitting a Pull Request

1. Fork the repository
2. Create a feature branch: `git checkout -b feature/tr-queue`
3. Make your changes following the guidelines above
4. Ensure all tests pass: `ctest --test-dir build/debug --output-on-failure`
5. Ensure clang-format produces no changes: `clang-format --dry-run -Werror include/tr_datastructures/*.h`
6. Push your branch and open a pull request against `main`
7. Fill in the pull request description explaining what was changed and why

---

## Reporting Bugs

Open an issue on GitHub with:

- A minimal reproducible example
- Your compiler and version (`gcc --version`)
- Your operating system
- The expected behaviour and actual behaviour

---

## License

By contributing to TR_datastructures you agree that your contributions will be released into the public domain under the [Unlicense](LICENSE).