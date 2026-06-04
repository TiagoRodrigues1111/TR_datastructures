# ------------------------------------------------------------------
# tests/CMakeLists.txt
# ------------------------------------------------------------------

# ------------------------------------------------------------------
# 1. UNITY TEST FRAMEWORK
# ------------------------------------------------------------------
include(FetchContent)

FetchContent_Declare(
    unity
    GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
    GIT_TAG        v2.6.0
)

FetchContent_MakeAvailable(unity)


set_target_properties(unity PROPERTIES
    C_CLANG_TIDY   ""
    COMPILE_OPTIONS ""
)

target_compile_options(unity
    PRIVATE
        $<$<C_COMPILER_ID:GNU>:-w>
        $<$<C_COMPILER_ID:Clang>:-w>
        $<$<C_COMPILER_ID:MSVC>:/w>
)

# enable double precision in Unity
target_compile_definitions(unity
    PUBLIC
        UNITY_INCLUDE_DOUBLE
)


# ------------------------------------------------------------------
# 2. STACK TESTS
# ------------------------------------------------------------------
add_executable(test_stack
    stack/test_stack.c
)

target_compile_options(test_stack
    PRIVATE
        $<$<C_COMPILER_ID:GNU>:-Wno-unused-result>
        $<$<C_COMPILER_ID:Clang>:-Wno-unused-result>
)


target_link_libraries(test_stack
    PRIVATE
        tr_datastructures
        unity
)

add_test(
    NAME    test_stack
    COMMAND test_stack
)