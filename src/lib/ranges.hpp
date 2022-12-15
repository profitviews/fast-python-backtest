/*
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
*/
#pragma once

#if __has_include(<version>)
#    include <version>
#else
#    include <ciso646>
#endif

#if __has_include(<ranges>)
#    include <ranges>
#endif

#if (__cpp_lib_ranges >= 202110L)
#    include <algorithm>
#    include <iterator>
namespace profitview
{
namespace ranges = std::ranges;
}

namespace std::ranges
{
using std::back_inserter;    // https://github.com/ericniebler/range-v3/issues/867
}

#else
#    include <range/v3/algorithm.hpp>
#    include <range/v3/iterator.hpp>
#    include <range/v3/range/access.hpp>
#    include <range/v3/view.hpp>
namespace profitview
{
namespace ranges = ::ranges;
}
#endif    // (__cpp_lib_ranges >= 202202L)
