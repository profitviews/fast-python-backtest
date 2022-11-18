
add_executable(top_of_book top_of_book.cpp)
target_link_libraries(top_of_book
    INTERFACE
        arrow::arrow
        arrow::parquet
        Boost::boost
)

include_directories(${arrow_INCLUDE_DIRS})


