#pragma once

#if __has_include(<version>)
#   include <version>
#else
#   include <ciso646>
#endif

#if __has_include(<format>)
#include <format>
#endif

#if (__cpp_lib_format >= 201907L)
namespace profitview { namespace fmt_ns = std; }
#else
#   include <fmt/format.h>
#   include <fmt/ostream.h>
namespace profitview { namespace fmt_ns = fmt; }
#endif
