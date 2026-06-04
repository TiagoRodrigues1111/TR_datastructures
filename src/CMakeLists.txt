# ------------------------------------------------------------------
# 1. LIBRARY TARGET
# ------------------------------------------------------------------
add_library(tr_datastructures)
add_library(tr_datastructures::tr_datastructures ALIAS tr_datastructures)

# ------------------------------------------------------------------
# 2. SOURCES
# ------------------------------------------------------------------
target_sources(tr_datastructures
    PRIVATE
        tr_datastructures.c
        stack/stack.c
)

# ------------------------------------------------------------------
# 3. INCLUDE DIRECTORIES
# ------------------------------------------------------------------
target_include_directories(tr_datastructures
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>   
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/internal/include
)

# ------------------------------------------------------------------
# 4. COMPILE DEFINITIONS
# ------------------------------------------------------------------
target_compile_definitions(tr_datastructures
    PRIVATE
        $<$<CONFIG:Debug>:TR_CMAKE_DEBUG>
        $<$<BOOL:${BUILD_SHARED_LIBS}>:TR_DATASTRUCTURES_SHARED>
        $<$<BOOL:${BUILD_SHARED_LIBS}>:TR_DATASTRUCTURES_EXPORTS>
    INTERFACE
        $<$<BOOL:${BUILD_SHARED_LIBS}>:TR_DATASTRUCTURES_SHARED>
)


# ------------------------------------------------------------------
# 5. INSTALL TARGET
# ------------------------------------------------------------------
install(
    TARGETS tr_datastructures
    EXPORT  tr_datastructures-targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(
    DIRECTORY ${PROJECT_SOURCE_DIR}/include/tr_datastructures
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
    FILES
        "${CMAKE_CURRENT_BINARY_DIR}/../include/tr_datastructures/tr_version.h"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/tr_datastructures"
)

install(
    EXPORT      tr_datastructures-targets
    FILE        tr_datastructures-targets.cmake
    NAMESPACE   tr_datastructures::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/tr_datastructures
)