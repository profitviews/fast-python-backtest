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

#include "format.hpp"

namespace profitview
{

/// \class Version
///     Simple type representing semantic versioning: https://semver.org/
struct Version
{
    std::uint16_t max = 0;
    std::uint16_t min = 0;
    std::uint16_t patch = 0;
};

} // namespace profitview

template <>
struct profitview::fmt_ns::formatter<profitview::Version>
{
    template <typename Context>
    constexpr auto parse(Context& context)
    {
        return std::begin(context);
    }

    template <typename Context>
    constexpr auto format(profitview::Version const& value, Context& context) const
    {
        return profitview::fmt_ns::format_to(
            context.out(), "{{max={},min={},patch={}}}", value.max, value.min, value.patch);
    }
};
