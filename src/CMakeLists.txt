
add_executable(read_parquet)
target_sources(read_parquet
    PRIVATE
        enum.hpp
        format.hpp
        program_options.hpp
        read_parquet.cpp
)

target_link_libraries(read_parquet
    PRIVATE
        arrow::arrow
        arrow::parquet
        Boost::boost
        Boost::log
        Boost::program_options
        fmt::fmt
)

target_compile_options(read_parquet
    PRIVATE
        $<$<CXX_COMPILER_ID:Clang>:-stdlib=libc++>
)

target_link_options(read_parquet
    PRIVATE
        $<$<CXX_COMPILER_ID:Clang>:-stdlib=libc++ -lc++abi>    
)

set_target_properties(read_parquet
    PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
