#[[
Copyright 2022 Profitview

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
documentation files (the "Software"), to deal in the Software without restriction, including without limitation the
rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit
persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the
Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
]]

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
