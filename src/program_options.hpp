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

#include "enum.hpp"
#include "format.hpp"

#include <boost/program_options.hpp>
#include <boost/throw_exception.hpp>
#include <boost/log/trivial.hpp>

#include <concepts>
#include <filesystem>
#include <iostream>
#include <optional>

namespace profitview
{

template<BoostDescribeEnum Enum>
void validate(boost::any& v, const std::vector<std::string>& values, Enum*, int)
{
    namespace po = boost::program_options;
    po::validators::check_first_occurrence(v);
    std::string const& s = po::validators::get_single_string(values);

    if (auto const value = fromString<Enum>(s); value.has_value())
    {
        v = value.value();
    }
    else
    {
        boost::throw_exception(po::invalid_option_value(s));
    }
}

// clang-format off
template<typename T>
concept CustomProgramOptions = requires(T& t, boost::program_options::options_description& options)
{
    { t.addOptions(options) } -> std::same_as<void>;
};
// clang-format on

std::optional<int> parseProgramOptions(int argc, char* argv[], CustomProgramOptions auto&... options)
{
    namespace po = boost::program_options;
    try
    {
        po::options_description desc(fmt_ns::format("{}\n\nUsage: ", std::filesystem::path(argv[0]).filename().string()));

        desc.add_options()("help", "produce help message");

        (options.addOptions(desc), ...);

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);

        if (vm.count("help"))
        {
            std::cout << desc << "\n";
            return 1;
        }

        po::notify(vm);
    }
    catch (po::required_option& e)
    {
        BOOST_LOG_TRIVIAL(error) << e.what() << std::endl;
        return 1;
    }
    return std::nullopt;
}

}    // namespace profitview