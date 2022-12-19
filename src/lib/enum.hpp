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

#include <boost/algorithm/string.hpp>
#include <boost/describe/enumerators.hpp>
#include <boost/mp11.hpp>
#include <optional>
#include <string_view>
#include <type_traits>

#include "format.hpp"

namespace profitview
{

template<class T>
concept BoostDescribeEnum = std::is_enum_v<T> && requires
{
    typename boost::describe::describe_enumerators<T>;
};

template<BoostDescribeEnum Enum>
std::string_view toString(Enum const value, std::string_view result = {})
{
    boost::mp11::mp_for_each<boost::describe::describe_enumerators<Enum>>(
        [&result, value](auto describe)
        {
            if (value == describe.value)
            {
                result = describe.name;
            }
        });
    return result;
}

template<BoostDescribeEnum Enum>
auto fromString(std::string_view const value)
{
    std::optional<Enum> result = std::nullopt;
    boost::mp11::mp_for_each<boost::describe::describe_enumerators<Enum>>(
        [&result, value](auto describe)
        {
            if (boost::iequals(value, describe.name))
            {
                result = describe.value;
            }
        });
    return result;
}

}    // namespace profitview

template<profitview::BoostDescribeEnum Enum>
struct profitview::fmt_ns::formatter<Enum>
{
    template<typename Context>
    constexpr auto parse(Context& context)
    {
        return std::begin(context);
    }

    template<typename Context>
    constexpr auto format(Enum const& value, Context& context) const
    {
        return profitview::fmt_ns::format_to(context.out(), "{}", toString(value));
    }
};
