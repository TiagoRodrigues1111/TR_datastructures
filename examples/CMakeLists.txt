# ------------------------------------------------------------------
# examples/CMakeLists.txt
# ------------------------------------------------------------------

# ------------------------------------------------------------------
# 1. STACK EXAMPLE
# ------------------------------------------------------------------
add_executable(example_stack
    stack/example_stack.c
)

target_compile_options(example_stack
    PRIVATE
        $<$<C_COMPILER_ID:GNU>:-Wno-unused-result>
        $<$<C_COMPILER_ID:Clang>:-Wno-unused-result>
)

target_link_libraries(example_stack
    PRIVATE
        tr_datastructures
)

# ------------------------------------------------------------------
# 2. QUEUE EXAMPLE (placeholder)
# ------------------------------------------------------------------
# add_executable(example_queue
#     queue/example_queue.c
# )
#
# target_link_libraries(example_queue
#     PRIVATE
#         tr_datastructures
# )