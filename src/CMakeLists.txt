
add_executable(top_of_book top_of_book.cpp)
target_link_libraries(top_of_book
    PUBLIC
        arrow::arrow
        arrow::parquet
        Boost::boost
)

set_target_properties(top_of_book
    PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
