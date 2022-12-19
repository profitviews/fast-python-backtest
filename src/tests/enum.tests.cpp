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
#include "enum.hpp"

#include <catch2/catch.hpp>
#include <boost/describe/enum.hpp>

namespace profitview
{

enum Colours
{
    Green,
    Yellow,
    Red
};

BOOST_DESCRIBE_ENUM(Colours, Green, Yellow, Red);

TEST_CASE("Ensure enums are convertable to string", "[enum.to_string]")
{
    REQUIRE(toString(Green) == "Green");
    REQUIRE(toString(Yellow) == "Yellow");
    REQUIRE(toString(Red) == "Red");
}

TEST_CASE("Ensure enums are convertable from string", "[enum.from_string]")
{
    REQUIRE(fromString<Colours>("Green") == Green);
    REQUIRE(fromString<Colours>("GREEN") == Green);
    REQUIRE(fromString<Colours>("GrEeN") == Green);
    REQUIRE(fromString<Colours>("yellow") == Yellow);
    REQUIRE(fromString<Colours>("YELLOW") == Yellow);
    REQUIRE(fromString<Colours>("yElLoW") == Yellow);
    REQUIRE(fromString<Colours>("Red") == Red);
    REQUIRE(fromString<Colours>("rEd") == Red);
    REQUIRE(fromString<Colours>("RED") == Red);
    REQUIRE(!fromString<Colours>("Invalid"));
}

}