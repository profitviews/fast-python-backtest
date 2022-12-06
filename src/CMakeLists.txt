add_library(profitview INTERFACE)
add_library(profitview::profitview ALIAS profitview)
target_sources(profitview
    INTERFACE
        enum.hpp
        format.hpp
        program_options.hpp
)

target_link_libraries(profitview
    INTERFACE
        arrow::arrow
        arrow::parquet
        Boost::boost
        Boost::log
        Boost::program_options
        fmt::fmt
)

target_compile_definitions(profitview
    INTERFACE
        PARQUET_STATIC # Workaround required until this PR is merged: https://github.com/conan-io/conan-center-index/pull/14376
)

target_compile_options(profitview
    INTERFACE
        $<$<CXX_COMPILER_ID:Clang>:-stdlib=libc++>
)

target_link_options(profitview
    INTERFACE
        $<$<CXX_COMPILER_ID:Clang>:-stdlib=libc++ -lc++abi>
)


add_executable(read_parquet)
target_sources(read_parquet
    PRIVATE
        read_parquet.cpp
)

target_link_libraries(read_parquet
    PRIVATE
        profitview::profitview
)

set_target_properties(read_parquet
    PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

add_executable(simple_thread_pool)
target_sources(simple_thread_pool
    PRIVATE
        simple_thread_pool.cpp
)

target_link_libraries(simple_thread_pool
    PRIVATE
        profitview::profitview
)

set_target_properties(simple_thread_pool
    PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

add_subdirectory(extern/pybind11)

pybind11_add_module(simple_pybind11_test simple_pybind11_test.cpp)

set_target_properties(simple_pybind11_test
    PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

pybind11_add_module(simple_pybind11_parquet_test simple_pybind11_parquet_test.cpp)

target_link_libraries(simple_pybind11_parquet_test
    PRIVATE
        profitview::profitview
)


set_target_properties(simple_pybind11_parquet_test
    PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

pybind11_add_module(pybind11_parquet_class_test pybind11_parquet_class_test.cpp)

target_link_libraries(pybind11_parquet_class_test
    PRIVATE
        profitview::profitview
)


set_target_properties(pybind11_parquet_class_test
    PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)