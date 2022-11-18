
add_executable(top_of_book)
target_sources(top_of_book
    PRIVATE
        enum.hpp
        format.hpp
        program_options.hpp
        top_of_book.cpp
)

target_link_libraries(top_of_book
    PUBLIC
        arrow::arrow
        arrow::parquet
        Boost::boost
        Boost::log
        Boost::program_options
        fmt::fmt
)

set_target_properties(top_of_book
    PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
