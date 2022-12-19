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