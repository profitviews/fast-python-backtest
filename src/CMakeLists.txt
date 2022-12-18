add_library(profitview INTERFACE)
add_library(profitview::profitview ALIAS profitview)
target_sources(profitview
    INTERFACE
        enum.hpp
        format.hpp
)

target_link_libraries(profitview
    INTERFACE
        arrow::arrow
        arrow::parquet
        Boost::boost
        Boost::log
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

add_subdirectory(extern/pybind11)

pybind11_add_module(parquet_table parquet_table.cpp)

target_link_libraries(parquet_table
    PRIVATE
        profitview::profitview
)


set_target_properties(parquet_table
    PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)