#pragma once

#if __has_include(<version>)
#   include <version>
#else
#   include <ciso646>
#endif

#if __has_include(<print>)
#include <print>
#endif // #if __has_include(<print>)

#if (__cpp_lib_print >= 202207L)
namespace profitview { namespace print_ns = std; }
#else
#   include <fmt/core.h>
namespace profitview { namespace print_ns = fmt; }
#endif