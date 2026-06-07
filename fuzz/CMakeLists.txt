# ------------------------------------------------------------------
# fuzz/CMakeLists.txt
# Fuzz targets for tr_datastructures
# ------------------------------------------------------------------
# Fuzz testing is not yet implemented.
# This file is a placeholder for future fuzz targets.
#
# Planned fuzz targets:
#   - Stack push with random data sizes and types
#   - Stack create with boundary values for size and capacity
#   - Stack operations sequence fuzzing
#
# Requirements:
#   - Clang with libFuzzer support (-fsanitize=fuzzer)
#   - ASan enabled alongside fuzzer (-fsanitize=fuzzer,address)
#   - Use the asan preset from CMakePresets.json as a base
#
# Recommended approach: libFuzzer (built into Clang)
# Example fuzz target structure:
#
# int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size)
# {
#     struct stack *p_stack = NULL;
#     tr_stack_create(sizeof(uint8_t), 10, TR_STACK_ARRAY_DYNAMIC, &p_stack);
#     tr_stack_push(p_stack, data);
#     tr_stack_destroy(&p_stack);
#     return 0;
# }
# ------------------------------------------------------------------